#ifndef MOTOR_H_
#define MOTOR_H_

/**
 * Initialize GPIO pins for output, which will be
 * used to control the direction of the motors and
 * to enable or disable the drivers.
 * The motors are initially stopped, the drivers
 * are initially powered down, and the PWM speed
 * control is uninitialized.
 * @param   none
 * @return  none
 * @brief   Initialize motor interface
 */
void motor_init(void);

/**
 * Stop the motors, power down the drivers, and
 * set the PWM speed control to 0% duty cycle.
 * @param   none
 * @return  none
 * @brief   Stop the robot
 */
void motor_stop(void);

/**
 * Set motors to apply forces to brake.
 * @param   none
 * @return  none
 * @brief   Brake the robot
 */
void motor_brake(void);

/**
 * Set speed and directly return.
 * @param   v   linear speed (unit: ~3mm/s)
 * @param   v_rot   rotate speed (unit: ~3degree/s)
 * @return  none
 * @brief   Set robot speed and directly return
 */
void motor_set(int v, int v_rot);

/**
 * Spin at 90 degree/s. Return until done spinning.
 * @param   degree spinning angle (degree)
 * @return  none
 * @brief   Spin and pending.
 */
void motor_spin(int degree);

/**
 * Shift at 20 cm/s. Return until done shifting.
 * @param   cm shifting distance (cm)
 * @return  none
 * @brief   Shift and pending.
 */
void motor_shift(int cm);

#endif /* MOTOR_H_ */
