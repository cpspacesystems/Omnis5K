#include <Arduino.h>
#include "BMI088.h"

#include "state.h"
#include "data.h"
#include "telemetry.h"
#include "log.h"

// State thresholds

#define BURN_ACCEL 5 // Acceleration (m/s2) which when exceeded the state will change to burn
#define APOGEE_THRESH 4 // The distance (m) between the highest altitude recorded and the current altitude to switch to descent
#define GROUND_TIME_THRESH 10000 // number of millis which the vertical velocity must be below the ground velocity threshold for the state to switch to ground
#define GROUND_VEL_THRESH 2 // m/s threshold of the ground velocity

uint32_t f_startTime;
uint32_t f_groundCheckTime;

uint8_t state;

void setup() {

    state = NAV_CONVERGE;
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
    telemetry_send(state);
    telemetry_receive();

    switch (state) {
        case NAV_CONVERGE:

            if (data_calibrate()) {
                state = PAD_IDLE;
                Serial.println("STATE: Moving to pad_idle state");
            }
            break;


        case PAD_IDLE:
            if (data_accel() > BURN_ACCEL) {
                state = ASCENT;
                Serial.println("STATE: Moving to ascent state");
            }
            break;

        case ASCENT:
            log_logFrame(state);

            if (data_maxAltitude() > data_smoothAltitude() + APOGEE_THRESH) {
                state = DESCENT;
                Serial.println("STATE: Moving to descent state");
                f_groundCheckTime = millis();
                // eject water here
            }
            break;

        case DESCENT:
            log_logFrame(state);

            if (abs(data_velocityX()) < GROUND_VEL_THRESH) {
                if (millis() - f_groundCheckTime > GROUND_TIME_THRESH) {
                    state = GROUNDED;
                    Serial.println("STATE: Moving to grounded state");
                    log_dumpToSD();
                }
            } else {
                f_groundCheckTime = millis();
            }
            break;

        case GROUNDED:
            break;

    }

    //logSensors();

}