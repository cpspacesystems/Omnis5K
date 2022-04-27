#ifndef ERROR_H
#define ERROR_H

#include <Arduino.h>

#define ACCEL_ERR_OFFSET 0
#define GYRO_ERR_OFFSET 1
#define BARO_ERR_OFFSET 2
#define FLASH_ERR_OFFSET 3

void error_assert(int val, int no_err_val, int err_offset, const char* err_message);

bool error_ok();

uint8_t error_status();

#endif