#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

// Biblioteca padrão e da SDK
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include <math.h>

// Biblioteca para o display OLED
#include "ssd1306.h"

// Biblioteca para o Neopixel - Matrix de LEDs RGB
#include "neopixel.h"

// Biblioteca para o Buzzer
#include "buzzer.h"

// Biblioteca para o Joystick
#include "joystick.h"

// Biblioteca para os botões
#include "buttons.h"

// Funcoes da atividade
#include "tests.h"

// Funcoes da atividade de matemática - divisão inteira
#include "math_integer_division.h"

typedef enum {
    STATE_INIT,
    STATE_RUN_TESTS,
    STATE_START_SCREEN,
    STATE_START_SCREEN_WAIT_LOOP,

} state_t;

void run_state_machine();

#endif // STATE_MACHINE_H
