#ifndef UART0_H_
#define UART0_H_

/**
 * @details Initialize EUSCI_A0 for UART operation
 * @details 115,200 baud rate (assuming 12 MHz SMCLK clock),
 * @details 8 bit word length, no parity bits, one stop bit
 * @param   none
 * @return  none
 * @note    assumes 48 MHz bus and 12 MHz SMCLK
 * @brief   Initialize EUSCI A0
 */
void uart0_init_115200_8n1(void);

/**
 * @details Initializes C standard library, enables printf, scanf to work
 * @details Initialize EUSCI_A0 for UART operation
 * @details 115,200 baud rate (assuming 12 MHz SMCLK clock),
 * @details 8 bit word length, no parity bits, one stop bit
 * @param   none
 * @return  none
 * @note    assumes 48 MHz bus and 12 MHz SMCLK
 * @note    This initialization calls uart0_init
 * @brief   Initialize EUSCI A0 and printf
 */
void uart0_init_stdio(void);

/**
 * @details Receive a character from EUSCI_A0 UART
 * @details Busy-wait synchronization,
 * @details blocking, wait for new serial port input
 * @param   none
 * @return  ASCII code for key typed
 * @note    uart0_init must be called once prior
 * @brief   Receive byte into MSP432
 */
char uart0_inchar(void);

/**
 * @details Transmit a character to EUSCI_A0 UART
 * @details Busy-wait synchronization,
 * @details blocking, wait for UART to be ready
 * @param   letter is the ASCII code for key to send
 * @return  none
 * @note    uart0_init must be called once prior
 * @brief   Transmit byte out of MSP432
 */
void uart0_outchar(char c);

/**
 * @details Transmit a string to EUSCI_A0 UART
 * @param   pt is pointer to null-terminated ASCII string to be transferred
 * @return  none
 * @note    uart0_init must be called once prior
 * @brief   Transmit string out of MSP432
 */
void uart0_outstring(char *s);

#endif /* UART0_H_ */
