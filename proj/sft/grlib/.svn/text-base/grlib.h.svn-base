/**
 * @file grlib.h
 *
 * @author Marcos Augusto Stemmer
 *
 * @brief FArquivo header de funcoes graficas no display
 *
 * com o msp430
 *
 * FENG-PUCRS Laboratorio de Processadores I
 *
 * http://www.sparkfun.com/tutorial/Nokia 6100 LCD Display Driver.pdf
 * http://www.nxp.com/acrobat_download2/datasheets/PCF8833_1.pdf
 *
*/
/** \brief Em struct txtinfo tem os atributos do texto */
struct txtinfo {
	int corfundo;
	int corfrente;
	int linha;
	int coluna;
	};

void WrSPI(int c);
void lcd_nokia_init();
void espera_ms(unsigned t);

#define WriteSpiCommand(c) WrSPI((c) & 0xff)
#define WriteSpiData(c) WrSPI((c)|0x100)
/* No display sem nada escrito deve definir INVERTE */
//#define INVERTE


/* Funcoes para escrever um caractere no display grafico */

void lcdgr_putchar(int c);
void lcdgr_puts(char *str);
void setPixel(int x, int y);
void txtattrib(struct txtinfo *ptxt);

