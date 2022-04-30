#include <Arduino.h>
#include "BMI088.h"

#include "data.h"
#include "telemetry.h"
#include "log.h"

// State thresholds

#define BURN_ACCEL 12 // Acceleration (m/s2) which when exceeded the state will change to burn
#define APOGEE_THRESH 0.5 // The distance (m) between the highest altitude recorded and the current altitude to switch to post_apogee

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
                Serial.println("STATE: Moving to descent state");
            }
            break;

        case descent:
            // log_logFrame(state);
            log_dumpToSD(); // This needs to be updated
            state = grounded;
            break;

        case grounded:
            break;

    }

    //logSensors();

}