#include "error.h"

uint8_t f_errorStatus = 0x00;

void error_assert(int val, int no_err_val, int err_offset, const char* err_message) {
    if (val != no_err_val) {
        Serial.print("ERROR: ");
        Serial.println(err_message);
        f_errorStatus |= 1 << err_offset;
    }
}

bool error_ok() {
    return f_errorStatus == 0;
}

uint8_t error_status() {
    return f_errorStatus;
}