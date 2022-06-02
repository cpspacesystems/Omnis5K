#include <string.h>
#include <Arduino.h>
#include <SD.h>
#include "SPIMemory.h"
#include "data.h"
#include "error.h"

#define FLASH_PIN 0xA
#define FLASH_CAPACITY 0x1000000
#define SD_PIN BUILTIN_SDCARD
#define CSV_DELIMITER ','

uint32_t f_startAddr;
uint32_t f_endAddr;
uint32_t f_frameIndex;

File logFile;

SPIFlash flash(FLASH_PIN);

void log_init() {

    Serial.println("INIT: Flash");

    error_assert(flash.begin(), true, FLASH_ERR_OFFSET, "flash.begin()");
    error_assert(flash.eraseChip(), true, FLASH_ERR_OFFSET, "flash.eraseChip()"); // Probably should change this to be more dynamic

    Serial.println("INIT: SD");

    error_assert(SD.begin(SD_PIN), true, SD_ERR_OFFSET, "SD.begin()");

    f_startAddr = 0;
    f_endAddr = 0;
    f_frameIndex = 0;
}

void log_logFrame(uint8_t state) {

    float frame[DATA_FRAME_SIZE];
    data_valuesArray(frame);

    if (f_endAddr > FLASH_CAPACITY - 0x100) {
        return;
    }

    flash.writeULong(f_endAddr, f_frameIndex);
    f_endAddr += 4;

    flash.writeByte(f_endAddr, state);
    f_endAddr += 1;

    for (uint16_t i = 0; i < DATA_FRAME_SIZE; i++) {
        flash.writeFloat(f_endAddr, frame[i]);
        f_endAddr += 4;
    }

    f_frameIndex++;
    
}

void log_dumpToSD() {

    // Search for a unique file name on the SD card

    int fileNum = 0;
    char name[11];
    char num[3];
    while (fileNum < 100) {
        name[0] = 0;
        num[0] = 0;
        itoa(fileNum, num, 10);
        strcat(name, "log_");
        strcat(name, num);
        strcat(name, ".csv");
        Serial.print("Tried file name ");
        Serial.println(name);
        if (!SD.exists(name)) break;
        fileNum++;
    }

    // Open file and log column labels
    logFile = SD.open(name, FILE_WRITE);
    logFile.println("frame,state,pressure,ASL,AGL,smoothAGL,temp,accelX,accelY,accelZ,gyroX,gyroY,gyroZ,velocityX");

    uint32_t dataAddr = f_startAddr;
    
    for (uint16_t i = 0; i < f_frameIndex; i += 1) {

        logFile.print(flash.readULong(dataAddr));
        logFile.print(CSV_DELIMITER);
        dataAddr += 4;

        logFile.print(flash.readByte(dataAddr));
        logFile.print(CSV_DELIMITER);
        dataAddr += 1;

        for (uint16_t j = 0; j < DATA_FRAME_SIZE; j++) {
            logFile.print(flash.readFloat(dataAddr));
            logFile.print(CSV_DELIMITER);
            dataAddr += 4;
        }
        logFile.println();

    }

    logFile.close();

}

void log_debugLog() {

}