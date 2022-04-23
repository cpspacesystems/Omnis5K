#include <Arduino.h>
#include "BMI088.h"

#include "data.h"

/* Enum responsible for representing the states of flight:

  nav_converge:   calibration of the GPS, IMU, and Barometer
  pad_idle:       waiting for rocket burn, begin position tracking
  burn:           begin data logging
  coast:          dump data if necessary TODO: determine if needed due to space limitations
  post_apogee:    deploy water from nose cone
  chute_deployed: dump data if necessary TODO: determine if needed due to space limitations
  grounded:       end data logging, end position tracking, dump all logged data onto the SD card

*/

enum system_state {
    nav_converge,
    pad_idle,
    burn,
    coast,
    post_apogee,
    chute_deployed,
    grounded
} state;

void setup() {
    state = nav_converge;
    Serial.begin(9600);
    
    initSensors();
    delay(500);
}

void loop() {

    //debugLog();

    switch (state) {

        case nav_converge:
            calibrateSensors();
            logCalibration();

            if (millis() > 10000) {
                state = pad_idle;
                Serial.println("STATE: Moving to pad_idle state");
            }
            break;

        case pad_idle:
            updateSensors();
            logSensors();
            break;

        case burn:
            break;

        case coast:
            break;

        case post_apogee:
            break;

        case chute_deployed:
            break;

        case grounded:
            break;

    }

}