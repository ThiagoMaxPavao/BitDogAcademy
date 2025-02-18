#include "math_integer_division.h"

#define SELECT_DIVIDEND 0
#define SELECT_DIVISOR 1

extern ssd1306_t disp;

static int dividend = 9;
static int divisor = 9;
static int quotient;
static int rest;

static bool joystick_extended = false;
static bool last_joystick_extended = false;

static int selected = SELECT_DIVIDEND;

static bool draw_selected = true;
static bool grouping_mode = false; // groups by the divisor or quotient

static bool redraw = true;

static float r, angle;

static struct repeating_timer switch_draw_selected_and_redraw_timer;

bool switch_draw_selected_and_redraw_callback(struct repeating_timer *t) {
    draw_selected = !draw_selected;
    redraw = true;
    return true;
}

// false = dividend, true = divisor
void draw_division_ssd1306(ssd1306_t *disp, int dividend, int divisor, int quotient, int rest, bool dividend_selected, bool divisor_selected) {
    char buffer[3]; // Buffer para a strings formatadas

    ssd1306_clear(disp);

    // Dividendo
    sprintf(buffer, "%2d", dividend);
    ssd1306_draw_string_by_center(disp, disp->width/3, 15, 2, buffer);
    if(dividend_selected) {
        ssd1306_draw_line(disp, disp->width/3-13, 23, disp->width/3+10, 23);
    }

    // Divisor
    sprintf(buffer, "%2d", divisor);
    ssd1306_draw_string_by_center(disp, 2*disp->width/3, 15, 2, buffer);
    if(divisor_selected) {
        ssd1306_draw_line(disp, 2*disp->width/3-13, 23, 2*disp->width/3+10, 23);
    }

    // Quociente
    sprintf(buffer, "%2d", quotient);
    ssd1306_draw_string_by_center(disp, 2*disp->width/3, 45, 2, buffer);

    // Resto
    sprintf(buffer, "%2d", rest);
    ssd1306_draw_string_by_center(disp, disp->width/3, 45, 2, buffer);

    // L da divisão - linha vertical
    ssd1306_draw_line(disp, disp->width/2+1, 5, disp->width/2+1, 27);
    ssd1306_draw_line(disp, disp->width/2+2, 5, disp->width/2+2, 27);
    
    // Linha horizontal
    ssd1306_draw_line(disp, disp->width/2+1, 26, 110, 26);
    ssd1306_draw_line(disp, disp->width/2+2, 27, 110, 27);

    ssd1306_show(disp);
}

void draw_division_np(int dividend, int divisor, int quotient, int rest) {
    int count = 0;

    np_clear();

    uint8_t group_colors[] = {
        255, 0, 0, // Vermelho
        0, 255, 0, // Verde
        0, 0, 255, // Azul
        255, 255, 0, // Amarelo
        255, 0, 255, // Magenta
        0, 255, 255, // Ciano
        255, 127, 0, // Laranja
    };

    int n_colors = sizeof(group_colors) / 3;

    for(int index = 24; index >= 0; index--) {
        if(count >= dividend) break; // end

        if(count < dividend - rest) {
            int color_step = count / quotient;
            int color_i = color_step % n_colors;
            np_set_led_rgb_index(index, group_colors[3*color_i],
                                        group_colors[3*color_i+1],
                                        group_colors[3*color_i+2]);
        } else {
            np_set_led_rgb_index(index, 255, 255, 255);
        }

        count++;
    }
    
    np_write();
}

void math_integer_division_button_A_callback() { }

void math_integer_division_button_B_callback() { }

void math_integer_division_button_joystick_callback() {
    grouping_mode = !grouping_mode;
    redraw = true;
}

void math_integer_division_activity_setup() {
    quotient = dividend / divisor;
    rest = dividend % divisor;

    draw_division_ssd1306(&disp, dividend, divisor, quotient, rest, false, false);
    draw_division_np(dividend, divisor, quotient, rest);

    set_button_joystick_callback(math_integer_division_button_joystick_callback);

    add_repeating_timer_ms(500, switch_draw_selected_and_redraw_callback, NULL, &switch_draw_selected_and_redraw_timer);
}

void math_integer_division_activity_loop() {
    joystick_get_RA(&r, &angle);

    if(r > 0.9) joystick_extended = true;
    else if(r < 0.8) joystick_extended = false;

    if(joystick_extended && !last_joystick_extended) {

        if(angle_difference(angle, 0) < M_PI/4) {
            if(selected == SELECT_DIVIDEND) {
                selected = SELECT_DIVISOR;
                draw_selected = true;
                success_sound();
            } else {
                error_sound();
            }
        } 
        else if(angle_difference(angle, M_PI) < M_PI_4) {
            if(selected == SELECT_DIVISOR) {
                selected = SELECT_DIVIDEND;
                draw_selected = true;
                success_sound();
            } else {
                error_sound();
            }
        } 
        else if(angle_difference(angle, M_PI_2) < M_PI_4) {
            if(selected) {
                divisor++;
            } else {
                dividend++;
            }
            success_sound();
        } 
        else {
            if(selected) {
                divisor--;
            } else {
                dividend--;
            }
            success_sound();
        }

        divisor = clamp(divisor, 1, 25);
        dividend = clamp(dividend, 0, 25);

        quotient = dividend / divisor;
        rest = dividend % divisor;

        redraw = true;
    }

    if(redraw) {
        draw_division_ssd1306(&disp, dividend, divisor, quotient, rest, draw_selected && !selected, draw_selected && selected);
        draw_division_np(dividend, grouping_mode ? divisor : quotient, grouping_mode ? quotient : divisor, rest);
    }
    
    last_joystick_extended = joystick_extended;

    sleep_ms(20);
}
