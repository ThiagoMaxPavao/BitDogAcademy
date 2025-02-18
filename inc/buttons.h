// buttons.h
#ifndef BUTTONS_H
#define BUTTONS_H

#include <stdint.h>
#include "pico/stdlib.h"

#define DEBOUNCE_TIME 100

typedef void (*button_callback_t)(void);

void buttons_init(int button_A_pin_, int button_B_pin_, int button_joystick_pin_);
void set_button_A_callback(button_callback_t callback);
void set_button_B_callback(button_callback_t callback);
void set_button_joystick_callback(button_callback_t callback);

#endif // BUTTONS_H
