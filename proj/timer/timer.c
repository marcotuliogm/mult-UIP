/*
 ####        # 
 #           #
 ## ### #  # #
 #  #-# #\ # #
 #  # # # \# ####
 Author: Felipe de Andrade Neves Lavratti

 Copyright: There are no restrictions. Use as you want.	  
*/

/*
	LPC 2478
	+ TFT DISPLAY
	+ TOUCH PANNEL
	+ SWIM
	+ SDRAM

	Get more at: selivre.wordpress.com
*/
#include "timer.h"

void T0Init(int ms)
{
	T0TCR = 0;
	T0TCR =	2;
	T0PR = (CPU_CLOCK_HZ*ms/4000) - 1;
	T0TCR = 0;
}

void T0Stop()
{
	T0TCR = 0;
}
void T0Start()
{
	T0TCR = 1;
}

int T0Get()
{
	return T0TC;
}

void delayMs(unsigned int delayInMs)
{
	/*
	* setup timer #1 for delay
	*/
	T1TCR = 0x02;		/* reset timer */
	T1PR  = (CPU_CLOCK_HZ/4000 -1);		/* set prescaler to zero */
	T1MR0 = delayInMs;
	T1IR  = 0xff;		/* reset all interrrupts */
	T1MCR = 0x04;		/* stop timer on match */
	T1TCR = 0x01;		/* start timer */

	/* wait until delay time has elapsed */
	while (T1TCR & 0x01);
  return;
}

void delayUs(unsigned int delayInUs)
{
	/*
	* setup timer #1 for delay
	*/
	T2TCR = 0x02;		/* reset timer */
	T2PR  = (CPU_CLOCK_HZ/4000000 -1);
	T2MR0 = delayInUs;
	T2IR  = 0xff;		/* reset all interrrupts */
	T2MCR = 0x04;		/* stop timer on match */
	T2TCR = 0x01;		/* start timer */

	/* wait until delay time has elapsed */
	while (T2TCR & 0x01);
  return;
}

int timeoutMs(unsigned int delayInMs, unsigned int condition)
{
	switch(condition)
	{
	case START:
		T1TCR = 0x02;		/* reset timer */
		T1PR  = (CPU_CLOCK_HZ/4000 -1);
		T1MR0 = delayInMs;
		T1IR  = 0xff;		/* reset all interrrupts */
		T1MCR = 0x04;		/* stop timer on match */
		T1TCR = 0x01;		/* start timer */
		break;

	case CHECK_IF_MATCH:
		if (T1TCR&0x01) return NOT_MATCH;
		else return MATCH;

	}

	return 1;
}

void delay_1us(int t)	//configurar ini_timer(1000000);
{
     unsigned int tf;
     tf = T0TC + t;
     while(tf != T0TC);
}




