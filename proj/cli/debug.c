#include "../lpc_config.h"
#include "../uart/uart.h"
#include "../rtc/rtc.h"
#include <stdio.h>
//#include "debug.h"



//
//void PRT_ERROR(msg_error erro){
//	char buff[50];
//	sprintf(buff, "\n%s\n",error_cli[erro]);
//	uart_puts(buff);
//}
//
////chamada de função para depuração do modulos da CLI.
//void debug_cli(char *data){
//	char buff[400];
//	sprintf(buff, "\n%02d:%02d:%02d<DEBUG_CLI>:%s",RTC_HOUR, RTC_MIN, RTC_SEC, data);
//	(DEBUG_CLI) ? uart_puts(buff) :	0;
//}
//
//
////chamada de função para depuração das funcionalidades aplicadas a CLI.
//void debug_func(char *data){
//	char buff[400];
//	sprintf(buff, "\n%02d:%02d:%02d<DEBUG_FUNCTION_CLI>:%s",RTC_HOUR, RTC_MIN, RTC_SEC, data);
//	(DEBUG_FUNCTION_CLI) ? uart_puts(buff) :	0;
//}

