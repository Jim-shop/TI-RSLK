#include <stdbool.h>
#include <stdio.h>
#include <inttypes.h>
#include "msp.h"

#include "clock.h"
#include "uart0.h"
#include "motor.h"
#include "board.h"
#include "reflectance.h"
#include "detect.h"

#include "debug.h"

void main(void)
{
    clock_init_48mhz();
    board_init();
    motor_init();
    reflectance_init();

    uart0_init_stdio();
    while (true) {
        // printf("\r\nidle");
        while (!board_s1()) // start signal
            clock_delay_ms(10);
        // printf("\r\nstart!");
        while (true) {
            uint8_t ref = reflectance_read(8192);
            int32_t pos = reflectance_position(ref);
            bool terminate = false;
            enum status result = detect(ref);
            switch (result) {
            case STOP:
                board_led2(false, false, false);
                // printf("\n\rstop line!");
                terminate = true;
                break;
            case NIL:
                board_led2(false, false, false);
                // printf("\n\rback");
                motor_spin(-90);
                break;
            case ERROR:
                board_led1(true);
                board_led2(false, false, false);
                break;
            case STRAIGHT:
                board_led2(true, false, false);
                motor_set(70, 0.5f * pos);
                break;
            case LEFT:
                board_led2(false, true, false);
                // printf("\n\rturn left");
                motor_spin(-90);
                motor_shift(1);
                break;
            case RIGHT:
                board_led2(false, false, true);
                // printf("\n\rturn right");
                motor_spin(90);
                motor_shift(1);
                break;
            case STRAIGHT_LEFT:
                board_led2(true, true, false);
                // printf("\n\rstraight+left: choose left");
                motor_spin(-90);
                motor_shift(1);
                break;
            case STRAIGHT_RIGHT:
                board_led2(true, false, true);
                // printf("\n\rtraight+right: choose straight");
                motor_shift(1);
                break;
            case STRAIGHT_LEFT_RIGHT:
                board_led2(true, true, true);
                // printf("\n\rstraight+left+right: choose left");
                motor_spin(-90);
                motor_shift(1);
                break;
            case LEFT_RIGHT:
                board_led2(false, true, true);
                // printf("\n\rleft+right: choose left");
                motor_spin(-90);
                motor_shift(1);
                break;
            }
            if (terminate)
                break;
        }
        motor_brake();
    }
}
