#ifndef CLOCK_H_
#define CLOCK_H_

#include <stdint.h>

/**
 * @details Configure the MSP432 clock to run at 48 MHz, SMCLK at 12 MHz, HSMCLK at 24 MHz
 * @param   none
 * @return  none
 * @brief   Initialize clock to 48 MHz
 */
void clock_init_48mhz(void);
/**
 * @brief The current bus clock frequency
 */
 extern const volatile uint32_t CLOCK_FREQ;
/**
 * @details Simple delay function which delays about n milliseconds.
 * @details It is implemented with a nested for-loop and is very approximate.
 * @param   n is the number of msec to wait
 * @return  none
 * @note    This function assumes a 48 MHz clock.
 * @brief   Software implementation of a busy-wait delay
 */
void clock_delay_ms(uint32_t n);
/**
 * @details Simple delay function which delays about n microseconds.
 * @details It is implemented with a nested for-loop and is very approximate.
 * @param   n is the number of usec to wait
 * @return  none
 * @note    This function assumes a 48 MHz clock.
 * @brief  Software implementation of a busy-wait delay
 */
void clock_delay_us(uint32_t n);

#endif /* CLOCK_H_ */
