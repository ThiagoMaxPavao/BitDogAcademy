#ifndef TESTS_H
#define TESTS_H

#include <stdio.h>
#include <stdint.h>

// Bibliotecas para os periféricos
#include "ssd1306.h"
#include "neopixel.h"
#include "buzzer.h"
#include "joystick.h"
#include "buttons.h"

void test_ssd1306(ssd1306_t *disp);
void test_np();
void test_buzzer();
void test_joystick();
void test_buttons();

#endif // TESTS_H
