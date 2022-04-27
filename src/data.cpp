#include "BMI088.h"
#include "Adafruit_BMP3XX.h"
#include "Quaternion.h"
#include "SPIMemory.h"
#include <Arduino.h>

#define FLASH_PIN 0x0A
#define ACCEL_I2C 0x18
#define GYRO_I2C 0x68
#define BARO_I2C 0x76

#define ACCEL_ERR_OFFSET 0
#define GYRO_ERR_OFFSET 1
#define BARO_ERR_OFFSET 2
#define FLASH_ERR_OFFSET 3

const float u_groundLevel = 103.632f;
const float u_altitudeConstant = pow(1 - u_groundLevel / 44330.0, -5.2549) / 100.0;

// Calibration values

unsigned int c_calibrations = 0;

float c_groundPressure = 0;
float c_seaPressure;

float c_gyroBiasX;
float c_gyroBiasY;
float c_gyroBiasZ;

// Sensor objects

Bmi088Accel accel(Wire, ACCEL_I2C);
Bmi088Gyro gyro(Wire, GYRO_I2C);
Adafruit_BMP3XX baro;
SPIFlash flash(FLASH_PIN);

// Flight variables

float f_oldTime;
float f_newTime;
float f_deltaTime;

float f_pressure;
float f_ASL;
float f_AGL;
float f_temperature;

float f_smoothAGL = 0;
float f_maxAltitude = 0;

float f_accelX;
float f_accelY;
float f_accelZ;
float f_accelMag;

float f_gyroX;
float f_gyroY;
float f_gyroZ;

uint8_t f_initStatus = 0x00;

Quaternion orientation;

void data_err(int val, int no_err_val, int err_offset, String err_message) {
    if (val != no_err_val) {
        Serial.println("ERROR: " + err_message);
        f_initStatus |= 1 << err_offset;
    }
}

void data_init() {

    Serial.println("INIT: baro");

    data_err(baro.begin_I2C(BARO_I2C), 1, BARO_ERR_OFFSET, "baro.begin_I2C");
    data_err(baro.setTemperatureOversampling(BMP3_NO_OVERSAMPLING), 1, BARO_ERR_OFFSET, "baro.setTemperatureOversampling");
    data_err(baro.setPressureOversampling(BMP3_OVERSAMPLING_8X), 1, BARO_ERR_OFFSET, "baro.setPressureOversampling");
    data_err(baro.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_15), 1, BARO_ERR_OFFSET, "baro.setIIRFilterCoeff");
    data_err(baro.setOutputDataRate(BMP3_ODR_200_HZ), 1, BARO_ERR_OFFSET, "baro.setOutputDataRate");

    Serial.println("INIT: Flash");

    data_err(flash.begin(), 1, FLASH_ERR_OFFSET, "flash.initialize()");

    Serial.print("INIT: Accel");

    data_err(accel.begin(), 1, ACCEL_ERR_OFFSET, "accel.begin()");
    data_err(accel.setOdr(Bmi088Accel::ODR_1600HZ_BW_280HZ), 1, ACCEL_ERR_OFFSET, "accel.setOdr()");
    data_err(accel.setRange(Bmi088Accel::RANGE_12G), 1, ACCEL_ERR_OFFSET, "accel.setRange()");
    
    Serial.println("INIT: Gyro");

    data_err(gyro.begin(), 1, GYRO_ERR_OFFSET, "gyro.begin()");
    data_err(gyro.setOdr(Bmi088Gyro::ODR_400HZ_BW_47HZ), 1, GYRO_ERR_OFFSET, "gyro.setOdr()");
    data_err(gyro.setRange(Bmi088Gyro::RANGE_1000DPS), 1, GYRO_ERR_OFFSET, "gyro.setRange()");

    orientation = Quaternion();

    f_oldTime = millis();
    f_newTime = 0.0f;
    f_deltaTime = 0.0f;
}

