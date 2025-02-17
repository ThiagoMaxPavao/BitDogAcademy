#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "pico/stdlib.h"
#include "hardware/adc.h"
#include <math.h>

// Function to initialize the joystick on specified X and Y pins
void joystick_init(uint X_pin, uint Y_pin);

// Function to get X and Y values (-1 to 1 range)
void joystick_get_XY(float *x, float *y);

// Function to get radial distance (r) and angle (θ in radians)
void joystick_get_RA(float *r, float *angle);

#endif // JOYSTICK_H
