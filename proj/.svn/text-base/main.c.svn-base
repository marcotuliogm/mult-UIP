/**
 * @mainpage Multiplas Aplicacoes com UIP
 *
 * @section sec_intro Introducao
 *
 * MULTIPLOS SERVIÇOS DE INTERNET COM O uIP -
 * DESENVOLVIMENTO DE UMA APLICAÇÃO EMBARCADA PARA REDES TCP/IP
 *
 * Orientador: Marcos Augusto Stemmer
 *
 * Apoio para imrportar o UIP para o LPC2378 - Felipe de Andrade Neves Lavratti
 *
 * Site com o projeto UIP generico: http://www.sics.se/~adam/uip/index.php/Main_Page
 *
 *
 * @section sec_install Compilacao e Instalacao
 *
 * Para Compilar este projeto utilizar o Makefile
 *
 * Bibliotecas Necessarias:
 *
 * 		arm-elf-gcc Compilador C
 *
 * 		arm-elf-g++ Compilador C++
 *
 * 		arm-elf-as Assembler.
 *
 * 		arm-elf-ld Linker/loader
 *
 * 		arm-elf-objcopy Copia convertendo formatos de código objeto
 *
 *
 * 		Para informacoes de como instalar requisitos de software:
 *
 * 			http://www.ee.pucrs.br/~stemmer/labproc/apostila2011/lpc-01.html
 *
 *
 * @section sec_summary Resumo
 *
 * @author Marco Túlio G. Martins
 *
 * @date Sun April 03 14:03:50 BRT 2011
 */
#include "lpc_config.h"

#include "uart/uart.h"
#include "timer/timer.h"
#include "rtc/rtc.h"
#include "eth/eth.h"
#include "i2c/i2c.h"
#include "lcd/lcd.h"
#include "cli/cli.h"
#include "sft/grlib/grlib.h"
#include "mult/api_services.h"
#include <stdio.h>

/* Function prototypes */
void PLLInit( void );
void GPIOInit(void);
void LCDbcd(int val);


extern volatile int I2CCount;
extern volatile char I2CMasterBuffer[BUFSIZE];
extern volatile int I2CCmd, I2CMasterState;
extern volatile int I2CReadLength, I2CWriteLength;

int read_temp(void)
{
  int i;


  for ( i = 0; i < BUFSIZE; i++ )	/* clear buffer */
  {
	I2CMasterBuffer[i] = 0;
  }
  I2CWriteLength = 2;
  I2CReadLength = 0;
  I2CMasterBuffer[0] = LM75_ADDR;
  I2CMasterBuffer[1] = LM75_CONFIG;
  I2CMasterBuffer[2] = 0x00;

  I2CCmd = LM75_CONFIG;
  I2CEngine();

  for ( i = 0; i < BUFSIZE; i++ )
  {
	I2CMasterBuffer[i] = 0;
  }
  I2CWriteLength = 1;
  I2CReadLength = 2;
  I2CMasterBuffer[0] = LM75_ADDR;
  I2CMasterBuffer[1] = LM75_TEMP;
  I2CMasterBuffer[2] = LM75_ADDR | RD_BIT;
  I2CCmd = LM75_TEMP;
  I2CEngine();

  /* The temp reading value should reside in I2CMasterBuffer char 3, 4, ... */
  return (I2CMasterBuffer[3] << 8 | I2CMasterBuffer[4]);
}

void LigaAC()
{
//	Liga 1.23
//	PINSEL3 &= ~(3<<14); //1.23 GPIO
//	PINMODE3 |= 3<<14;   //1.23 Pull Down
	FIO1DIR |= (1<<23); //1.23 Output
	FIO1SET = 1<<23;     //1.23 ON
}
void DesligaAC()
{
//	Desliga 1.23
//	PINSEL3 &= ~(3<<14); //1.23 GPIO
//	PINMODE3 |= 3<<14;   //1.23 Pull Down
	FIO1DIR |= (1<<23); //1.23 Output
	FIO1CLR = 1<<23;     //1.23 OFF
}



RTCTime localtime;
unsigned char temp0, temp1, setpoint, ac_auto_on, ac_modo;

