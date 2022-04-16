#include <Arduino.h>
#include "BMI088.h"

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

Bmi088Accel accel(Wire, 0x18);
Bmi088Gyro gyro(Wire, 0x68);

float f_accelX;
float f_accelY;
float f_accelZ;
float f_gyroX;
float f_gyroY;
float f_gyroZ;

void setup() {
    state = nav_converge;
    Serial.begin(9600);

    int status;

    status = accel.begin();
    if (status < 0) {
        Serial.println("ERROR: failed accel init");
    }

    status = gyro.begin();
    if (status < 0) {
        Serial.println("ERROR: failed gyro init");
    }

}

void loop() {
    switch (state) {

        case nav_converge:
            break;

        case pad_idle:
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