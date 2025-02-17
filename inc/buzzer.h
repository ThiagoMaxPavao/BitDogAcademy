#ifndef BUZZER_H
#define BUZZER_H

#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

// Initialize the pin for pwm
void buzzer_init(uint pin);

// Make the buzzer beep with a given frequency and duration
void buzzer_beep(uint frequency, uint duration_ms);

#endif // BUZZER_H
