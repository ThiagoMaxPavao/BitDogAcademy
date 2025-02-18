#ifndef BUZZER_H
#define BUZZER_H

#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

#define BUZZER_VOLUME 0.5f

// Initialize the pin for pwm
void buzzer_init(uint pin);

// Make the buzzer beep with a given frequency and duration, blocking
void buzzer_beep(uint frequency, uint duration_ms);

// Make the buzzer beep with a given frequency and duration, non-blocking
void buzzer_start(uint frequency, uint duration_ms);

// Make a success sound, two beeps. Non-blocking
void success_sound();

// Make an error sound, one beep. Non-blocking
void error_sound();

#endif // BUZZER_H
