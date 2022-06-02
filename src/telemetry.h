#ifndef TELEMETRY_H
#define TELEMETRY_H

#include "state.h"

void telemetry_init(); // Should be called on power up.

void telemetry_send(system_state state); // Sends data over telemetry. Should be called every update.

void telemetry_receive(); // Receives commands from telemetry uplink. Should be called once every update.
void uplink_funcA(String);
void uplink_funcB(String);
void uplink_funcC(String);
void uplink_funcD(String);
void uplink_funcE(String);
void uplink_funcF(String);
void uplink_funcG(String);
void uplink_funcH(String);

#endif