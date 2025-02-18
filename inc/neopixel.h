#ifndef NEOPIXEL_H
#define NEOPIXEL_H

#include <stdio.h>
#include <stdint.h>
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2818b.pio.h"
#include <math.h>
#include "pico/stdlib.h"

#define LED_COUNT 25
#define MAX_BRIGHTNESS 30

// RGB values range from 0 to 255
// HSL values: Hue (0.0 - 1.0), Saturation (0.0 - 1.0), Lightness (0.0 - 1.0)

typedef struct {
    uint8_t G, R, B;
} npLED_t;

void np_init(uint pin);
void np_clear();
void np_write();

void np_set_led_rgb_index(uint index, uint8_t r, uint8_t g, uint8_t b);
void np_set_led_rgb_xy(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b);

void np_set_led_hsl_index(uint index, double h, double s, double l);
void np_set_led_hsl_xy(uint8_t x, uint8_t y, double h, double s, double l);

#endif // NEOPIXEL_H