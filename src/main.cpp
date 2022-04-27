#include <Arduino.h>
#include "BMI088.h"

#include "data.h"
#include "telemetry.h"

// State thresholds

#define BURN_ACCEL 12 // Acceleration (m/s2) which when exceeded the state will change to burn
#define COAST_ACCEL 10 // When the acceleration (m/s2) is under this value, the state will change from burn to coast
#define APOGEE_THRESH 1 // The distance (m) between the highest altitude recorded and the current altitude to switch to post_apogee

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
    
    data_init();
    telemetry_init();
    delay(500);
}

void loop() {

    data_update();
    telemetry_send();

    switch (state) {
        case nav_converge:
            Serial.print("NAV_CONV ");

            data_calibrate();

            if (millis() > 10000) {
                state = pad_idle;
                Serial.println("STATE: Moving to pad_idle state");
            }
            break;

        case pad_idle:
            Serial.print("PAD_IDLE ");

            if (data_accel() > BURN_ACCEL) {
                state = burn;
                Serial.println("STATE: Moving to burn state");
            }
            break;

        case burn:
            Serial.print("BURN ");

            if (data_accel() < COAST_ACCEL) {
                state = coast;
                Serial.println("STATE: Moving to coast state");
            }
            break;

        case coast:
            Serial.print("COAST ");

            if (data_maxAltitude() > data_smoothAltitude() + APOGEE_THRESH) {
                state = post_apogee;
                Serial.println("STATE: Moving to post_apogee state");
            }
            break;

        case post_apogee:
            Serial.print("POST APOGEE ");
            break;

        case chute_deployed:
            break;

        case grounded:
            break;

    }

    logSensors();

}