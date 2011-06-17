/*****************************************************************************
 *   rtctest.c:  main C entry file for NXP LPC23xx/24xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.07.13  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#include "LPC23xx.H"                        /* LPC23xx/24xx definitions */
#include "type.h"
#include "irq.h"
#include "timer.h"
#include "rtc.h"

extern volatile DWORD alarm_on;
RTCTime local_time, alarm_time, current_time;

/*****************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{	    		
    /* Initialize RTC module */
    RTCInit();

    local_time.RTC_Sec = 0;
    local_time.RTC_Min = 0;
    local_time.RTC_Hour = 0;
    local_time.RTC_Mday = 8;
    local_time.RTC_Wday = 3;
    local_time.RTC_Yday = 12;		/* current date 07/12/2006 */
    local_time.RTC_Mon = 07;
    local_time.RTC_Year = 2006;
    RTCSetTime( local_time );		/* Set local time */

    alarm_time.RTC_Sec = 0;
    alarm_time.RTC_Min = 0;
    alarm_time.RTC_Hour = 0;
    alarm_time.RTC_Mday = 1;
    alarm_time.RTC_Wday = 0;
    alarm_time.RTC_Yday = 1;		/* alarm date 01/01/2007 */
    alarm_time.RTC_Mon = 1;
    alarm_time.RTC_Year = 2007;
    RTCSetAlarm( alarm_time );		/* set alarm time */
   
    /* install RTC timer handler mainly for alarm control */
    if ( install_irq( RTC_INT, (void *)RTCHandler, HIGHEST_PRIORITY ) == FALSE )
    {
		while ( 1 );		/* very bad happened */
    }

    /* mask off alarm mask, turn on IMYEAR in the counter increment interrupt
    register */
    RTCSetAlarmMask(AMRSEC|AMRMIN|AMRHOUR|AMRDOM|AMRDOW|AMRDOY|AMRMON|AMRYEAR);
    RTC_CIIR = IMMIN | IMYEAR;
    /* 2007/01/01/00:00:00 is the alarm on */
    
    RTCStart();

    while (1) 
    {					/* Loop forever */
		current_time = RTCGetTime();
		if ( alarm_on != 0 )
		{
			alarm_on = 0;
			/* Get current time when alarm is on */
			current_time = RTCGetTime();	
		}
    }
    return 0;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