void data_calibrate() {

    baro.performReading();
    gyro.readSensor();

    if (c_calibrations == 0) {
        c_groundPressure = baro.readPressure();
        c_gyroBiasX = 0;
        c_gyroBiasY = 0;
        c_gyroBiasZ = 0;
    
    } else {
        c_groundPressure += (baro.readPressure() - c_groundPressure) / c_calibrations;
        c_seaPressure = c_groundPressure * u_altitudeConstant;

        c_gyroBiasX += (gyro.getGyroX_rads() - c_gyroBiasX) / c_calibrations;
        c_gyroBiasY += (gyro.getGyroY_rads() - c_gyroBiasY) / c_calibrations;
        c_gyroBiasZ += (gyro.getGyroZ_rads() - c_gyroBiasZ) / c_calibrations;
    }

    c_calibrations++;
}

void data_update() {

    // Update timing

    f_newTime = millis();
    f_deltaTime = (f_newTime - f_oldTime) / 1000.0f;
    f_oldTime = f_newTime;

    // Update barometer data

    baro.performReading();
    f_pressure = baro.readPressure();
    f_ASL = baro.readAltitude(c_seaPressure);
    f_AGL = f_ASL - u_groundLevel;
    f_temperature = baro.readTemperature();
    if (c_calibrations > 10) f_smoothAGL = f_smoothAGL + (f_AGL - f_smoothAGL) * 0.05f;

    if (f_smoothAGL > f_maxAltitude) {
        f_maxAltitude = f_smoothAGL;
    }

    // Update accelerometer data

    accel.readSensor();

    f_accelX = accel.getAccelX_mss();
    f_accelY = accel.getAccelY_mss();
    f_accelZ = accel.getAccelZ_mss();
    f_accelMag = sqrt(sq(f_accelX) + sq(f_accelY) + sq(f_accelZ));

    // Update gyroscope data

    gyro.readSensor();

    f_gyroX = gyro.getGyroX_rads();
    f_gyroY = -gyro.getGyroY_rads();
    f_gyroZ = gyro.getGyroZ_rads();

    // Integrate gyro data

    float norm = sqrtf(sq(f_gyroX) + sq(f_gyroY) + sq(f_gyroZ));
    norm = copysignf(max(abs(norm), 1e-9), norm);

    Quaternion a = Quaternion::from_axis_angle(f_deltaTime * norm, f_gyroX / norm, f_gyroY / norm, f_gyroZ / norm);

    orientation = a * orientation;
}

float data_accel() {
    return f_accelMag;
}

float data_altitude() {
    return f_AGL;
}

float data_smoothAltitude() {
    return f_smoothAGL;
}

float data_maxAltitude() {
    return f_maxAltitude;
}

void logCalibration() {
    Serial.print("Sea Pressure (pa): " + String(c_seaPressure) + "\t");
    Serial.print("Gyro bias (r/s2): " + String(c_gyroBiasX) + ", " + String(c_gyroBiasY) + ", " + String(c_gyroBiasZ) + "\t");
    Serial.println();
}

void logSensors() {

    Serial.print("Accel (m/s2): ");
    Serial.print(f_accelMag);
    Serial.print("\t");

    //Serial.print("Accel (m/s2): ");
    //Serial.print(f_accelX);
    //Serial.print(", ");
    //Serial.print(f_accelY);
    //Serial.print(", ");
    //Serial.print(f_accelZ);
    //Serial.print("\t");

    Serial.print("Smooth Alt (m): ");
    Serial.print(f_smoothAGL);
    Serial.print("\t");

    Serial.print("Max alt (m): ");
    Serial.print(f_maxAltitude);
    Serial.print("\t");

    Serial.print("Temp (c): ");
    Serial.println(f_temperature);

    //Serial.println(String(orientation.a) + ", " + String(orientation.b) + ", " + String(orientation.c) + ", " + String(orientation.d));

}