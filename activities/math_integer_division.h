#ifndef MATH_INTEGER_DIVISION_ACTIVITY_H
#define MATH_INTEGER_DIVISION_ACTIVITY_H

#include "ssd1306.h"
#include <math.h>
#include "util.h"

// Bibliotecas para os periféricos
#include "ssd1306.h"
#include "neopixel.h"
#include "buzzer.h"
#include "joystick.h"
#include "buttons.h"

// Fonte para o título de introdução da atividade
#include "acme_5_outlines_font.h"

void math_integer_division_activity_setup();
void math_integer_division_activity_loop();
int math_integer_division_draw_tutorial_page(int page_number);

#endif // MATH_INTEGER_DIVISION_ACTIVITY_H
