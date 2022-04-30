#include <Arduino.h>
#define TEL Serial5
#define BAUD 57600

void telemetry_init() {
    Serial.println("INIT: Telemetry");
    TEL.begin(BAUD);
}

void telemetry_send() {
    TEL.println("Hello");
}