#ifndef PWM_H_
#define PWM_H_

/**
 * @details Initialize PWM outputs on P2.6, P2.7
 * @remark  Counter counts up to TA0CCR0 and back down
 * @remark  P2.6=1 when timer equals TA0CCR3 on way down, P2.6=0 when timer equals TA0CCR3 on way up
 * @remark  P2.7=1 when timer equals TA0CCR4 on way down, P2.7=0 when timer equals TA0CCR4 on way up
 * @remark  Assumes 48 MHz bus clock
 * @remark  Assumes SMCLK = 48MHz/4 = 12 MHz
 * @param   duty3 is initial width of high pulse on P2.6
 * @param   duty4 is initial width of high pulse om P2.7
 * @return  none
 * @brief   Initialize PWM3 PWM4
 */
void pwm_init34(uint16_t period, uint16_t duty3, uint16_t duty4);

/**
 * @details Set duty cycle on P2.6
 * @param   duty3 is width of high pulse on P2.6
 * @return  none
 * @warning duty3 must be less than period
 * @brief   set duty cycle on PWM3
 */
void pwm_setduty3(uint16_t duty3);

/**
 * @details  Set duty cycle on P2.7
 * @param    duty4 is width of high pulse on P2.7
 * @return   none
 * @warning  duty4 must be less than period
 * @brief    set duty cycle on PWM4
 */
void pwm_setduty4(uint16_t duty4);

#endif /* PWM_H_ */
