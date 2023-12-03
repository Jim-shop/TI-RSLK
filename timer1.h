#ifndef TIMER1_H_
#define TIMER1_H_

/**
 * Activate Timer A1 interrupts to run user task periodically
 * @param   callback is a pointer to a user function called periodically
 * @param   period2us in 2us units (24/SMCLK), 16 bits
 * @return  none
 * @note    Assumes low-speed subsystem master clock is 12 MHz.
 * With divide by 24 the timer clock will be 500 kHz timer clock.
 * The slowest period is 65535*2us=130ms, or about 8 Hz
 * @brief   Initialize Timer A1
 */
void timer1_init(uint16_t period2us, void (*callback)(void));

/**
 * Deactivate the interrupt running a user task periodically.
 * @param   none
 * @return  none
 * @brief   Deactivate Timer A1
 */
void timer1_stop(void);

#endif
