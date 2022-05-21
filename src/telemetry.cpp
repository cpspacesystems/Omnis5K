#include <Arduino.h>
#include "data.h"
#define TEL Serial5
#define BAUD 57600

void telemetry_init() {
    Serial.println("INIT: Telemetry");
    TEL.begin(BAUD);
}

void telemetry_send() {

    float arr[DATA_FRAME_SIZE];
    data_valuesArray(arr);

    for (int i = 0; i < DATA_FRAME_SIZE - 1; i++) {
        TEL.print(arr[i]);
        TEL.print(",");
    }
    TEL.print(arr[DATA_FRAME_SIZE - 1]);

    TEL.println();
}