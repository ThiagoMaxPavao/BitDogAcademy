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

// Funcoes de teste dos periféricos
#include "tests.h"

// Funcoes da atividade
#include "math_integer_division.h"


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

// Variáveis globais
ssd1306_t disp;

int main() {
    stdio_init_all();
    // sleep_ms(5000);

    // Initialization
    disp.external_vcc = false;
    ssd1306_init(&disp, 128, 64, 0x3C, I2C_PORT, I2C_SDA, I2C_SCL);

    np_init(LED_PIN);

    buzzer_init(BUZZER_PIN);

    joystick_init(JOY_X_PIN, JOY_Y_PIN);

    buttons_init(BUTTON_A, BUTTON_B, BUTTON_JOYSTICK);

    // Tests
    // test_ssd1306(&disp);
    // test_np();
    // test_buzzer();
    // test_joystick();
    // test_buttons();

    math_integer_division_activity_setup();

    while(true) {
        math_integer_division_activity_loop();
    }

    return 0;
}
