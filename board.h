#ifndef BOARD_H_
#define BOARD_H_

#include <stdbool.h>

/**
 * Initialize LaunchPad I/O
 *
 * @param  none
 * @return none
 * @brief  Initialize Switch inputs and LED outputs
 */
void board_init(void);

/**
 * Input from button S1 on LaunchPad
 * Value returned in pos-logic
 *
 * @param  none
 * @return state (pos-logic)
 * @brief  Input from button S1
 */
bool board_s1(void);

/**
 * Input from button S2 on LaunchPad
 * Value returned in pos-logic
 *
 * @param  none
 * @return state (pos-logic)
 * @brief  Input from button S2
 */
bool board_s2(void);

/**
 * Output to red LED on LaunchPad
 *
 * @param  state is value to send to LED,
 * 0 is off; 1 is on
 * @return none
 * @brief  Output to red LED
 */
void board_led1(bool state);

/**
 * Output to 3-color LED on LaunchPad
 *
 * @param  r is value to send to rd LED
 * @param  g is value to send to green LED
 * @param  b is value to send to blue LED
 * 0 is off; 1 is on
 * @return none
 * @brief  Output to 3-color LED
 */
void board_led2(bool r, bool g, bool b);

#endif /* BOARD_H_ */
