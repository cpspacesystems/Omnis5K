#ifndef ERROR_H
#define ERROR_H

#include <Arduino.h>

/*
    Errors in systems are handled by a status value which is an integer.
    Each bit in the integer represents a system which when set high means
    an error has occurred within that system.
*/

// These offsets represent the bit in the error value for each system

#define ACCEL_ERR_OFFSET 0
#define GYRO_ERR_OFFSET 1
#define BARO_ERR_OFFSET 2
#define FLASH_ERR_OFFSET 3
#define SD_ERR_OFFSET 4

void error_assert(int val, int no_err_val, int err_offset, const char* err_message); // Prints the error message and sets error status bit accordingly

bool error_ok(); // Returns true if no errors present (error status is 0)

uint8_t error_status(); // Returns the error status value

#endif