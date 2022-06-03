#include <Arduino.h>
#include "debug.h"
#include "data.h"
#include "telemetry.h"
#define TEL Serial5
#define BAUD 57600

void telemetry_init() {
    debug_println("INIT: Telemetry");
    TEL.begin(BAUD);
}

void telemetry_send(uint8_t state) {

    float arr[DATA_FRAME_SIZE];
    data_valuesArray(arr);

    for (int i = 0; i < DATA_FRAME_SIZE; i++) {
        TEL.print(arr[i]);
        TEL.print(",");
    }

    TEL.print((float)state);

    TEL.println();
}

void telemetry_receive() {
    String msg;
    while( TEL.available() > 0 ){
        msg = TEL.readString();
        // Running corresponding functions
        if(msg.startsWith("ACTION_A_ABC"))
            uplink_funcA(msg);

        else if(msg.startsWith("ACTION_B_DEF"))
            uplink_funcB(msg);

        else if(msg.startsWith("ACTION_C_GHI"))
            uplink_funcC(msg);

        else if(msg.startsWith("ACTION_D_JKL"))
            uplink_funcD(msg);

        else if(msg.startsWith("ACTION_E_MNO"))
            uplink_funcE(msg);

        else if(msg.startsWith("ACTION_F_PQR"))
            uplink_funcF(msg);

        else if(msg.startsWith("ACTION_G_STU"))
            uplink_funcG(msg);

        else if(msg.startsWith("ACTION_H_VWX"))
            uplink_funcH(msg);

        else{
            debug_println("Command not recognized:");
            debug_println(msg.c_str());
        }
    }
}

void uplink_funcA(String input){
    debug_println("Running funcA");
    debug_print("Received ");
    debug_println(input.c_str());
}

void uplink_funcB(String input){
    debug_println("Running funcB");
}

void uplink_funcC(String input){
    debug_println("Running funcC");
}

void uplink_funcD(String input){
    debug_println("Running funcD");
}

void uplink_funcE(String input){
    debug_println("Running funcB");
}

void uplink_funcF(String input){
    debug_println("Running funcC");
}

void uplink_funcG(String input){
    debug_println("Running funcD");
}

void uplink_funcH(String input){
    debug_println("Running funcD");
}