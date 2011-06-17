/*
 ####        # 
 #           #
 ## ### #  # #
 #  #-# #\ # #
 #  # # # \# ####
 Author: Felipe de Andrade Neves Lavratti

 Copyright: There are no restrictions. Use as you want.	  
*/

#include "uart.h"


/* the "location parameter is used when the particular channel can be connected
 * to many different places. the order is P0_x, P1_x, etc. */
void	UARTInit(unsigned int baud, char channel, char location)
{
    unsigned int pclkoffset,  div_l, div_h;

    switch (channel)
	{
		case UART_CH_0:
			PCLKSEL0 &= ~(0x3<<6);
			pclkoffset = 6;
			break;

		case UART_CH_1:
			PCLKSEL0 &= ~(0x3<<8);
			pclkoffset = 8;
			break;

/*		case UART_CH_2:
			PCLKSEL0 &= ~(0x3<<48);
			pclkoffset = 48;
			break;

		case UART_CH_3:
			PCLKSEL0 &= ~(0x3<<50);
			pclkoffset = 50;
			break;
*/
		default:
			return;
	}


	div_h = ((UART_CLK/baud) >> 12) & 0xff;
	div_l = ((UART_CLK/baud+8) >> 4) & 0xff;

	switch (channel)
	{
		case UART_CH_0:
			PINSEL0 &= ~(0x00000050);
			PINSEL0 |= 0x00000050;
			PCONP |= 0x1<<3;
			U0LCR = 0x83;   /* 8 bits, no Parity, 1 Stop bit */
			U0DLM = div_h;
			U0DLL = div_l;
			U0LCR = 0x03;   /* DLAB = 0 */
			U0FCR = 0x07;   /* Enable and reset TX and RX FIFO. */
			break;

		case UART_CH_1:
			switch (location)
			{
				case 0:
					PINSEL0 &= ~(0xC0000000);
					PINSEL0 |= 0x40000000;
					PINSEL1 &= ~(0x00000003);
					PINSEL1 |= 0x00000001;
					break;

				case 1:
					PINSEL4 &= ~(0x0000000F);
					PINSEL4 |= 0x00000000A;
					break;

				case 2:
					PINSEL7 |= 0x0000000F;
					break;
			}
			PCONP |= 0x1<<4;
			U1LCR = 0x83;   /* 8 bits, no Parity, 1 Stop bit */
			U1DLM = div_h;
			U1DLL = div_l;
			U1LCR = 0x03;   /* DLAB = 0 */
			U1FCR = 0x07;   /* Enable and reset TX and RX FIFO. */
			break;

		case UART_CH_2:
			switch (location)
			{
				case 0:
					PINSEL0 &= ~(0x00F00000);
					PINSEL0 |=   0x00500000;
					break;

				case 1:
					PINSEL4 &= ~(0x000F0000);
					PINSEL4 |=   0x000A0000;
					break;

				case 2:
					PINSEL9 &= ~(0x0000F000);
					PINSEL9 |=   0x0000A000;
					break;
			}
			PCONP |= 0x1<<24;
			U2LCR = 0x83;   /* 8 bits, no Parity, 1 Stop bit */
			U2DLM = div_h;
			U2DLL = div_l;
			U2LCR = 0x03;   /* DLAB = 0 */
			U2FCR = 0x07;   /* Enable and reset TX and RX FIFO. */
			break;

		case UART_CH_3:
			switch (location)
			{
				case 0:
					PINSEL0 &= ~(0x0000000F);
					PINSEL0 |=   0x0000000A;
					break;

				case 1:
					PINSEL1 |=   0x003C0000;
					break;

				case 2:

					PINSEL9 |=   0x0F000000;
					break;
			}
			PCONP |= 0x1<<25;
			U3LCR = 0x83;   /* 8 bits, no Parity, 1 Stop bit */
			U3DLM = div_h;
			U3DLL = div_l;
			U3LCR = 0x03;   /* DLAB = 0 */
			U3FCR = 0x07;   /* Enable and reset TX and RX FIFO. */
			break;
	}
}


char UARTIsDataReady(char channel)
{
	switch (channel)
	{
		case UART_CH_0:
			if (U0LSR & 0x01) return TRUE;
				else return FALSE;

		case UART_CH_1:
			if (U1LSR & 0x01) return TRUE;
				else return FALSE;

		case UART_CH_2:
			if (U2LSR & 0x01) return TRUE;
				else return FALSE;

		case UART_CH_3:
			if (U3LSR & 0x01) return TRUE;
				else return FALSE;

		default:
			return FALSE;
	}

}
char UARTWaitForByte(char channel)
{
	switch (channel)
	{
		case UART_CH_0:
			while((U0LSR & 0x01)==0);
			return U0RBR;

		case UART_CH_1:
			while((U1LSR & 0x01)==0);
			return U1RBR;

		case UART_CH_2:
			while((U2LSR & 0x01)==0);
			return U2RBR;

		case UART_CH_3:
			while((U3LSR & 0x01)==0);
			return U3RBR;

		default:
			return FALSE;
	}
}

char UARTReceiveByte(char channel)
{
	switch (channel)
	{
		case UART_CH_0:
			//while((U0LSR & 0x01)==0);	//enquanto não a sinal de caracter vindo da serial.
			return U0RBR;

		case UART_CH_1:
			return U1RBR;

		case UART_CH_2:
			return U2RBR;

		case UART_CH_3:
			return U3RBR;

		default:
			return FALSE;
	}
}

void	UARTTransmitByte(char ch, char channel)
{
	switch (channel)
	{
		case UART_CH_0:
			while (!(U0LSR & 0x20));
			U0THR = ch;
			break;

		case UART_CH_1:
			while (!(U1LSR & 0x20));
			U1THR = ch;
			break;

		case UART_CH_2:
			while (!(U2LSR & 0x20));
			U2THR = ch;
			break;

		case UART_CH_3:
			while (!(U3LSR & 0x20));
			U3THR = ch;
			break;

		default:
			break;
	}

}

void UARTTransmitString(char *str, char channel)
{
	while(*str) UARTTransmitByte(*str++, channel);
}

/*void UARTTakeString(char *str, char channel)
{
	int vd = 0xff;
	char tp[10]="\0";
	char cond = 0x0d;
	do{
			vd = UARTReceiveByte(channel);
		if (vd != cond) sprintf(tp, "%s%c", tp,vd);
	}while (vd != cond);
	strcpy(str, tp);
}*/


