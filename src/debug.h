#ifndef DEBUG_H
#define DEBUG_H

#define DEBUG_SERIAL

void debug_begin();
void debug_print(const char *message);
void debug_print(float value);
void debug_println(const char *message);
void debug_println(float value);

#endif