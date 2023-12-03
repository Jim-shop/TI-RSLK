#include <stdbool.h>
#include <stdio.h>
#include <inttypes.h>
#include "msp.h"

#include "clock.h"
#include "uart0.h"
#include "motor.h"
#include "board.h"
#include "reflectance.h"

void main(void)
{
    clock_init_48mhz();
    board_init();
    motor_init();
    reflectance_init();

    uart0_init_stdio();
    int v, v_rot;
    while (true) {
        if (2 != scanf(" %" SCNd32 "%" SCNd32, &v, &v_rot))
            while (getchar() != '\r');
        printf("\n\rv=%" PRId32 ", v_rot=%" PRId32 "\n\r", v, v_rot);
        motor_set(v, v_rot);
        clock_delay_ms(1000);
        motor_set(0, 0);
    }

    while (true) {
        while (!board_s1())
            clock_delay_ms(10);
        motor_shift(10);
        motor_spin(180);
        motor_shift(10);
        motor_spin(180);
        motor_brake();
    }
}
