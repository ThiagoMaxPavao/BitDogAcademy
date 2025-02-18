#include "state_machine.h"

// Pinagem/Configuração

// I2C display OLED
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15

// Neopixel - Matriz de LEDs RGB
#define LED_PIN 7

// Buzzer
#define BUZZER_PIN 21

// Joystick
#define JOY_X_PIN 26
#define JOY_Y_PIN 27

// Buttons
#define BUTTON_A 5
#define BUTTON_B 6
#define BUTTON_JOYSTICK 22

// Fonte para a tela inicial
#include "acme_5_outlines_font.h"
#include "bubblesstandard_font.h"
#include "BMSPA_font.h"

// Variáveis globais
ssd1306_t disp;
const char *subjects[] = {"Matematica", "Portugues", "Ciencias", "Geografia", "Historia"};
const uint n_subjects = 5;

const char *activities[] = {"Divisao inteira", "Poligonos", "Equacoes"};
const uint n_activities = 3;

// Variáveis estáticas
static int selected_subject = 0;
static int selected_activity = 0;

// Estado atual
static state_t current_state = STATE_INIT;

void state_machine_button_A_callback() {
    switch (current_state) {
        case STATE_USAGE_TUTORIAL_WAIT:
        current_state = STATE_START_SCREEN;
        success_sound();
        break;

        case STATE_SELECT_SUBJECT_WAIT:
        current_state = STATE_SHOW_USAGE_TUTORIAL;
        success_sound();
        break;

        case STATE_SELECT_ACTIVITY_WAIT:
        current_state = STATE_DRAW_SELECT_SUBJECT;
        success_sound();
        break;

        default:
        error_sound();
        break;
    }
}

void state_machine_button_B_callback() {
    switch (current_state) {
        case STATE_START_SCREEN_WAIT:
        current_state = STATE_SHOW_USAGE_TUTORIAL;
        success_sound();
        break;

        case STATE_USAGE_TUTORIAL_WAIT:
        current_state = STATE_DRAW_SELECT_SUBJECT;
        success_sound();
        break;

        case STATE_SELECT_SUBJECT_WAIT:
        if(selected_subject == 0) {
            current_state = STATE_DRAW_SELECT_ACTIVITY;
            success_sound();
        }
        else {
            error_sound();
        }
        break;

        default:
        error_sound();
        break;
    }
}

void state_machine_button_joystick_callback() {
    switch (current_state) {
        default:
        error_sound();
        break;
    }
}

void state_init() {
    // Inicializa os periféricos
    disp.external_vcc = false;
    ssd1306_init(&disp, 128, 64, 0x3C, I2C_PORT, I2C_SDA, I2C_SCL); // display OLED

    np_init(LED_PIN); // Matriz de LEDs Neopixel

    buzzer_init(BUZZER_PIN); // Buzzer

    joystick_init(JOY_X_PIN, JOY_Y_PIN); // Joystick

    buttons_init(BUTTON_A, BUTTON_B, BUTTON_JOYSTICK); // Botões

    // Configura callbacks dos botões
    set_button_A_callback(state_machine_button_A_callback);
    set_button_B_callback(state_machine_button_B_callback);
    set_button_joystick_callback(state_machine_button_joystick_callback);

    current_state = STATE_START_SCREEN;
}

void state_run_tests() {
    test_ssd1306(&disp);
    test_np();
    test_buzzer();
    test_joystick();
    test_buttons();
}

void state_start_screen() {
    ssd1306_clear(&disp);
    ssd1306_draw_string_with_font(&disp, 16, 2, 2, BMSPA_font, "BITDOG");
    ssd1306_draw_string_with_font(&disp, 8, 22, 2, BMSPA_font, "ACADEMY");
    ssd1306_draw_string_by_center(&disp, disp.width/2, 48, 1, "Pressione o botao B");
    ssd1306_draw_string_by_center(&disp, disp.width/2, 58, 1, "para iniciar");
    ssd1306_show(&disp);

    start_song();

    state_start_screen_wait(true); // reset animation

    current_state = STATE_START_SCREEN_WAIT;
}

void state_start_screen_wait(bool reset) {
    static uint a = 0;
    static float l = 0;

    if(reset) {
        a = l = 0;
        return;
    }

    for(uint x = 0; x < 5; x++) {
        for(uint y = 0; y < 5; y++) {
            int shift = 25*(x-y);
            float adjusted_l = l - (y-x+4)*0.01;
            np_set_led_hsl_xy(x, y, ((a+shift) % 360) / 360.0, 1, clampf(adjusted_l, 0, 0.5));
        }
    }

    np_write();

    a += 10;
    l += 0.01;

    sleep_ms(60);
}

