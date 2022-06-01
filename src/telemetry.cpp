#include <Arduino.h>
#include "data.h"
#include "telemetry.h"
#define TEL Serial5
#define BAUD 57600

void telemetry_init() {
    Serial.println("INIT: Telemetry");
    TEL.begin(BAUD);
}

void telemetry_send() {

    float arr[DATA_FRAME_SIZE];
    data_valuesArray(arr);

    for (int i = 0; i < DATA_FRAME_SIZE - 1; i++) {
        TEL.print(arr[i]);
        TEL.print(",");
    }
    TEL.print(arr[DATA_FRAME_SIZE - 1]);

    TEL.println();
}

void telemetry_receive() {
    String msg;
    while( TEL.available() > 0 ){
        msg = TEL.readString();
        // Running corresponding functions
        if(msg.startsWith("funcA"))
            uplink_funcA(msg);

        else if(msg.startsWith("funcB"))
            uplink_funcB(msg);

        else if(msg.startsWith("funcC"))
            uplink_funcC(msg);

        else if(msg.startsWith("funcD"))
            uplink_funcD(msg);
        else{
            Serial.println("Command not recognized:");
            Serial.println(msg);
        }
    }
}

void uplink_funcA(String input){
    Serial.println("Running funcA");
    Serial.print("Received ");
    Serial.println(input);
}

void uplink_funcB(String input){
    Serial.println("Running funcB");
}

void uplink_funcC(String input){
    Serial.println("Running funcC");
}

void uplink_funcD(String input){
    Serial.println("Running funcD");
}