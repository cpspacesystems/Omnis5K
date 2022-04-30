#ifndef LOG_H
#define LOG_H

void log_init(); // Should be called on power up
void log_logFrame(uint8_t state); // Takes data from data.cpp and logs it to the flash chip
void log_dumpToSD(); // Takes all frames stored in the flash chip and stores them to a csv file in the SD card

void log_debugLog();

#endif