void state_show_usage_tutorial() {
    np_clear();
    np_write();

    ssd1306_clear(&disp);
    ssd1306_draw_string_by_center(&disp, disp.width/2, 10, 2, "Instrucoes");
    ssd1306_draw_string(&disp, 2, 25, 1, "Botao A para voltar");
    ssd1306_draw_string(&disp, 2, 35, 1, "Botao B para avancar");
    ssd1306_draw_string(&disp, 2, 47, 1, "Joystick para");
    ssd1306_draw_string(&disp, 2, 56, 1, "selecionar opcoes");
    ssd1306_show(&disp);

    current_state = STATE_USAGE_TUTORIAL_WAIT;
}

void state_draw_select_subject() {
    ssd1306_clear(&disp);
    ssd1306_draw_string_by_center(&disp, disp.width/2, 4, 1, "Selecione a");
    ssd1306_draw_string_by_center(&disp, disp.width/2, 13, 1, "Disciplina");

    int previous = selected_subject-1;
    int next = selected_subject+1;

    if(previous < 0) previous = n_subjects-1;
    if(next == n_subjects) next = 0;

    ssd1306_draw_string(&disp, 16, 32, 1, subjects[previous]);
    ssd1306_draw_char(&disp, 8, 42, 1, '>');
    ssd1306_draw_string(&disp, 16, 42, 1, subjects[selected_subject]);
    ssd1306_draw_string(&disp, 16, 52, 1, subjects[next]);

    ssd1306_show(&disp);

    current_state = STATE_SELECT_SUBJECT_WAIT;
}

void state_select_subject_wait() {
    static bool joystick_extended = false;
    static bool last_joystick_extended = false;
    float r, angle;

    joystick_get_RA(&r, &angle);

    if(r > 0.9) joystick_extended = true;
    else if(r < 0.8) joystick_extended = false;

    if(joystick_extended && !last_joystick_extended) {
        if(angle_difference(angle, M_PI_2) < M_PI_4) {
            selected_subject--;
            if(selected_subject < 0) selected_subject = n_subjects-1;
            current_state = STATE_DRAW_SELECT_SUBJECT;
            simple_success_sound();
        } 
        else if(angle_difference(angle, 3*M_PI_2) < M_PI_4){
            selected_subject++;
            if(selected_subject == n_subjects) selected_subject = 0;
            current_state = STATE_DRAW_SELECT_SUBJECT;
            simple_success_sound();
        }
        else {
            error_sound();
        }
    }

    last_joystick_extended = joystick_extended;

    sleep_ms(20);
}

void state_draw_select_activity() {
    ssd1306_clear(&disp);
    ssd1306_draw_string_by_center(&disp, disp.width/2, 4, 1, "Selecione a");
    ssd1306_draw_string_by_center(&disp, disp.width/2, 13, 1, "Atividade");

    int previous = selected_activity-1;
    int next = selected_activity+1;

    if(previous < 0) previous = n_activities-1;
    if(next == n_activities) next = 0;

    ssd1306_draw_string(&disp, 16, 32, 1, activities[previous]);
    ssd1306_draw_char(&disp, 8, 42, 1, '>');
    ssd1306_draw_string(&disp, 16, 42, 1, activities[selected_activity]);
    ssd1306_draw_string(&disp, 16, 52, 1, activities[next]);

    ssd1306_show(&disp);

    current_state = STATE_SELECT_ACTIVITY_WAIT;
}

void state_select_activity_wait() {
    static bool joystick_extended = false;
    static bool last_joystick_extended = false;
    float r, angle;

    joystick_get_RA(&r, &angle);

    if(r > 0.9) joystick_extended = true;
    else if(r < 0.8) joystick_extended = false;

    if(joystick_extended && !last_joystick_extended) {
        if(angle_difference(angle, M_PI_2) < M_PI_4) {
            selected_activity--;
            if(selected_activity < 0) selected_activity = n_activities-1;
            current_state = STATE_DRAW_SELECT_ACTIVITY;
            simple_success_sound();
        } 
        else if(angle_difference(angle, 3*M_PI_2) < M_PI_4){
            selected_activity++;
            if(selected_activity == n_activities) selected_activity = 0;
            current_state = STATE_DRAW_SELECT_ACTIVITY;
            simple_success_sound();
        }
        else {
            error_sound();
        }
    }

    last_joystick_extended = joystick_extended;

    sleep_ms(20);
}

void run_state_machine() {
    switch (current_state) {
        case STATE_INIT:
        state_init();
        break;

        case STATE_RUN_TESTS:
        state_run_tests();
        break;

        case STATE_START_SCREEN:
        state_start_screen();
        break;

        case STATE_START_SCREEN_WAIT:
        state_start_screen_wait(false);
        break;

        case STATE_SHOW_USAGE_TUTORIAL:
        state_show_usage_tutorial();
        break;

        case STATE_DRAW_SELECT_SUBJECT:
        state_draw_select_subject();
        break;

        case STATE_SELECT_SUBJECT_WAIT:
        state_select_subject_wait();
        break;

        case STATE_DRAW_SELECT_ACTIVITY:
        state_draw_select_activity();
        break;

        case STATE_SELECT_ACTIVITY_WAIT:
        state_select_activity_wait();
        break;

        default:
        break;
    }
}
