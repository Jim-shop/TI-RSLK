#include "msp.h"

static volatile int encoder_cnt_left = 0, encoder_cnt_right = 0;

void encoder_init(void) {
    // RA: P10.4 (TA3.0)
    P10->SEL0 |= BIT4;
    P10->SEL1 &= ~BIT4; // second function (TA3.x instead of GPIO)
    P10->DIR &= ~BIT4; // input
    // RB: P10.5 (TA3.1)
    P10->SEL0 |= BIT5;
    P10->SEL1 &= ~BIT5;
    P10->DIR &= ~BIT5;
    // LA: P8.2 (TA3.2)
    P8->SEL0 |= BIT2;
    P8->SEL1 &= ~BIT2;
    P8->DIR &= ~BIT2;
    // LB: P9.2 (TA3.3)
    P9->SEL0 |= BIT2;
    P9->SEL1 &= ~BIT2;
    P9->DIR &= ~BIT2;
    // timer setup
    TIMER_A3->CTL = TIMER_A_CTL_MC__STOP; // halt first
    TIMER_A3->CTL = TIMER_A_CTL_SSEL__SMCLK; // assume SMCLK = 12MHz
    TIMER_A3->CCTL[0] = TIMER_A3->CCTL[1] = TIMER_A3->CCTL[2] = TIMER_A3->CCTL[3]
        = TIMER_A_CCTLN_CCIS__CCIA | TIMER_A_CCTLN_CM__BOTH | TIMER_A_CCTLN_SCS | TIMER_A_CCTLN_CAP | TIMER_A_CCTLN_CCIE;
    TIMER_A3->EX0 = (TIMER_A3->EX0 & ~TIMER_A_EX0_IDEX_MASK) | TIMER_A_EX0_IDEX__1;
    // INT14 <- TA3CCTL0.CCIFG; INT15 <- TA3CCTL(1-4).CCIFG
    NVIC->IP[3] = (NVIC->IP[3] & ~(NVIC_IPR3_PRI_14_M | NVIC_IPR3_PRI_15_M)) | 0x40400000; //  priority 2
    NVIC->ISER[0] = BIT(TA3_0_IRQn) | BIT(TA3_N_IRQn);
    // timer start
    TIMER_A3->CTL |= TIMER_A_CTL_MC__CONTINUOUS | TIMER_A_CTL_CLR;
}

// Override weak symbol
void TA3_0_IRQHandler(void) { // CCR0 (RA)
    TIMER_A3->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
    if (!!(P10->IN & BIT4) ^ !!(P10->IN & BIT5)) // A faster than B
        encoder_cnt_right--;
    else
        encoder_cnt_right++;
}
void TA3_N_IRQHandler(void) { // CCR1~4
    switch (TIMER_A3->IV) {
    case 0x02: // CCR1 (RB)
        TIMER_A3->CCTL[1] &= ~TIMER_A_CCTLN_CCIFG;
        if (!!(P10->IN & BIT4) ^ !!(P10->IN & BIT5)) // B faster than A
            encoder_cnt_right++;
        else
            encoder_cnt_right--;
        break;
    case 0x04: // CCR2 (LA)
        TIMER_A3->CCTL[2] &= ~TIMER_A_CCTLN_CCIFG;
        if (!!(P8->IN & BIT2) ^ !!(P9->IN & BIT2)) // A faster than B
            encoder_cnt_left++;
        else
            encoder_cnt_left--;
        break;
    case 0x06: // CCR3 (LB)
        TIMER_A3->CCTL[3] &= ~TIMER_A_CCTLN_CCIFG;
        if (!!(P8->IN & BIT2) ^ !!(P9->IN & BIT2)) // B faster than A
            encoder_cnt_left--;
        else
            encoder_cnt_left++;
        break;
    }
}

int encoder_get_cnt_left(void) {
    int result = encoder_cnt_left;
    encoder_cnt_left = 0;
    return result;
}

int encoder_get_cnt_right(void) {
    int result = encoder_cnt_right;
    encoder_cnt_right = 0;
    return result;
}

