/**
 * @file lcd.c
 *
 * @brief Configuracao para uso do LCD 16x2
 *
 * @author Marco Tulio Martins
 *
 */

/**
==================================================================================
** Funções LCD
==================================================================================
**/
#include "lcd.h"
#include <string.h>


/**
 * @brief Executa um comando passado por parametro.
 *
 * @param comando Comando para ser enviado para o display.
 */
void lcd_command(int comando)	  //executa comando.
{
	  FIO4CLR = lcd_rs;
	  FIO3CLR = lcd_dados;//LIMPA OS DADOS
	  FIO3SET = comando;
	  FIO4SET = lcd_en;	//COLOC O ENABLE EM 1
	  //espera_1ms(1);   //AGUARDA UM SEGUNDO.
	  delay_1us(1);
	  FIO4CLR = lcd_en;	//LIMPA O ENABLE. EN =0;
	  //espera_1ms(1);	 //AGUARDA UM SEGUNDO
	  delay_1us(1);
}

/**
 * @brief Escreve um caracter no display.
 *
 * \param dado Caracter a ser enviado para o display.
 */
void lcd_putc(int dado)
{
	  FIO4SET = lcd_rs;
	  FIO3CLR = lcd_dados;//LIMPA OS DADOS
	  FIO3SET = dado;
	  FIO4SET = lcd_en;
	  //espera_1ms(1);   //AGUARDA UM SEGUNDO.
	  delay_1us(1);
	  FIO4CLR = lcd_en;
	  //espera_1ms(1);	 //AGUARDA UM SEGUNDO
	  delay_1us(1);
}

/**
 * @brief Limpa todos os dados do display.
 *
 */
void lcd_clr(void)
{
	lcd_command(0x01);
}

/**
 * \brief Escreve uma string de dados passados por parametro.
 *
 * \param str String como a mensagem a ser transmitida.
 */
void lcd_string(char *str)
{
	while(*str) lcd_putc(*str++);
}


void lcd_string_old(char *str)
{
	//while(*str) lcd_putc(*str++);
	int qtd, i=0;

	qtd = strlen(str);
	for(i=0;i<qtd;i++)
	{
	   lcd_putc(str[i]);
	}

}

/**
 * \brief Posiciona o cursorno display.
 *
 * \param linha Linha onde o curso e para ficar.
 * \param coluna Coluna onde o curso tem que estar.
 */
void lcd_goto(int linha, int coluna)
{
	//0x80 a 0x8f - para acessar a primeira linhas e as colunas de 0 a f.
	//0xC0 a 0xcf - para acessar a segunda linhae as colunas de 0 a f.
	if(linha <=1) lcd_command(0x80 + coluna);
	else lcd_command(0XC0 + coluna);
}

/**
 * \brief Inicialia o display de lcd
 */
void init_lcd(void)
{
	FIO4DIR = lcd_en + lcd_rs + lcd_rw;	//DIR -> TODOS OS BITS EM UM, SIGNIFICA A PORTA ONDE VAI ESTAR MEXENDO;
	FIO3DIR = lcd_dados;
    FIO4CLR = lcd_en + lcd_rw; //configura inicializa os comandos em zero
    delay_1us(1);

	lcd_command(0x38);
	lcd_command(0x0e);
	lcd_command(0x06);
	lcd_command(0x01);
}




