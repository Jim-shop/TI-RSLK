#include <stdint.h>
#include <stdbool.h>
#include "msp.h"
#include "clock.h"

void reflectance_init(void) {
    // LEDON (P5.3)
    P5->DIR |= BIT3; // output
    P5->OUT &= ~BIT3; // turn off IR LEDs
    // OUT1~8 (P7.0~P7.7)
    P7->DIR &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7); // input
}

uint8_t reflectance_read(uint32_t wait_us) {
    // 1) Turn on the 8 IR LEDs
    P5->OUT |= BIT3;
    P7->DIR |= BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7; // output
    P7->DIR |= BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7; // IR LEDs on
    // 2) Pulse the 8 sensors high for 10 us
    clock_delay_us(10);
    // 3) Make the sensor pins input
    P7->DIR &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7); // input
    // 4) Wait wait_us
    clock_delay_us(wait_us);
    // 5) Read sensors (white is 0, black is 1)
    uint8_t result = P7->IN;
    // 6) Turn off the 8 IR LEDs
    P5->OUT &= ~BIT3;
    return result;
}

int32_t reflectance_position(uint8_t data) {
    const int32_t weight[8] = {332, 237, 142, 47, -47, -142, -237, -332};
    if (data == 0)
        return INT32_MAX; // off line
    int32_t sum_weight = 0, sum_bit = 0;
    for (int i = 0; i < 8; i++) {
        if (data & BIT(i)) {
            sum_bit++;
            sum_weight += weight[i];
        }
    }
    return sum_weight / sum_bit;
}
