#include <Arduino.h>

int start_millis;

int f_elapsed;
float f_result;
float f_a;
float f_b;

int i_elapsed;
int i_result;
int i_a;
int i_b;

void run_test() {
    start_millis = micros();

    f_a = 4.18173488349f;
    f_b = 1.18374128934f;
    f_result = f_a + f_b;

    for (int i = 0; i < 100; i++) {
        f_result = f_a * f_result;
        f_result = f_b / f_result;
        f_result = sqrt(f_result);
    }
    f_elapsed = micros() - start_millis;

    start_millis = micros();

    i_a = 424214589;
    i_b = 132432454;
    i_result = i_a + i_b;

    for (int i = 0; i < 100; i++) {
        i_result = i_a * i_result;
        i_result = i_b / i_result;
        i_result = sqrt(i_result);
    }
    i_elapsed = micros() - start_millis;

    Serial.println("Float time(micros): " + String(f_elapsed) + "\t Int time(micros): " + String(i_elapsed));
}