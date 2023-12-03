#ifndef REFLECTANCE_H_
#define REFLECTANCE_H_

#include <stdint.h>

/**
 * Initialize the GPIO pins associated with the QTR-8RC.
 * One output to IR LED, 8 inputs from the sensor array.
 * Initially, the IR outputs are off.
 * @param  none
 * @return none
 * @brief  Initialize the GPIO pins for the QTR-8RC reflectance sensor.
 */
void reflectance_init(void);

/**
 * Read the eight sensors
 * 1) Turn on the 8 IR LEDs<br>
 * 2) Pulse the 8 sensors high for 10 us<br>
 * 3) Make the sensor pins input<br>
 * 4) Wait wait_us us<br>
 * 5) Read sensors (white is 0, black is 1)<br>
 * 6) Turn off the 8 IR LEDs<br>
 * @param  wait_us delay value in us
 * @return 8-bit result
 * @note Assumes reflectance_init() has been called
 * @brief  Read the eight sensors.
 */
uint8_t reflectance_read(uint32_t wait_us);

/**
 * Calculate the weighted average for each bit
 * Position varies from -332 (left) to +332 (right), with units of 0.1mm.
 * @param  data is 8-bit result from line sensor
 * @return position in 0.1mm relative to center of line
 * @brief  Perform sensor integration.
 * @note returns 333 if data is zero (off the line)
 */
int32_t reflectance_position(uint8_t data);

#endif /* REFLECTANCE_H_ */
