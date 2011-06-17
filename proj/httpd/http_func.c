/**
 * @file http_func.c
 *
 * @brief Atende requisicoes feitas na pagina HTTP do WebServer.
 *
 * @author Marco Tulio Martins
 */

#include "http_func.h"

/**
 * @param c contem string vinda do formulário
 * posição 0 para linha e coluna, default
 */
void http_set_lcd(char * c)
{
	writeDataLcd(c,0,0);
}
