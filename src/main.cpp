#include <Arduino.h>
#include "BMI088.h"

#include "data.h"
#include "telemetry.h"
#include "log.h"

// State thresholds

#define BURN_ACCEL 12 // Acceleration (m/s2) which when exceeded the state will change to burn
#define APOGEE_THRESH 0.5 // The distance (m) between the highest altitude recorded and the current altitude to switch to descent
#define GROUND_TIME_THRESH 10000 // number of millis which the vertical velocity must be below the ground velocity threshold for the state to switch to ground
#define GROUND_VEL_THRESH 2 // m/s threshold of the ground velocity

/* Enum responsible for representing the states of flight:

  nav_converge:   calibration of the GPS, IMU, and Barometer
  pad_idle:       waiting for rocket burn, begin position tracking
  burn:           begin data logging
  coast:          dump data if necessary TODO: determine if needed due to space limitations
  post_apogee:    deploy water from nose cone
  chute_deployed: dump data if necessary TODO: determine if needed due to space limitations
  grounded:       end data logging, end position tracking, dump all logged data onto the SD card

*/

uint32_t f_startTime;
uint32_t f_groundCheckTime;

enum system_state {
    nav_converge,
    pad_idle,
    ascent,
    descent,
    grounded
} state;

void setup() {

    state = nav_converge;
    Serial.begin(9600);

    data_init();
    log_init();
    telemetry_init();
    delay(500);
    f_startTime = millis();
    Serial.println("STATE: Moving to nav_converge state");
}

void loop() {

    data_update();
    telemetry_send();
    telemetry_receive();

    switch (state) {
        case nav_converge:
            //Serial.println("NAV_CONV ");

            if (data_calibrate() > 100) {
                state = pad_idle;
                Serial.println("STATE: Moving to pad_idle state");
            }
            break;


        case pad_idle:
            if (data_accel() > BURN_ACCEL) {
                state = ascent;
                Serial.println("STATE: Moving to ascent state");
            }
            break;

        case ascent:
            log_logFrame(state);

            if (data_maxAltitude() > data_smoothAltitude() + APOGEE_THRESH) {
                state = descent;
                f_groundCheckTime = millis();
                Serial.println("STATE: Moving to descent state");
            }
            break;

        case descent:
            log_logFrame(state);

            if (abs(data_velocityX()) < GROUND_VEL_THRESH) {
                if (millis() - f_groundCheckTime > GROUND_TIME_THRESH) {
                    state = grounded;
                    log_dumpToSD();
                    Serial.println("STATE: Moving to grounded state");
                }
            } else {
                f_groundCheckTime = millis();
            }
            break;

        case grounded:
            break;

    }

    //logSensors();

}