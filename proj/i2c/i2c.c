/*****************************************************************************
 *   i2c.c:  I2C C file for NXP LPC23xx/24xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.07.19  ver 1.00    Prelimnary version, first Release
 *
*****************************************************************************/

#include "i2c.h"
#include "../lpc_config.h"

#include "../uart/uart.h"


volatile int I2CMasterState = I2C_IDLE;
volatile int I2CSlaveState = I2C_IDLE;

volatile int I2CCmd;
volatile int I2CMode;

volatile char I2CMasterBuffer[BUFSIZE];
volatile char I2CSlaveBuffer[BUFSIZE];
volatile int I2CCount = 0;
volatile int I2CReadLength;
volatile int I2CWriteLength;

volatile int RdIndex = 0;
volatile int WrIndex = 0;

/* 
From device to device, the I2C communication protocol may vary, 
in the example below, the protocol uses repeated start to read data from or 
write to the device:
For master read: the sequence is: STA,Addr(W),offset,RE-STA,Addr(r),data...STO 
for master write: the sequence is: STA,Addr(W),length,RE-STA,Addr(w),data...STO
Thus, in state 8, the address is always WRITE. in state 10, the address could 
be READ or WRITE depending on the I2CCmd.
*/   


/*****************************************************************************
** Function name:		I2CStart
**
** Descriptions:		Create I2C start condition, a timeout
**				value is set if the I2C never gets started,
**				and timed out. It's a fatal error. 
**
** parameters:			None
** Returned value:		true or false, return false if timed out
** 
*****************************************************************************/
int I2CStart( void )
{
  int timeout = 0;
  int retVal = FALSE;
 
  /*--- Issue a start condition ---*/
  I22CONSET = I2CONSET_STA;	/* Set Start flag */
  /*--- Wait until START transmitted ---*/
  while( 1 )
  {
	if ( I2CMasterState == I2C_STARTED )
	{
	  retVal = TRUE;
	  break;	
	}
	if ( timeout >= MAX_TIMEOUT )
	{
	  retVal = FALSE;
	  break;
	}
	timeout++;
  }
  return( retVal );
}

/*****************************************************************************
** Function name:		I2CStop
**
** Descriptions:		Set the I2C stop condition, if the routine
**				never exit, it's a fatal bus error.
**
** parameters:			None
** Returned value:		true or never return
** 
*****************************************************************************/
int I2CStop( void )
{
  I22CONSET = I2CONSET_STO;      /* Set Stop flag */
  I22CONCLR = I2CONCLR_SIC;  /* Clear SI flag */
  /*--- Wait for STOP detected ---*/
  while( I22CONSET & I2CONSET_STO );
  return TRUE;
}

/*****************************************************************************
** Function name:		I2CInit
**
** Descriptions:		Initialize I2C controller
**
** parameters:			I2c mode is either MASTER or SLAVE
** Returned value:		true or false, return false if the I2C
**				interrupt handler was not installed correctly
** 
*****************************************************************************/
int I2CInit( int I2cMode )
{
  //I2C2
  PCONP |=   0x04000000;
  PINSEL0 &= 0xffafffff;
  PINSEL0 |= 0x00a00000;

  //I2C0
//  PCONP |=  0x00000080
//  PINSEL1 &= ~0x03C00000;
//  PINSEL1 |= 0x01400000;	/* set PIO0.27 and PIO0.28 to I2C0 SDA and SCK */
							/* function to 01 on both SDA and SCK. */
  /*--- Clear flags ---*/
  I22CONCLR = I2CONCLR_AAC | I2CONCLR_SIC | I2CONCLR_STAC | I2CONCLR_I2ENC;


  /*--- Reset registers ---*/
  I22SCLL   = I2SCLL_SCLL;
  I22SCLH   = I2SCLH_SCLH;
  if ( I2cMode == I2CSLAVE )
  {
	I22ADR = LM75_ADDR;
  }    

  extern void I2C0MasterHandler( void ) __attribute__ ((interrupt("IRQ")));

  disableIRQ();
//  PINSEL0 &= 0xffafffff;
//  PINSEL0 |= 0x00a00000;
  VICIntSelect &= ~0x40000000;	/* i2c2=bit 30 como IRQ	*/
  VICIntEnable = 0x40000000;	/* Habilita int do i2c2 no VIC*/
  VICVectAddr30 = (int)I2C0MasterHandler;	/* Vetor para atendimento do I2C2 */

//  PINSEL1 &= 0xfd7fffff;
//  PINSEL1 |= 0x01400000;
//  VICIntSelect &= ~0x200;	/* i2c0=bit 9 como IRQ	*/
//  VICIntEnable = 0x200;	/* Habilita int do i2c0 no VIC*/
//  VICVectAddr9 = (int)I2C0MasterHandler;	/* Vetor 9 para atendimento do I2C0 */
  enableIRQ();

  I22CONSET = I2CONSET_I2EN;
  return( TRUE );
}


/*****************************************************************************
** Function name:		I2CEngine
**
** Descriptions:		The routine to complete a I2C transaction
**				from start to stop. All the intermitten
**				steps are handled in the interrupt handler.
**				Before this routine is called, the read
**				length, write length, I2C master buffer,
**				and I2C command fields need to be filled.
**				see i2cmst.c for more details. 
**
** parameters:			None
** Returned value:		true or false, return false only if the
**				start condition can never be generated and
**				timed out. 
** 
*****************************************************************************/
int I2CEngine( void )
{
  I2CMasterState = I2C_IDLE;
  RdIndex = 0;
  WrIndex = 0;
  if ( I2CStart() != TRUE )
  {
	I2CStop();
	return ( FALSE );
  }

  while ( 1 )
  {
	if ( I2CMasterState == DATA_NACK )
	{
	  I2CStop();
	  break;
	}
  }    
  return ( TRUE );      
}

/******************************************************************************
**                            End Of File
******************************************************************************/

