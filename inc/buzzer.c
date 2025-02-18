#include "buzzer.h"
#include "pico/time.h"

static uint pin;
static alarm_id_t buzzer_alarm_id = -1;

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

// Stop the buzzer (called by alarm)
int64_t buzzer_stop(alarm_id_t id, void *user_data) {
    pwm_set_gpio_level(pin, 0);  // Turn off the buzzer
    buzzer_alarm_id = -1;        // Reset alarm ID
    return 0;  // Do not repeat the alarm
}

// Start a non-blocking beep
void buzzer_start(uint frequency, uint duration_ms) {
    uint slice_num = pwm_gpio_to_slice_num(pin);

    // Configure PWM settings
    pwm_config config = pwm_get_default_config();
    float clock_div = (float)clock_get_hz(clk_sys) / (frequency * 4096);
    pwm_config_set_clkdiv(&config, clock_div);

    pwm_init(slice_num, &config, true);
    pwm_set_gpio_level(pin, BUZZER_VOLUME * 2048); // Set duty cycle to 50% * BUZZER_VOLUME

    // Schedule alarm to stop the buzzer
    buzzer_alarm_id = add_alarm_in_ms(duration_ms, buzzer_stop, NULL, false);
}

int64_t success_single_beep(alarm_id_t id, void *user_data) {
    buzzer_start(8000, 100);
    return 0;
}

// Success beep sequence (non-blocking)
void success_sound() {
    success_single_beep(0, NULL);
    add_alarm_in_ms(150, success_single_beep, NULL, false);
}

// Error beep (single beep)
void error_sound() {
    buzzer_start(3000, 150);
}
