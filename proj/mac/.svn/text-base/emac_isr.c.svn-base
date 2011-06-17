#include "../eth/eth.h"
#include "../uart/uart.h"

void EMAC_ISR_Handler( void ) __attribute__ ((interrupt("IRQ")));

void EMAC_ISR_Handler( void )
{
    MAC_INTCLEAR = 0xf0ff;
    VICVectAddr = 0;
	//uart_putc('e');
	ethernet_task();
}
