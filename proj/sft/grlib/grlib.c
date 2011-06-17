/**
 * @file grlib.c
 *
 * @author Marcos Augusto Stemmer
 *
 * @brief Funcoes usadas pelo display da Nokia.
 *
 * Biblioteca grafica para display Nokia6100
 *
 * Fontes de informacao:
 *
 * http://www.sparkfun.com/tutorial/Nokia 6100 LCD Display Driver.pdf
 * http://www.nxp.com/acrobat_download2/datasheets/PCF8833_1.pdf
 *
 * P0.9	MOSI-1	DIO Dados SPI1
 *
 * P0.8	DRE	Reset
 *
 * P0.7	DCL	Clock
 *
 * P0.6	DCS	Chip Select
 *
*/
#include <arch/nxp/lpc23xx.h>
#include "grlib.h"
#include "charset.c"
struct txtinfo *ptxt;


/*
Em alguns displays as cores e a coordenada X sao invertidas
Comente esta linha se as cores parecerem erradas */
#define INVERTE
/* espera t milisegundos (usa timer T1) */
void espera_ms(unsigned t)
{
t += T1TC;
while(t != T1TC);
}

void spiconfig(void)
{
SSP1CR1 = 0;	// Conigura desabilitado
// Divide por 1 no SSP1-PCLK
PCLKSEL0 = (PCLKSEL0 & (~(3<<20))) | (0<<20);
// Seleciona pinos p0.9=MOSI1; P0.7=SCLK1;  P0.6=SSEL1
PINSEL0 = (PINSEL0 & (~0x000ff000)) | 0x8a000;
PCONP |= (1 << 10);		// Liga ssp1
SSP1CR0 = 8 + (0x0 << 6);	// Configura SPI de 9 bits
SSP1CPSR = 4;	// Divide PCLK por 4
SSP1CR1 = 2;	// Habilita SSP1 como SPI
}

/* Rotina para escrever um byte na interface SPI */
void WrSPI(int c)
{
while((SSP1SR & 2) == 0);	// Wait while FIFO full
SSP1DR = c;
}

void lcd_nokia_init()
{
int k;
spiconfig();
T1TCR = 0;	// T1 em 1000 contagens por segundo
T1PR = 12000000/10000 - 1;
T1TCR = 2;
T1TCR = 1;
FIO0DIR |= 0x100;
FIO0CLR = 0x100;	// Reset no PCF8833
espera_ms(10);		// Pulso de 10ms
FIO0SET = 0x100;
espera_ms(10);
WriteSpiCommand(0x11);	// SLEEPOUT
WriteSpiCommand(0x3a);	// COLMOD (Modo de colorido)
WriteSpiData(0x03);	// 12 bits
// Dependendo do modelo de display estas configuracoes devem mudar
WriteSpiCommand(0x36);	// MADCTL
#ifdef INVERTE
WriteSpiData(0x48);	// 0x40: Xmirror;  0x08: bgr [1-bgr 0-rgb]
#else
WriteSpiData(0x0);	// no mirror - rgb
WriteSpiCommand(0x20);	// INVON
#endif
WriteSpiCommand(0x25);	// SETCON Ajusta contraste
WriteSpiData(0x38);	// Metade do maximo
WriteSpiCommand(0x29);	// DISPON
WriteSpiCommand(0x2b);	// PASET Linha inicial e final
WriteSpiData(0);
WriteSpiData(131);
WriteSpiCommand(0x2a);	// CASET Coluna inicial e final
WriteSpiData(0);
WriteSpiData(131);
WriteSpiCommand(0x2c);	// Write Memory
/* Pinta tudo de branco */
for(k=0; k < 26136;){
	WriteSpiData(0xff); k++;
	WriteSpiData(0xff); k++;
	WriteSpiData(0xff); k++;
	}
}




///Funções para configuração das mensagens para aparecer no display.

void inline txtattrib(struct txtinfo *pptxt)
{
ptxt = pptxt;
}



/* \brief Escreve um caractere no display grafico */
void lcdgr_putchar(int c)
{
int x,y, bc, cbg, cfg;
WriteSpiCommand(0x2b);	// PASET Linha inicial e final
WriteSpiData(ptxt->linha);
WriteSpiData(ptxt->linha+7);
WriteSpiCommand(0x2a);	// CASET Coluna inicial e final
WriteSpiData(ptxt->coluna);
WriteSpiData(ptxt->coluna+7);
WriteSpiCommand(0x2c);	// Write Memory
cfg = ptxt->corfrente;
cbg = ptxt->corfundo;
ptxt->coluna += 8;
if((ptxt->coluna > 128) || (c == '\n') ) {
	ptxt->coluna = 0;
	ptxt->linha += 12;
	if(ptxt->linha > 128) ptxt->linha = 0;
	return;
	}
if(c < 0x20) return;
for(y = 0; y < 8; y++) {
	bc = rom8x8_bits[8*(c-0x20) + y];
	for(x=4; x--; bc <<=2) {
		switch(bc & 0xc0){
		case 0x00:
			WriteSpiData(cbg >> 4);
			WriteSpiData(((cbg << 4) | (cbg >> 4)) & 0xff);
			WriteSpiData(cbg & 0xff);
			break;
		case 0x40:
			WriteSpiData(cbg >> 4);
			WriteSpiData(((cbg << 4) | (cfg >> 4)) & 0xff);
			WriteSpiData(cfg & 0xff);
			break;
		case 0x80:
			WriteSpiData(cfg >> 4);
			WriteSpiData(((cfg << 4) | (cbg >> 4)) & 0xff);
			WriteSpiData(cbg & 0xff);
			break;
		case 0xc0:
			WriteSpiData(cfg >> 4);
			WriteSpiData(((cfg << 4) | (cfg >> 4)) & 0xff);
			WriteSpiData(cfg & 0xff);
			break;
			}
		}
	}
}

void lcdgr_puts(char *str)
{
while(*str) lcdgr_putchar(*str++);
}


void setPixel(int x, int y)
{
/* Se o bit 6 (mascara 0x4000) da cor for 1 desenha com linha grossa */
if(ptxt->corfrente & 0x4000) {
	WriteSpiCommand(0x2b);	// PASET Linha inicial e final
	WriteSpiData(y);
	WriteSpiData(y+1);
	WriteSpiCommand(0x2a);	// CASET coluna inicial e final
	WriteSpiData(x);
	WriteSpiData(x+1);
	WriteSpiCommand(0x2c);	// Write Memory
	WriteSpiData(ptxt->corfrente >> 4);
	WriteSpiData((ptxt->corfrente << 4) | ((ptxt->corfrente >> 8) & 15));
	WriteSpiData(ptxt->corfrente);
	WriteSpiData(ptxt->corfrente >> 4);
	WriteSpiData((ptxt->corfrente << 4) | ((ptxt->corfrente >> 8) & 15));
	WriteSpiData(ptxt->corfrente);
	return;
	}
WriteSpiCommand(0x2b);	// PASET Linha inicial e final
WriteSpiData(y);
WriteSpiData(y);
WriteSpiCommand(0x2a);	// CASET coluna inicial e final
WriteSpiData(x);
WriteSpiData(x);
WriteSpiCommand(0x2c);	// Write Memory
WriteSpiData(ptxt->corfrente >> 4);
WriteSpiData((ptxt->corfrente << 4) & 0xf0);
WriteSpiCommand(0);	// NOP
}


