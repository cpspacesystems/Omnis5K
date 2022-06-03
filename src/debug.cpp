#include <Arduino.h>
#include "debug.h"

void debug_begin() {
    Serial.begin(9600);
}

void debug_print(const char *message) {
#ifdef DEBUG_SERIAL
    Serial.print(message);
#endif
}

void debug_print(float value) {
#ifdef DEBUG_SERIAL
    Serial.print(value);
#endif
}

void debug_println(const char *message) {
#ifdef DEBUG_SERIAL
    Serial.println(message);
#endif
}

void debug_println(float value) {
#ifdef DEBUG_SERIAL
    Serial.println(value);
#endif
}