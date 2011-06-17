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

#ifndef TIMER_H_
#define TIMER_H_

#include "../lpc_config.h"

#define START 0x07
#define MATCH TRUE
#define NOT_MATCH FALSE
#define CHECK_IF_MATCH 0x08

void T0Init(int ms);
void T0Stop();
void T0Start();
int T0Get();
void delayMs(unsigned int delayInMs);
void delayUs(unsigned int delayInUs);
int timeoutMs(unsigned int delayInMs, unsigned int condition);

#endif /* TIMER_H_ */
