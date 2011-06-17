#ifndef _LPC_CONFIG_H
#define _LPC_CONFIG_H

#include "lpc24xx.h"


#define XTAL_CLOCK 			12000000
#define CPU_CLOCK_HZ        ( ( unsigned int ) XTAL_CLOCK * 2 * (PLL_MUL+1) / (CPU_CLK_DIV+1) / (PLL_DIV+1) )	/* =11.0592Mhz xtal multiplied by 4 using the PLL. */

/* PLL parameters */
#define PLL_MUL					( ( unsigned int ) ( 6 - 1 ) )
#define PLL_DIV					( ( unsigned int ) 0x0000 )
#define CPU_CLK_DIV				( ( unsigned int ) 0x0001 )
#define PLL_ENABLE				( ( unsigned int ) 0x0001 )
#define PLL_CONNECT				( ( ( unsigned int ) 0x0002 ) | PLL_ENABLE )
#define PLL_FEED_BYTE1			( ( unsigned int ) 0xaa )
#define PLL_FEED_BYTE2			( ( unsigned int ) 0x55 )
#define PLL_LOCK				( ( unsigned int ) 0x4000000 )
#define PLL_CONNECTED			( ( unsigned int ) 0x2000000 )
#define OSC_ENABLE				( ( unsigned int ) 0x20 )
#define OSC_STAT				( ( unsigned int ) 0x40 )
#define OSC_SELECT				( ( unsigned int ) 0x01 )

/* Constants to setup the MAM. */
#define mainMAM_TIM_3				( ( unsigned char ) 0x03 )
#define mainMAM_MODE_FULL			( ( unsigned char ) 0x02 )

#define FALSE (1==2)
#define TRUE  !FALSE
#define PASS 1
#define FAIL 0

/************************************************************
Arquivo de cabecalho para habilitar interrupcoes no CPSR
(Current Processor Status Register)
************************************************************/

/* Declaracoes especiais para rotinas de atendimento a interrupcoes*/
void SWI_Routine(void) __attribute__ ((interrupt("SWI")));
void IRQ_Routine(void) __attribute__ ((interrupt("IRQ")));
void FIQ_Routine(void) __attribute__ ((interrupt("FIQ")));

/* Macros para habilitar/desabilitar IRQ/FIQ no CPSR */
#define enableFIQ() asm volatile(\
	"mrs r3, cpsr\n" \
	"bic r3, r3, # 0x40\n" \
	"msr cpsr, r3\n")

#define disableFIQ() asm volatile(\
	"mrs r3, cpsr\n" \
	"orr r3, r3, # 0x40\n" \
	"msr cpsr, r3\n")

#define enableIRQ() asm volatile(\
	"mrs r3, cpsr\n" \
	"bic r3, r3, # 0x80\n" \
	"msr cpsr, r3\n")

#define disableIRQ() asm volatile(\
	"mrs r3, cpsr\n" \
	"orr r3, r3, # 0x80\n" \
	"msr cpsr, r3\n")

#endif
