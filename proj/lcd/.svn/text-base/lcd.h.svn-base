/*
 * lcd.h
 *
 *  Created on: Feb 8, 2011
 *      Author: MTGM
 *
 *      Controle do Display de LCD.
 */

#ifndef LCD_H_
#define LCD_H_

//#include<LPC23xx.H>
#include "../lpc_config.h"
#include<stdio.h>
#include "../timer/timer.h"


#define lcd_dados 0xFF
#define lcd_en (1<<24)
#define lcd_rs (1<<25)
#define lcd_rw (1<<30)


void lcd_command(int comando);//lcd_comando
void lcd_putc(int dado);//lcd_dado
void lcd_string(char *dado);//lcd_string
void lcd_goto(int linha, int coluna);
void init_lcd(void);
void lcd_clr(void);

#endif /* LCD_H_ */
