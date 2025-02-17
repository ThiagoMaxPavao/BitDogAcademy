#include "buzzer.h"

static uint pin;

void buzzer_init(uint gpio_pin) {
    pin = gpio_pin;
    gpio_set_function(pin, GPIO_FUNC_PWM); // Set pin as PWM output
}

void buzzer_beep(uint frequency, uint duration_ms) {
    uint slice_num = pwm_gpio_to_slice_num(pin); // Get PWM slice for the pin

    // Configure PWM settings
    pwm_config config = pwm_get_default_config();
    float clock_div = (float)clock_get_hz(clk_sys) / (frequency * 4096);
    pwm_config_set_clkdiv(&config, clock_div);
    
    pwm_init(slice_num, &config, true); // Initialize PWM with config
    pwm_set_gpio_level(pin, BUZZER_VOLUME * 2048); // Set duty cycle to 50% * BUZZER_VOLUME

    sleep_ms(duration_ms); // Wait for duration

    pwm_set_gpio_level(pin, 0); // Turn off the buzzer
}
