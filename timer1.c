#include "msp.h"

static void (*timer1_callback)(void);

void timer1_init(uint16_t period2us, void (*callback)(void)) {
    timer1_callback = callback;
    // timer setup
    TIMER_A1->CTL = TIMER_A_CTL_MC__STOP; // halt first
    TIMER_A1->CTL = TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_ID__4;
    TIMER_A1->CCTL[0] = TIMER_A_CCTLN_CCIE;
    TIMER_A1->CCR[0] = (period2us - 1);
    TIMER_A1->EX0 = TIMER_A_EX0_IDEX__6;
    // nvic setup (INT10: TA1.0)
    NVIC->IP[2] = (NVIC->IP[2] & ~NVIC_IPR2_PRI_10_M) | 0x00400000; // priority 2
    NVIC->ISER[0] = BIT(TA1_0_IRQn); // enable INT10
    // timer start
    TIMER_A1->CTL |= TIMER_A_CTL_MC__UP | TIMER_A_CTL_CLR;
}

void timer1_stop(void) {
    TIMER_A1->CTL = TIMER_A_CTL_MC__STOP; // halt first
    NVIC->ICER[0] = BIT(TA1_0_IRQn); // disable INT10
}

// override weak symbol
void TA1_0_IRQHandler(void) {
    TIMER_A1->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
    timer1_callback();
}
