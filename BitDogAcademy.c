// Biblioteca padrão e da SDK
#include <stdio.h>
#include <stdint.h>

#include "state_machine.h"

int main() {
    stdio_init_all();
    // sleep_ms(5000);

    while(true) {
        run_state_machine();
    }

    return 0;
}
