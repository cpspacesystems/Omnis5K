#include <Arduino.h>
#define TEL Serial2
#define BAUD 57600

void telemetry_init() {
    TEL.begin(BAUD);
}

void telemetry_send() {
    TEL.println("Hello");
}