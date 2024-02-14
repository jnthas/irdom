#include "errorhandler.h"

namespace ErrorHandler {


    void handleError(EnumDefaultErrors error) {
        handleError(error, "NO DESCRIPTION");
    }

    void handleError(EnumDefaultErrors error, const char *description) {
        
        Serial.printf("\n>>> [ERROR] %s, code: %d\n", description, error);
        Display7::print("Er"); //make it blink

    }

}