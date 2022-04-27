#include <Arduino.h>
#include "SPIMemory.h"
#include "error.h"

#define FLASH_PIN 0x0A

SPIFlash flash(FLASH_PIN);

void log_init() {
    Serial.println("INIT: Flash");
    flash.begin();
    //error_assert(flash.begin(), 1, FLASH_ERR_OFFSET, "flash.initialize()");
    Serial.println("done");
    //error_assert(flash.eraseChip(), true, FLASH_ERR_OFFSET, "flash.eraseChip()"); // Assuming for now that the chip should be wiped on power up, may be changed later for safety
    flash.eraseSector(0);
    flash.writeFloat(0, 1.12f);
}

void log_debugLog() {
    Serial.println(flash.getCapacity());
    Serial.println(flash.readFloat(0));
}