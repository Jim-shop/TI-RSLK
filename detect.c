#include "detect.h"

#include <stdbool.h>

#include "motor.h"
#include "clock.h"
#include "reflectance.h"

#include "debug.h"

static bool has_stop_line(uint8_t data) {
    // detect 10101
    int flag = 0;
    int i = 0;
    while (i < 2 && !(data & (1 << i))) i++; // ignore left white
    while (i < 4 && (data & (1 << i))) i++, flag |= 1 << 0; // black
    while (i < 6 && !(data & (1 << i))) i++, flag |= 1 << 1; // white
    while (i < 6 && (data & (1 << i))) i++, flag |= 1 << 2; // black
    while (i < 8 && !(data & (1 << i))) i++, flag |= 1 << 3; // white
    while (i < 8 && (data & (1 << i))) i++, flag |= 1 << 4; // black
    while (i < 8 && !(data & (1 << i))) i++; // ignore right white
    return (i == 8 && flag == 0x1f);
}

static bool is_blank(uint8_t data) {
//    int cnt = 0;
//    for (int i = 0; i < 8; i++) cnt += (data >> i) & 0x1;
//    printf("\n\rblank cnt=%d", cnt);
//    return cnt <= 1;
    return data == 0x00;
}

static bool is_single_line(uint8_t data) {
    int flag = 0;
    int width = 0;
    int i = 0;
    data &= 0x7e;
    while (i < 8 && !(data & (1 << i))) i++, flag |= 1 << 0; // white
    while (i < 8 && (data & (1 << i))) i++, flag |= 1 << 1, width++; // black
    while (i < 8 && !(data & (1 << i))) i++, flag |= 1 << 2; // white
    return (i == 8 && flag == 0x07 && (1 <= width && width <= 4));
}

static bool is_left_line(uint8_t data) { // high is left
    int flag = 0;
    int i = 0;
    int hole = 0;
    int cnt = 0;
    while (i < 8 && !(data & (1 << i))) i++, flag |= 1 << 0; // white
    while (i < 8 && (data & (1 << i))) i++, flag |= 1 << 1, cnt++; // black
    while (i < 8 && !(data & (1 << i))) i++, hole++; // white hole
    while (i < 8 && (data & (1 << i))) i++, cnt++; // black
//    print_bits("left", data);
//    printf("\n\ri=%d, flag=%d, hole=%d, cnt=%d", i, flag, hole, cnt);
    return (i == 8 && flag == 0x03 && hole <= 1 && (3 <= cnt && cnt <= 6));
}

static bool is_right_line(uint8_t data) { // low is right
    int flag = 0;
    int i = 7;
    int hole = 0;
    int cnt = 0;
    while (i >= 0 && !(data & (1 << i))) i--, flag |= 1 << 0; // white
    while (i >= 0 && (data & (1 << i))) i--, flag |= 1 << 1, cnt++; // black
    while (i >= 0 && !(data & (1 << i))) i--, hole++; // white
    while (i >= 0 && (data & (1 << i))) i--, cnt++; // black
    // print_bits("right", data);
    // printf("\n\ri=%d, flag=%d, hole=%d, cnt=%d", i, flag, hole, cnt);
    return (i == -1 && flag == 0x03 && hole <= 1 && (3 <= cnt && cnt <= 6));
}

static bool is_solid_line(uint8_t data) {
    int cnt = 0;
    for (int i = 0; i < 8; i++) cnt += (data >> i) & 0x1;
    // printf("\n\rsolid cnt=%d", cnt);
    return cnt >= 6;
}

enum status detect(uint8_t data) {
    if (is_blank(data))
        return NIL;
    else if (has_stop_line(data))
        return STOP;
    else if (is_single_line(data))
        return STRAIGHT;
    motor_set(70, 0);
    clock_delay_ms(100);
    uint8_t second_data = reflectance_read(8192);
//    print_bits("data1", data);
//    print_bits("data2", second_data);
    bool then_straight = is_single_line(second_data);
    if (is_left_line(data))
        return then_straight ? STRAIGHT_LEFT : LEFT;
    else if (is_right_line(data))
        return then_straight ? STRAIGHT_RIGHT : RIGHT;
    else if (is_solid_line(data))
        return then_straight ? STRAIGHT_LEFT_RIGHT : LEFT_RIGHT;
    else {
        if (is_blank(second_data))
            return NIL;
        else if (has_stop_line(second_data))
            return STOP;
        else if (then_straight)
            return STRAIGHT;
        else
            return ERROR;
    }
}
