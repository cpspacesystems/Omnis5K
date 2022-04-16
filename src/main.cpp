#include <Arduino.h>

/* Enum responsible for representing the states of flight:

  nav_converge:   calibration of the GPS, IMU, and Barometer
  pad_idle:       waiting for rocket burn, begin position tracking
  burn:           begin data logging
  coast:          dump data if necessary TODO: determine if needed due to space limitations
  post_apogee:    deploy water from nose cone
  chute_deployed: dump data if necessary TODO: determine if needed due to space limitations
  grounded:       end data logging, end position tracking, dump all logged data onto the SD card

*/

enum system_state {
  nav_converge,
  pad_idle,
  burn,
  coast,
  post_apogee,
  chute_deployed,
  grounded
} state;



void setup() {
  state = nav_converge;
}

void loop() {
  
  switch (state) {

    case nav_converge:
      break;

    case pad_idle:
      break;

    case burn:
      break;
    
    case coast:
      break;

    case post_apogee:
      break;

    case chute_deployed:
      break;

    case grounded:
      break;

  }

}