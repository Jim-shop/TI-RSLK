#include <stdint.h>
#include "msp.h"

uint32_t CLOCK_FREQ = 3000000; // Hz

void clock_init_48mhz(void) {
    // Step 1: transition from `AM0_LDO` to `AM1_LDO`
    while (PCM->CTL1 & PCM_CTL1_PMR_BUSY);
    PCM->CTL0 = (PCM->CTL0 & ~(PCM_CTL0_KEY_MASK | PCM_CTL0_AMR_MASK)) | PCM_CTL0_KEY_VAL | PCM_CTL0_AMR__AM_LDO_VCORE1;
    if (PCM->IFG & PCM_IFG_AM_INVALID_TR_IFG) { // transition not success
        PCM->CLRIFG = PCM_CLRIFG_CLR_AM_INVALID_TR_IFG;
        return;
    }
    while ((PCM->CTL0 & PCM_CTL0_CPM_MASK) != PCM_CTL0_CPM__AM_LDO_VCORE1);
    while (PCM->CTL1 & PCM_CTL1_PMR_BUSY);
    // Step 2: configure oscillator pin (built-in 48Mhz: PJ.3 out, PJ.2 in)
    PJSEL0 |= (BIT2 | BIT3);
    PJSEL1 &= ~(BIT2 | BIT3);
    // Step 3: configure HFXT
    CS->KEY = CS_KEY_VAL;
    CS->CTL2 = (CS->CTL2 & ~(CS_CTL2_HFXTFREQ_MASK | CS_CTL2_HFXTBYPASS)) | CS_CTL2_HFXTFREQ_6 | CS_CTL2_HFXTDRIVE | CS_CTL2_HFXT_EN;
    while (CS->IFG & CS_IFG_HFXTIFG) // wait until HFXT stablize
        CS->CLRIFG = CS_CLRIFG_CLR_HFXTIFG;
    // Step 4: configure flash wait-state to 2 for both banks 0 & 1
    FLCTL->BANK0_RDCTL = (FLCTL->BANK0_RDCTL & ~FLCTL_BANK0_RDCTL_WAIT_MASK) | FLCTL_BANK0_RDCTL_WAIT_2;
    FLCTL->BANK1_RDCTL = (FLCTL->BANK1_RDCTL & ~FLCTL_BANK1_RDCTL_WAIT_MASK) | FLCTL_BANK1_RDCTL_WAIT_2;
    // Step 5: configure clock
    CS->CTL1 = CS_CTL1_SELM__HFXTCLK | CS_CTL1_SELS__HFXTCLK | CS_CTL1_SELA__REFOCLK | CS_CTL1_DIVHS__2 | CS_CTL1_DIVS__4;
    CS->KEY = 0;
    // Step 6: update meta
    CLOCK_FREQ = 48000000;
}

static inline void delay(volatile uint32_t cycle6) { // uint64_t calculate faster, surprising right?
    asm volatile (
        "pdloop: SUBS   R0, #1      \n"
        "        BNE    pdloop      \n"
    );
}

void clock_delay_ms(volatile uint32_t n) {
    while (n) {
        delay(CLOCK_FREQ / 9162); // tune at 48 MHz
        n--;
    }
}

void clock_delay_us(volatile uint32_t n) {
    n = (382 * n) / 100; // tune at 48 MHz
    while (n) {
        n--;
    }
}
