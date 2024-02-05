package main

import (
	"log"
	"os"
    "strconv"
    "net/http"
    "fmt"
    "time"

    "github.com/labstack/echo/v5"
    "github.com/pocketbase/dbx"
    "github.com/pocketbase/pocketbase"
    "github.com/pocketbase/pocketbase/apis"
    "github.com/pocketbase/pocketbase/core"
    "github.com/pocketbase/pocketbase/tokens"
    "github.com/pocketbase/pocketbase/models"
    "github.com/pocketbase/pocketbase/tools/template"
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

    // serves static files from the provided public dir (if exists)
    app.OnBeforeServe().Add(func(e *core.ServeEvent) error {
        e.Router.GET("/*", apis.StaticDirectoryHandler(os.DirFS("./pb_public"), false))
        return nil
    })

	app.OnBeforeServe().Add(func(e *core.ServeEvent) error {
        // register new "GET /hello" route
        e.Router.GET("/hello", func(c echo.Context) error {
            fmt.Println("Hello")
            return c.String(200, "Hello world!")

        })

        return nil
    })


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
        e.Router.POST("/register2", func(c echo.Context) error {
            
            payload := apis.RequestInfo(c).Data
            otp := c.Request().Header.Get("x-otp-code")

            decOtp, err := strconv.ParseInt(otp, 16, 16)
            if (err != nil || decOtp == 0) {
                return apis.NewBadRequestError("Invalid OTP code", nil)
            }


            record, err := app.Dao().FindFirstRecordByFilter(
                "devices", "macAddress = null && otpCode = {:otp}",
                dbx.Params{ "otp": otp },
            )

            if err != nil {
                return err
            }

            
            for k, v := range payload {
                record.Set(k, v)
            }
            record.Set("otpCode", nil)

            if err := app.Dao().SaveRecord(record); err != nil {
                return err
            }
            
            
            return c.JSON(http.StatusOK, map[string]string{"userId": record.GetString("userId")})
        })
    
        return nil
    })


    app.OnBeforeServe().Add(func(e *core.ServeEvent) error {
        
        registry := template.NewRegistry()

        e.Router.GET("/login", func(c echo.Context) error {
            
            html, err := registry.LoadFiles(
                "views/login.html",
            ).Render(map[string]any{
                "name": "teste",
            })

            if err != nil {
                // or redirect to a dedicated 404 HTML page
                return apis.NewNotFoundError("", err)
            }

            return c.HTML(http.StatusOK, html)
        })
    
        return nil
    })


    app.OnBeforeServe().Add(func(e *core.ServeEvent) error {
        
        e.Router.POST("/login", func(c echo.Context) error {
            
            data := &struct {
				Email    string `form:"email" json:"email"`
				Password string `form:"password" json:"password"`
			}{}

			// read the request data
			if err := c.Bind(data); err != nil {
				return apis.NewBadRequestError("Failed to read request data", err)
			}

			// fetch the user and check the provided password
			record, err := app.Dao().FindAuthRecordByEmail("users", data.Email)
			if err != nil || !record.ValidatePassword(data.Password) {
				return apis.NewBadRequestError("Invalid login credentials", err)
			}

			// generate a new auth token for the found user record
			token, err := tokens.NewRecordAuthToken(app, record)
			if err != nil {
				return apis.NewBadRequestError("Failed to create an auth token", err)
			}

			// set it as cookie
			cookie := &http.Cookie{
				Name:     "pb_auth",
				Value:    token,
				Secure:   false,
				HttpOnly: true,
				SameSite: http.SameSiteStrictMode,
				Expires:  time.Now().Add(time.Duration(app.Settings().RecordAuthToken.Duration) * time.Second),
			}

			c.SetCookie(cookie)
            //c.Response().Header().Set("Location", "/home")
            return c.Redirect(303, "/home")
        })
    
        return nil
    })
    


    app.OnBeforeServe().Add(func(e *core.ServeEvent) error {

        registry := template.NewRegistry()

        e.Router.GET("/home", func(c echo.Context) error {
            
            html, err := registry.LoadFiles(
                "views/home.html",
            ).Render(map[string]any{
                "name": "jonathas",
            })

            if err != nil {
                // or redirect to a dedicated 404 HTML page
                return apis.NewNotFoundError("", err)
            }

            return c.HTML(http.StatusOK, html)
        }, apis.ActivityLogger(app), cookieAuthMiddleware, apis.RequireRecordAuth())
    
        return nil
    })



    



    if err := app.Start(); err != nil {
        log.Fatal(err)
    }
}