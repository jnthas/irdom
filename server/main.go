package main

import (
	"log"
	//"os"
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
            irdomId := c.Request().Header.Get("x-irdom-id")

            if (len(irdomId) == 0) {
                return apis.NewBadRequestError("Invalid device", nil)
            }
            
            record, err := app.Dao().FindFirstRecordByFilter(
                "devices", "irdomId = {:irdomId}",
                dbx.Params{ "irdomId": irdomId },
            )

            if err != nil {
                collection, err := app.Dao().FindCollectionByNameOrId("devices")
                if err != nil {
                    return err
                }

                record = models.NewRecord(collection)
                record.Set("irdomId", irdomId)

                if err := app.Dao().SaveRecord(record); err != nil {
                    return err
                }

            } else {
                record.Set("updated", time.Now())

                if err := app.Dao().SaveRecord(record); err != nil {
                    return err
                }
            }
            
            
            return c.JSON(http.StatusOK, map[string]any{"irdomId": record.GetString("irdomId"), "codes": record.Get("codes")})
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
            irdomId := c.Request().Header.Get("x-irdom-id")

            if (len(irdomId) == 0) {
                return apis.NewBadRequestError("Invalid device", nil)
            }
            
            record, err := app.Dao().FindFirstRecordByFilter(
                "devices", "irdomId = {:irdomId}",
                dbx.Params{ "irdomId": irdomId },
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


    if err := app.Start(); err != nil {
        log.Fatal(err)
    }
}