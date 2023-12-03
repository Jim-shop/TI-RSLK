#ifndef ENCODER_H_
#define ENCODER_H_

/**
 * Initialize Timer A3 in edge time mode to request interrupts on
 * the rising edges of P10.4, P10.5, P8.2, P9.2.
 * @return  none
 * @note    Assumes low-speed subsystem master clock is 12 MHz
 * @brief   Initialize Timer A3
 */
void encoder_init(void);

int encoder_get_cnt_left(void);
int encoder_get_cnt_right(void);

#endif /* ENCODER_H_ */
