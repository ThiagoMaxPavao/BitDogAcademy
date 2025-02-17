// Biblioteca padrão e da SDK
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// Biblioteca para o display OLED e fontes
#include "inc/ssd1306.h"
#include "inc/acme_5_outlines_font.h"
#include "inc/bubblesstandard_font.h"
#include "inc/crackers_font.h"
#include "inc/BMSPA_font.h"

// Biblioteca para o Neopixel - Matrix de LEDs RGB
#include "inc/neopixel.h"

// Pinagem/Configuração

// I2C display OLED
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15

// Neopixel - Matriz de LEDs RGB
#define LED_PIN 7

void display_init(ssd1306_t *disp) {
    disp->external_vcc = false;
    ssd1306_init(disp, 128, 64, 0x3C, I2C_PORT);
    ssd1306_clear(disp);
    ssd1306_show(disp);
}

void setup_pins() {
    // display OLED
    i2c_init(I2C_PORT, 400000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
}

void test_ssd1306(ssd1306_t *disp) {
    const uint8_t *fonts[4]= {acme_font, bubblesstandard_font, crackers_font, BMSPA_font};
    char buffer[20]; // Buffer para a string formatada

    for(int i = 0; i < 5; i++) {
        ssd1306_clear(disp);

        sprintf(buffer, "%d:Teste", i);

        if(i == 0)
            ssd1306_draw_string(disp, 8, 24, 2, buffer);
        else
            ssd1306_draw_string_with_font(disp, 8, 24, 2, fonts[i-1], buffer);

        ssd1306_show(disp);
        sleep_ms(2000);
    }

    ssd1306_clear(disp);
    ssd1306_draw_string(disp, 8, 24, 2, "FIM");
    ssd1306_show(disp);
}

void test_np() {
    for(int i = 0; i < 3; i++) {
        for(int brilho = 0; brilho < 30; brilho++) {
            npSetLEDbyXY(0, 0, brilho, 0, 0);
            npSetLEDbyXY(2, 2, 0, brilho, 0);
            npSetLEDbyXY(4, 4, 0, 0, brilho);
            npWrite();
            sleep_ms(25);
        }

        for(int brilho = 30; brilho >= 0; brilho--) {
            npSetLEDbyXY(0, 0, brilho, 0, 0);
            npSetLEDbyXY(2, 2, 0, brilho, 0);
            npSetLEDbyXY(4, 4, 0, 0, brilho);
            npWrite();
            sleep_ms(25);
        }
    }
}

int main() {
    stdio_init_all();

    setup_pins();

    ssd1306_t disp;
    display_init(&disp);
    test_ssd1306(&disp);

    npInit(LED_PIN);
    npClear();
    npWrite();
    test_np();

    return 0;
}
