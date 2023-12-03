#include "msp.h"
#include "pwm.h"
#include "encoder.h"
#include "timer1.h"
#include "clock.h"

#define PWM_LIMIT 512
#define PID_PERIOD 10000 // 20ms

#define MOTOR_Kp 8
#define MOTOR_Ki 4
#define MOTOR_Kd 0

static volatile struct wheel {
    int v_current, v_target;
    int error, pre_error, pre_pre_error;
    int pwm;
} left, right;

static inline void wheel_calc_pwm(struct wheel volatile *wheel) {
    wheel->pre_pre_error = wheel->pre_error;
    wheel->pre_error = wheel->error;
    wheel->error = wheel->v_target - wheel->v_current;
    wheel->pwm += MOTOR_Kp * (wheel->error - wheel->pre_error)
                + MOTOR_Ki * wheel->error
                + MOTOR_Kd * (wheel->error - 2 * wheel->pre_error + wheel->pre_pre_error);
    wheel->pwm = wheel->pwm >= PWM_LIMIT ? PWM_LIMIT - 1 : wheel->pwm < -PWM_LIMIT ? -PWM_LIMIT : wheel->pwm;
}

static inline void wheel_write_pwm() {
    if (left.pwm >= 0) {
        P1->OUT |= BIT7;
        pwm_setduty4(PWM_LIMIT - left.pwm);
    } else {
        P1->OUT &= ~BIT7;
        pwm_setduty4(-left.pwm);
    }
    if (right.pwm >= 0) {
        P1->OUT |= BIT6;
        pwm_setduty3(PWM_LIMIT - right.pwm);
    } else {
        P1->OUT &= ~BIT6;
        pwm_setduty3(-right.pwm);
    }
}

static void motor_adj_speed(void) {
    // read current speed
    left.v_current = encoder_get_cnt_left();
    right.v_current = encoder_get_cnt_right();
    // calc pwm
    wheel_calc_pwm(&left);
    wheel_calc_pwm(&right);
    // write pwm
    wheel_write_pwm();
}

void motor_init(void) {
    // DRV8837 (x.6: right, x.7: left) (1.x: DIR, 2.x: PWM, 3.x: nSLP)
    // DIR (1.6, 1.7): 1 forward, 0 backward
    P1->SEL0 &= ~(BIT6 | BIT7);
    P1->SEL1 &= ~(BIT6 | BIT7); // GPIO
    P1->DIR |= (BIT6 | BIT7); // output
    P1->OUT &= ~(BIT6 | BIT7); // backward
    // nSLP (3.6, 3.7): 0 sleeping, 1 working
    P3->SEL0 &= ~(BIT6 | BIT7);
    P3->SEL1 &= ~(BIT6 | BIT7); // GPIO
    P3->DIR |= (BIT6 | BIT7); // output
    P3->OUT &= ~(BIT6 | BIT7); // sleeping

    pwm_init34(PWM_LIMIT - 1, 0, 0); // speed 0 when running backward
    encoder_init();
    left.v_target = right.v_target = 0;
    timer1_init(PID_PERIOD, motor_adj_speed);
}

void motor_stop(void) {
    P3->OUT &= ~(BIT6 | BIT7); // sleep
    left.v_target = right.v_target = 0;
}

void motor_brake(void) {
    P3->OUT |= (BIT6 | BIT7);
    left.v_target = right.v_target = 0;
}

void motor_set(int v, int v_rot) {
    P3->OUT |= (BIT6 | BIT7);
    left.v_target = v + v_rot;
    right.v_target = v - v_rot;
}

void motor_spin(int degree) {
    motor_set(0, 30);
    clock_delay_ms(degree * 10);
    motor_stop();
}

void motor_shift(int cm) {
    motor_set(60, 0);
    clock_delay_ms(cm * 50);
    motor_stop();
}
