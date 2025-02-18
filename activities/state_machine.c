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

// Estado atual
static state_t current_state = STATE_INIT;

void state_init() {
    disp.external_vcc = false;
    ssd1306_init(&disp, 128, 64, 0x3C, I2C_PORT, I2C_SDA, I2C_SCL);

    np_init(LED_PIN);

    buzzer_init(BUZZER_PIN);

    joystick_init(JOY_X_PIN, JOY_Y_PIN);

    buttons_init(BUTTON_A, BUTTON_B, BUTTON_JOYSTICK);

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

    current_state = STATE_START_SCREEN_WAIT_LOOP;
}

void state_start_screen_wait_loop() {
    static uint a = 0;
    static float l = 0;

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

        case STATE_START_SCREEN_WAIT_LOOP:
        state_start_screen_wait_loop();
        break;

        default:
        break;
    }
}
