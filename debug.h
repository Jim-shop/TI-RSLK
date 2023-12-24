#ifndef DEBUG_H_
#define DEBUG_H_

#include <inttypes.h>
#include <stdio.h>

static inline void print_bits(const char *name, uint8_t data) {
    printf("\n\r%s=", name);
    for (int i = 8; i-- > 0;)
        printf("%d", (data >> i) & 0x1);
}



#endif /* DEBUG_H_ */
