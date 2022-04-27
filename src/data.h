#ifndef DATA_H
#define DATA_H

void data_err(int val, int no_err_val, int err_offset, String err_message);

void data_init();
void data_calibrate();
void data_update();

float data_accel();
float data_altitude();
float data_smoothAltitude();
float data_maxAltitude();

void logSensors();
void logCalibration();

#endif