int main( void )
{
	int i;
	int temp;
	char  str[20];
	char buff[MAX_TAM_SIZE_STRING];
	char dest[MAX_COMMAND_SIZE];
	str_t *tp;
	RTCTime temp_time;
	struct txtinfo stxt;
	PLLInit();
	GPIOInit();

    RTCInit();
    RTCStart();

    UARTInit(115200,UART_CH_0,0);


 	uart_puts("\nDelay test: ");
 	delayMs(1000);
 	uart_putc('.');
 	delayMs(1000);
 	uart_putc('.');
 	uart_puts(" Ok\n");

   	uart_puts("RTC test: ..");
   	temp_time = RTCGetTime();
   	RTCClearTime();
   	delayMs(1000);
  	temp_time = RTCGetTime();
   	uart_puts(" Ok\n");

   	T0Init(1000);
   	T0Start();
   	uart_puts("Timer test: ");
   	delayMs(1000);
   	uart_puts(" Ok\n");

 	uart_puts("Init lcd ..");
 	init_lcd();
 	lcd_clr();
 	lcd_goto(1,0);
 	lcd_string("UIP Aplication");
 	uart_puts("Ok");

   	uart_puts("\nETH...");
   	ethernet_init();
   	uart_puts(" Ok");


 	uart_puts("\nI2C...");
 	temp = I2CInit( (int)I2CMASTER );
 	if (temp!=TRUE) uart_puts("erro");
 	uart_puts("initd...");
 	temp=read_temp();
	temp1 = temp>>8;
	temp0 = temp&0xf;
 	sprintf(str, " %d,%dC ",temp1,temp0 );
 	uart_puts(str);
 	uart_puts("Ok");

    uart_puts("\nNokia LCD init.. ");
    lcd_nokia_init(&stxt);	/* Inicializa display grafico (fundo branco) */
    uart_puts(" ok\n");


 	do_init(tp);
 	uart_puts(tp->resp);
 	uart_puts(CLI_PROMPT);
 	UARTTakeString(dest, UART_CH_0);
 	CLI_Builder(dest, buff);
 	uart_puts(buff);

	while(1)
    {
		fflush(stdout);

		/* Escreve hora:minuto:segundo no LCD de texto */
		lcd_goto(2,0);
		LCDbcd(RTC_HOUR);
		lcd_putc(':');
		LCDbcd(RTC_MIN);
		lcd_putc(':');
		LCDbcd(RTC_SEC);

#if 0
		if(global_chat_flag==0){//se em 1 significa que uart esta dedicada para atender o chat.
			if(UARTTakeStringAsync(dest, UART_CH_0) == 0){
				if (strlen(dest)>2){
				CLI_Builder(dest,buff);
				uart_puts(buff);
				memset(dest, '\0', sizeof(dest));

				if(global_chat_flag==0) uart_puts(CLI_PROMPT);
				}
			}
		}

#endif

        if(global_chat_flag!=1){//se em 1 significa que uart esta dedivada para atender o chat.
                uart_puts(CLI_PROMPT);
                UARTTakeString(dest, UART_CH_0);
                if (strlen(dest)>2){
                CLI_Builder(dest,buff);
                uart_puts(buff);
                memset(dest, '\0', sizeof(dest));
                }
        }



  	}



	return 0;
}


void PLLInit( void )
{
	int i;
	#ifdef RUN_FROM_RAM
		/* Remap the interrupt vectors to RAM if we are are running from RAM. */
		SCB_MEMMAP = 2;
	#endif

	/* Disable the PLL. */
	PLLCON = 0;
	PLLFEED = PLL_FEED_BYTE1;
	PLLFEED = PLL_FEED_BYTE2;

	/* Configure clock source. */
	SCS |= OSC_ENABLE;
	while( !( SCS & OSC_STAT ) );
	CLKSRCSEL = OSC_SELECT;

	/* Setup the PLL to multiply the XTAL input by 8. */
	PLLCFG = ( PLL_MUL | PLL_DIV );
	PLLFEED = PLL_FEED_BYTE1;
	PLLFEED = PLL_FEED_BYTE2;

	/* Turn on and wait for the PLL to lock... */
	PLLCON = PLL_ENABLE;
	PLLFEED = PLL_FEED_BYTE1;
	PLLFEED = PLL_FEED_BYTE2;
	/* Configurate CPU clock to divide PLL by 4... */
	CCLKCFG = CPU_CLK_DIV;
	while( !( PLLSTAT & PLL_LOCK ) );

	/* Connecting the clock. */
	PLLCON = PLL_CONNECT;
	PLLFEED = PLL_FEED_BYTE1;
	PLLFEED = PLL_FEED_BYTE2;
	while( !( PLLSTAT & PLL_CONNECTED ) );

	MAMCR = 0;
	MAMTIM = 3;
	MAMCR = 2;

}
void GPIOInit(void)
{
	// Set to inputs
	IODIR0  = 0;
	IODIR1  = 0;
	FIO0DIR = 0;
	FIO1DIR = 0;
	FIO2DIR = 0;
	FIO3DIR = 0;
	FIO4DIR = 0;

	// Enable Fast GPIO
	SCS|=0x01;

	// clear mask registers
	FIO0MASK =0;
	FIO1MASK =0;
	FIO2MASK =0;
	FIO3MASK =0;
	FIO4MASK = 0;

	// Reset all GPIO pins to default primary function
	PINSEL0 =0;
	PINSEL1 =0;
	PINSEL2 =0;
	PINSEL3 =0;
	PINSEL4 =0;
	PINSEL5 =0;
	PINSEL6 =0;
	PINSEL7 =0;
	PINSEL8 =0;
	PINSEL9 =0;
	PINSEL10= 0;
	PINSEL11= 0;

//	PCONP = 0xC00006; //All timers on only
}

void DABT_Routine()
{
	register unsigned long *r14 asm("%lr");
	unsigned long reg;
	char str[50];
	reg = (unsigned long)r14;
	sprintf(str, "\nData Abort Exception at 0x%x : OPCODE=0x%x", reg-8, *(unsigned long*)(reg-8));
	uart_puts(str);
	disableIRQ();
	while(1);
}

void PABT_Routine()
{
	register unsigned long *r14 asm("%r14");
	unsigned long reg;
	char str[50];
	reg = (unsigned long)r14;
	sprintf(str, "\nPrefetch Abort Exception at 0x%x", reg-4);
	uart_puts(str);
	disableIRQ();
	while(1);
}

void LCDbcd(int val)
{
	lcd_putc((val / 10)  + '0');
	lcd_putc((val % 10) + '0');
}


void SWI_Routine(){	uart_puts("SWI"); disableIRQ();	while(1);}
void FIQ_Routine(){	uart_puts("FIQ"); disableIRQ();	while(1);}
void UNDF_Routine(){ uart_puts("UNDEF"); disableIRQ();	while(1);}


