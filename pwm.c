#include "msp.h"

void pwm_init34(uint16_t period, uint16_t duty3, uint16_t duty4) {
    // P2.6, P2.7 <- TA0.CCR3, TA0.CCR4
    P2->DIR |= (BIT6 | BIT7); // output
    P2->SEL0 |= (BIT6 | BIT7);
    P2->SEL1 &= ~(BIT6 | BIT7); // timer A0 function

    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_OUTMOD_4; // Toggle
    TIMER_A0->EX0 = TIMER_A_EX0_IDEX__1;
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_ID__1 | TIMER_A_CTL_MC__UP;
    TIMER_A0->CCR[0] = period;

    TIMER_A0->CCTL[3] = TIMER_A_CCTLN_OUTMOD_7; // reset / set
    TIMER_A0->CCR[3] = duty3;
    TIMER_A0->CCTL[4] = TIMER_A_CCTLN_OUTMOD_7;
    TIMER_A0->CCR[4] = duty4;
}

void pwm_setduty3(uint16_t duty3) {
    TIMER_A0->CCR[3] = duty3;
}

void pwm_setduty4(uint16_t duty4) {

    TIMER_A0->CCR[4] = duty4;
}
