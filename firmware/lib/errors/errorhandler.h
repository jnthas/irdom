#pragma once

#include <Arduino.h>
#include "display7.h"

namespace ErrorHandler {

    // const char *DESC_WIFI_OFFLINE PROGMEM = "Device is not connected";
    // const char *DESC_IR_SEND_SIGNAL_ERROR PROGMEM = "Error sending IR";


    enum EnumDefaultErrors { 
        GENERIC_ERROR,
        WIFI_OFFLINE,
        IR_BUFFER_OVERFLOW,
        IR_SEND_SIGNAL_ERROR,
        BACKEND_REGISTER_FAILED
    };


    void handleError(EnumDefaultErrors error);
    void handleError(EnumDefaultErrors error, const char *description);

}
