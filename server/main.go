package main

import (
	"log"
	"os"
    //"strconv"
    "net/http"
    "fmt"
    "time"

    "github.com/labstack/echo/v5"
    "github.com/pocketbase/dbx"
    "github.com/pocketbase/pocketbase"
    "github.com/pocketbase/pocketbase/apis"
    "github.com/pocketbase/pocketbase/core"
    //"github.com/pocketbase/pocketbase/tokens"
    "github.com/pocketbase/pocketbase/models"
    //"github.com/pocketbase/pocketbase/tools/template"
)



func cookieAuthMiddleware(next echo.HandlerFunc) echo.HandlerFunc {
    return func(c echo.Context) error {

        cookie, err := c.Cookie("pb_auth")
        if err != nil {
            return err
        }
        //fmt.Println(cookie.Name)
        //fmt.Println(cookie.Value)
        c.Request().Header.Set("Authorization", cookie.Value)
        return next(c)            
    }
}


func main() {
    app := pocketbase.New()

    app.OnBeforeServe().Add(func(e *core.ServeEvent) error {
        e.Router.PATCH("/register", func(c echo.Context) error {
            
            ///payload := apis.RequestInfo(c).Data
            deviceId := c.Request().Header.Get("x-irdom-id")

            if (len(deviceId) == 0) {
                return apis.NewBadRequestError("Invalid device", nil)
            }
            
            record, err := app.Dao().FindFirstRecordByFilter(
                "devices", "deviceId = {:deviceId}",
                dbx.Params{ "deviceId": deviceId },
            )

            if err != nil {
                collection, err := app.Dao().FindCollectionByNameOrId("devices")
                if err != nil {
                    return err
                }

                record = models.NewRecord(collection)
                record.Set("deviceId", deviceId)

                if err := app.Dao().SaveRecord(record); err != nil {
                    return err
                }

            } else {
                record.Set("updated", time.Now())

                if err := app.Dao().SaveRecord(record); err != nil {
                    return err
                }
            }
            
            
            return c.JSON(http.StatusOK, map[string]any{"deviceId": record.GetString("deviceId"), "codes": record.Get("codes")})
        })
    
        return nil
    })


    app.OnBeforeServe().Add(func(e *core.ServeEvent) error {
        e.Router.POST("/button-code", func(c echo.Context) error {

            type IRCode struct {
                Slot       int `json:"slot" form:"slot"`
                Protocol   int `json:"protocol" form:"protocol"`
                ButtonCode string `json:"buttonCode" form:"buttonCode"`
                Size       int `json:"size" form:"size"`
            }

            var irCode[] IRCode

            if err := c.Bind(&irCode); err != nil {
                return apis.NewBadRequestError("Failed to read request data", err)
            }


            fmt.Println(irCode)

            
            ///payload := apis.RequestInfo(c).Data
            deviceId := c.Request().Header.Get("x-irdom-id")

            if (len(deviceId) == 0) {
                return apis.NewBadRequestError("Invalid device", nil)
            }
            
            record, err := app.Dao().FindFirstRecordByFilter(
                "devices", "deviceId = {:deviceId}",
                dbx.Params{ "deviceId": deviceId },
            )

            if err != nil {
                return apis.NewNotFoundError("Device not found", nil)
            }
            

            
            record.Set("codes", irCode)

            if err := app.Dao().SaveRecord(record); err != nil {
                return err
            }
            
            return c.String(http.StatusOK, "OK")
        })
    
        return nil
    })


    app.OnBeforeServe().Add(func(e *core.ServeEvent) error {
        e.Router.GET("/schedule", func(c echo.Context) error {
            
            ///payload := apis.RequestInfo(c).Data
            deviceId := c.Request().Header.Get("x-irdom-id")

            if (len(deviceId) == 0) {
                return apis.NewBadRequestError("Invalid device", nil)
            }

            records := []*models.Record{}

            query := app.Dao().RecordQuery("schedule").
                InnerJoin("devices", dbx.NewExp("devices.id = schedule.deviceId")).    
                Where(dbx.NewExp("devices.deviceId = {:deviceId}", dbx.Params{ "deviceId": deviceId })).
                AndWhere(dbx.HashExp{"schedule.acknowledged": false}).
                OrderBy("schedule.created DESC")

            if err := query.All(&records); err == nil {
                if (len(records) > 0) {

                    schedule, err := app.Dao().FindRecordById("schedule", records[0].GetString("id"))

                    if (err != nil) {
                        return err
                    }

                    schedule.Set("acknowledged", true)
                    if err := app.Dao().SaveRecord(schedule); err != nil {
                        return err
                    }

                    if errs := app.Dao().ExpandRecord(records[0], []string{"code"}, nil); len(errs) > 0 {
                        return fmt.Errorf("failed to expand: %v", errs)
                    }
                    
                    return c.JSON(http.StatusOK, map[string]any{
                        "slot": records[0].ExpandedOne("code").GetInt("slot"),
                        "protocol": records[0].ExpandedOne("code").GetInt("protocol"), 
                        "size":records[0].ExpandedOne("code").GetInt("size"),
                        "signal": records[0].ExpandedOne("code").GetString("signal"),
                    })


                }
            }

            return apis.NewNotFoundError("No scheduled codes", records)
        })
    
        return nil
    })




    // serves static files from the provided public dir (if exists)
    app.OnBeforeServe().Add(func(e *core.ServeEvent) error {
        e.Router.GET("/*", apis.StaticDirectoryHandler(os.DirFS("./pb_public"), false))
        return nil
    })


    if err := app.Start(); err != nil {
        log.Fatal(err)
    }
}