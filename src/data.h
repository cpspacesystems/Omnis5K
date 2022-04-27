#ifndef DATA_H
#define DATA_H

void data_init();
void data_calibrate();
void data_update();

float data_accel();
float data_altitude();
float data_smoothAltitude();
float data_maxAltitude();

void data_valuesArray(float* array);

void logSensors();
void logCalibration();

#endif