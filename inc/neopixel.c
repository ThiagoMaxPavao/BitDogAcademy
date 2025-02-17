#include "neopixel.h"

static PIO np_pio;
static uint sm;
static npLED_t leds[LED_COUNT];

void ws2818b_program_init(PIO pio, uint sm, uint offset, uint pin, float freq) {

  pio_gpio_init(pio, pin);
  
  pio_sm_set_consecutive_pindirs(pio, sm, pin, 1, true);
  
  // Program configuration.
  pio_sm_config c = ws2818b_program_get_default_config(offset);
  sm_config_set_sideset_pins(&c, pin); // Uses sideset pins.
  sm_config_set_out_shift(&c, true, true, 8); // 8 bit transfers, right-shift.
  sm_config_set_fifo_join(&c, PIO_FIFO_JOIN_TX); // Use only TX FIFO.
  float prescaler = clock_get_hz(clk_sys) / (10.f * freq); // 10 cycles per transmission, freq is frequency of encoded bits.
  sm_config_set_clkdiv(&c, prescaler);
  
  pio_sm_init(pio, sm, offset, &c);
  pio_sm_set_enabled(pio, sm, true);
}

void npInit(uint pin) {
    np_pio = pio0;
    uint offset = pio_add_program(np_pio, &ws2818b_program);
    sm = pio_claim_unused_sm(np_pio, false);
    if (sm < 0) {
        np_pio = pio1;
        sm = pio_claim_unused_sm(np_pio, true);
    }
    ws2818b_program_init(np_pio, sm, offset, pin, 800000.f);
    
    npClear();
    npWrite();
}

void npSetLED(uint index, uint8_t r, uint8_t g, uint8_t b) {
    if (index < LED_COUNT) {
        leds[index].R = r;
        leds[index].G = g;
        leds[index].B = b;
    }
}

void npClear() {
    for (uint i = 0; i < LED_COUNT; ++i) {
        npSetLED(i, 0, 0, 0);
    }
}

uint8_t reverseBits(uint8_t byte) {
    byte = (byte & 0xF0) >> 4 | (byte & 0x0F) << 4;
    byte = (byte & 0xCC) >> 2 | (byte & 0x33) << 2;
    byte = (byte & 0xAA) >> 1 | (byte & 0x55) << 1;
    return byte;
}

void npWrite() {
    for (uint i = 0; i < LED_COUNT; ++i) {
        pio_sm_put_blocking(np_pio, sm, reverseBits(leds[i].G));
        pio_sm_put_blocking(np_pio, sm, reverseBits(leds[i].R));
        pio_sm_put_blocking(np_pio, sm, reverseBits(leds[i].B));
    }
    sleep_us(100);
}


int getIndex(int x, int y) {
    return (y % 2 != 0) ? (y * 5 + x) : (y * 5 + (4 - x));
}

void npSetLEDbyXY(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b) {
    int index = getIndex(x, y);
    npSetLED(index, r, g, b);
}
