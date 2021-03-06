#ifndef DATA_H
#define DATA_H

#define DATA_FRAME_SIZE 18

void data_init(); // Should be called on power up
bool data_calibrate(); // Calibrates sensors and measures ground pressure, returns number of calibrations
void data_update(); // Records data from all sensors and does math on it

float data_accel();
float data_altitude();
float data_smoothAltitude();
float data_maxAltitude(); // Returns the max (smooth) altitude recorded so far
float data_velocityX();

void data_valuesArray(float* array); // Sets fills array with all data. array should have DATA_FRAME_SIZE space

void logSensors(); // used for debugging
void logCalibration();

#endif