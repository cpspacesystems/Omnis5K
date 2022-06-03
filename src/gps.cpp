#include <Arduino.h>
#include <Adafruit_GPS.h>
#include "gps.h"

#define GPS_SERIAL Serial1

#define GPS_REFRESH_RATE 100

Adafruit_GPS gps(&GPS_SERIAL);

GpsData f_data;

uint32_t f_timer;

void gps_init() {
    gps.begin(9600);
    gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    gps.sendCommand(PMTK_SET_NMEA_UPDATE_10HZ);
    gps.sendCommand(PGCMD_ANTENNA);

    delay(1000);

    GPS_SERIAL.println(PMTK_Q_RELEASE);
    
    f_timer = millis();
}

void gps_update() {

    while (gps.available()){
        gps.read();
    }

    if (gps.newNMEAreceived()) {
        if (!gps.parse(gps.lastNMEA())) return;
    }

    f_data.altitude = gps.altitude;
    f_data.longitude = gps.longitudeDegrees * 100;
    f_data.latitude = gps.latitudeDegrees * 100;
    f_data.satellites = gps.satellites;

}

GpsData gps_getData() {
    return f_data;
}