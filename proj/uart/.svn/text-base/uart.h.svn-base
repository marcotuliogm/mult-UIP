/*
 * uart.h
 *
 *  Created on: 10/07/2009
 *      Author: Felipe Lavratti
 */

#ifndef UART_H_
#define UART_H_

#include "../lpc_config.h"

#define uart_puts(__i__) 	UARTTransmitString(__i__, UART_CH_0)
#define uart_putc(__i__) 	UARTTransmitByte(__i__, UART_CH_0)


#define	UART_CH_0	0x0
#define	UART_CH_1	0x1
#define	UART_CH_2	0x2
#define	UART_CH_3	0x4

#define UART_CLK CPU_CLOCK_HZ/4

void	UARTInit(unsigned int baud, char location, char channel);
char	UARTIsDataReady(char channel);
char	UARTReceiveByte(char channel);
char	UARTWaitForByte(char channel);
void	UARTTransmitByte(char ch, char channel);
void 	UARTTransmitString(char *str, char channel);
//void 	UARTTakeString(char *str, char channel);

#endif /* UART_H_ */
