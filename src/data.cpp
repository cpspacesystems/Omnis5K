#include "BMI088.h"
#include "Quaternion.h"
#include <Arduino.h>

Bmi088Accel accel(Wire, 0x18);
Bmi088Gyro gyro(Wire, 0x68);

float f_oldTime;
float f_newTime;
float f_deltaTime;

float f_accelX;
float f_accelY;
float f_accelZ;

float f_gyroX;
float f_gyroY;
float f_gyroZ;

Quaternion orientation;

void setupSensors() {

    int status;

    status = accel.begin();
    if (status < 0) {
        Serial.println("ERROR: failed accel init");
    }

    status = gyro.begin();
    if (status < 0) {
        Serial.println("ERROR: failed gyro init");
    }

    orientation = Quaternion();

    f_oldTime = millis();
    f_newTime = 0.0f;
    f_deltaTime = 0.0f;

}

void updateSensors() {

    f_newTime = millis();
    f_deltaTime = (f_newTime - f_oldTime) / 1000.0f;
    f_oldTime = f_newTime;

    // Update accelerometer data

    accel.readSensor();

    f_accelX = accel.getAccelX_mss();
    f_accelY = accel.getAccelY_mss();
    f_accelZ = accel.getAccelZ_mss();

    // Update gyroscope data

    gyro.readSensor();

    f_gyroX = gyro.getGyroX_rads();
    f_gyroY = -gyro.getGyroY_rads();
    f_gyroZ = gyro.getGyroZ_rads();

    // Integrate gyro data

    float norm = sqrtf(sq(f_gyroX) + sq(f_gyroY) + sq(f_gyroZ));
    norm = copysignf(max(abs(norm), 1e-9), norm);

    Quaternion a = Quaternion::from_axis_angle(f_deltaTime * norm, f_gyroX / norm, f_gyroY / norm, f_gyroZ / norm);

    a *= orientation;
    orientation = a;

}

void logOrientation() {
    Serial.print(orientation.a);
    Serial.print(", ");
    Serial.print(orientation.b);
    Serial.print(", ");
    Serial.print(orientation.c);
    Serial.print(", ");
    Serial.println(orientation.d);
}

void logAccel() {
    Serial.print(f_accelX);
    Serial.print(", ");
    Serial.print(f_accelY);
    Serial.print(", ");
    Serial.println(f_accelZ);
}