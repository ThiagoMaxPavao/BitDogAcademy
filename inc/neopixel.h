#ifndef NEOPIXEL_H
#define NEOPIXEL_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2818b.pio.h"

#define LED_COUNT 25

typedef struct {
    uint8_t G, R, B;
} npLED_t;

void npInit(uint pin);
void npSetLED(uint index, uint8_t r, uint8_t g, uint8_t b);
void npSetLEDbyXY(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b);
void npClear();
void npWrite();
int getIndex(int x, int y);

#endif // NEOPIXEL_H
