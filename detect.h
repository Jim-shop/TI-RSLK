#ifndef DETECT_H_
#define DETECT_H_

#include <inttypes.h>

enum status {
    ERROR, STOP, NIL, STRAIGHT, LEFT, RIGHT, STRAIGHT_LEFT, STRAIGHT_RIGHT, STRAIGHT_LEFT_RIGHT, LEFT_RIGHT
};

enum status detect(uint8_t data);

#endif /* DETECT_H_ */
