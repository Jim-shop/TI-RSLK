#include <stdbool.h>
#include "msp.h"

void board_init(void) {
    // neg-logic button S1 (P1.1), S2 (P1.4)
    P1->SEL0 &= ~(BIT1 | BIT4);
    P1->SEL1 &= ~(BIT1 | BIT4); // GPIO
    P1->DIR &= ~(BIT1 | BIT4); // input
    P1->REN |= (BIT1 | BIT4); // enable pull resistors
    P1->OUT |= (BIT1 | BIT4); // pull-up
    // LED1 (P1.0)
    P1->SEL0 &= ~BIT0;
    P1->SEL1 &= ~BIT0; // GPIO
    P1->DIR |= BIT0; // output
    P1->OUT &= ~BIT0; // turn off
    // LED2 (R: P2.0; G: P2.1; B: P2.2)
    P2->SEL0 &= ~(BIT0 | BIT1 | BIT2);
    P2->SEL1 &= ~(BIT0 | BIT1 | BIT2); // GPIO
    P2->DIR |= (BIT0 | BIT1 | BIT2); // output
    P2->DS |= (BIT0 | BIT1 | BIT2); // increased drive strength
    P2->OUT &= ~(BIT0 | BIT1 | BIT2); // turn off
}

bool board_s1(void) {
    return ~P1->IN & BIT1;
}

bool board_s2(void) {
    return ~P1->IN & BIT4;
}

void board_led1(bool state) {
    P1->OUT = (P1->OUT & ~BIT0) | (state << 0);
}

void board_led2(bool r, bool g, bool b) {
    P2->OUT = (P2->OUT & ~(BIT0 | BIT1 | BIT2)) | (r << 0) | (g << 1) | (b << 2);
}
