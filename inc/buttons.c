// buttons.c
#include "buttons.h"

volatile int64_t button_A_main_edge_time = 0;
volatile int64_t button_B_main_edge_time = 0;
volatile int64_t button_joystick_main_edge_time = 0;

static int button_A_pin;
static int button_B_pin;
static int button_joystick_pin;

static button_callback_t button_A_callback = NULL;
static button_callback_t button_B_callback = NULL;
static button_callback_t button_joystick_callback = NULL;

void set_button_A_callback(button_callback_t callback) {
    button_A_callback = callback;
}

void set_button_B_callback(button_callback_t callback) {
    button_B_callback = callback;
}

void set_button_joystick_callback(button_callback_t callback) {
    button_joystick_callback = callback;
}

void process_button_A() {
    if(button_A_callback) {
        button_A_callback();
    }
}

void process_button_B() {
    if(button_B_callback) {
        button_B_callback();
    }
}

void process_button_joystick() {
    if(button_joystick_callback) {
        button_joystick_callback();
    }
}

void gpio_callback(uint gpio, uint32_t events) {
    int64_t now = to_ms_since_boot(get_absolute_time());

    if(gpio == button_A_pin) {
        if(events & GPIO_IRQ_EDGE_FALL && !(events & GPIO_IRQ_EDGE_RISE) && now - button_A_main_edge_time > DEBOUNCE_TIME) {
            process_button_A();
        }
        button_A_main_edge_time = now;
    }
    else if(gpio == button_B_pin) {
        if(events & GPIO_IRQ_EDGE_FALL && !(events & GPIO_IRQ_EDGE_RISE) && now - button_B_main_edge_time > DEBOUNCE_TIME) {
            process_button_B();
        }
        button_B_main_edge_time = now;
    }
    else if(gpio == button_joystick_pin) {
        if(events & GPIO_IRQ_EDGE_FALL && !(events & GPIO_IRQ_EDGE_RISE) && now - button_joystick_main_edge_time > DEBOUNCE_TIME) {
            process_button_joystick();
        }
        button_joystick_main_edge_time = now;
    }
}

void buttons_init(int button_A_pin_, int button_B_pin_, int button_joystick_pin_) {
    button_A_pin = button_A_pin_;
    button_B_pin = button_B_pin_;
    button_joystick_pin = button_joystick_pin_;

    gpio_init(button_A_pin);
    gpio_set_dir(button_A_pin, GPIO_IN);
    gpio_pull_up(button_A_pin);

    gpio_init(button_B_pin);
    gpio_set_dir(button_B_pin, GPIO_IN);
    gpio_pull_up(button_B_pin);

    gpio_init(button_joystick_pin);
    gpio_set_dir(button_joystick_pin, GPIO_IN);
    gpio_pull_up(button_joystick_pin);

    gpio_set_irq_enabled_with_callback(button_A_pin, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &gpio_callback);
    gpio_set_irq_enabled_with_callback(button_B_pin, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &gpio_callback);
    gpio_set_irq_enabled_with_callback(button_joystick_pin, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &gpio_callback);
}
