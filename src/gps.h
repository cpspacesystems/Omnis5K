#ifndef GPS_H
#define GPS_H

typedef struct GpsData {
    float longitude, latitude, altitude;
    int satellites;
} GpsData;

void gps_init();
void gps_update();
GpsData gps_getData();

#endif