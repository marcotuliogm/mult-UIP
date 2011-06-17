#include "../cli/cli_func.h"


/*
 * @brief Escreve dados no lcd
 */
void http_set_lcd(char *c)
{
	DEBUG_HTTP(c);
	writeDataLcd(c,0,0);
}

/**
 * @brief Altera a hora do relogio RTC
 */
void http_set_hour(char *c){
	DEBUG_HTTP(c);
	RTC_HOUR = atoi(c);
}

/**
 * @brief Altera os minutos do relogio RTC
 */
void http_set_min(char *c){
	RTC_MIN = atoi(c);
}

/**
 * @brief Altera ossegundos do relogio RTC
 */
void http_set_sec(char *c){
	DEBUG_HTTP(c);
	RTC_SEC = atoi(c);
}
