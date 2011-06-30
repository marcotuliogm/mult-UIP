/**
 * @file i2c_isr.c
 *
 * @brief Funcoes para uso do I2C
 *
 *
 * @author Felipe de Andrade Neves Lavratti
 */


/*
 * i2c_isr.c
 *
 *  Created on: 07/01/2011
 *      Author: fanl
 */

#include "../lpc_config.h"
#include "i2c.h"
#include "../uart/uart.h"

extern volatile int I2CMasterState;
extern volatile int I2CCmd;
extern volatile char I2CMasterBuffer[BUFSIZE];
extern volatile int I2CReadLength;
extern volatile int I2CWriteLength;
extern volatile int RdIndex;
extern volatile int WrIndex;


void I2C0MasterHandler( void ) __attribute__ ((interrupt("IRQ")));

/*****************************************************************************
** Function name:		I2C0MasterHandler
**
** Descriptions:		I2C0 interrupt handler, deal with master mode
**				only.
**
** parameters:			None
** Returned value:		None
**
*****************************************************************************/
void I2C0MasterHandler(void)
{
  char StatValue;
//  uart_putc('i');
  /* this handler deals with master read and master write only */
  StatValue = I22STAT;
  switch ( StatValue )
  {
	case 0x08:			/* A Start condition is issued. */
	I22DAT = I2CMasterBuffer[0];
	I22CONCLR = (I2CONCLR_SIC | I2CONCLR_STAC);
	I2CMasterState = I2C_STARTED;
	break;

	case 0x10:			/* A repeated started is issued */
	if ( I2CCmd == LM75_TEMP )
	{
	  I22DAT = I2CMasterBuffer[2];
	}
	I22CONCLR = (I2CONCLR_SIC | I2CONCLR_STAC);
	I2CMasterState = I2C_RESTARTED;
	break;

	case 0x18:			/* Regardless, it's a ACK */
	if ( I2CMasterState == I2C_STARTED )
	{
	  I22DAT = I2CMasterBuffer[1+WrIndex];
	  WrIndex++;
	  I2CMasterState = DATA_ACK;
	}
	I22CONCLR = I2CONCLR_SIC;
	break;

	case 0x28:	/* Data char has been transmitted, regardless ACK or NACK */
	case 0x30:
	if ( WrIndex != I2CWriteLength )
	{
	  I22DAT = I2CMasterBuffer[1+WrIndex]; /* this should be the last one */
	  WrIndex++;
	  if ( WrIndex != I2CWriteLength )
	  {
		I2CMasterState = DATA_ACK;
	  }
	  else
	  {
		I2CMasterState = DATA_NACK;
		if ( I2CReadLength != 0 )
		{
		  I22CONSET = I2CONSET_STA;	/* Set Repeated-start flag */
		  I2CMasterState = I2C_REPEATED_START;
		}
	  }
	}
	else
	{
	  if ( I2CReadLength != 0 )
	  {
		I22CONSET = I2CONSET_STA;	/* Set Repeated-start flag */
		I2CMasterState = I2C_REPEATED_START;
	  }
	  else
	  {
		I2CMasterState = DATA_NACK;
		I22CONSET = I2CONSET_STO;      /* Set Stop flag */
	  }
	}
	I22CONCLR = I2CONCLR_SIC;
	break;

	case 0x40:	/* Master Receive, SLA_R has been sent */
	I22CONSET = I2CONSET_AA;	/* assert ACK after data is received */
	I22CONCLR = I2CONCLR_SIC;
	break;

	case 0x50:	/* Data char has been received, regardless following ACK or NACK */
	case 0x58:
	I2CMasterBuffer[3+RdIndex] = I22DAT;
	RdIndex++;
	if ( RdIndex != I2CReadLength )
	{
	  I2CMasterState = DATA_ACK;
	}
	else
	{
	  RdIndex = 0;
	  I2CMasterState = DATA_NACK;
	}
	I22CONSET = I2CONSET_AA;	/* assert ACK after data is received */
	I22CONCLR = I2CONCLR_SIC;
	break;

	case 0x20:		/* regardless, it's a NACK */
	case 0x48:
	I22CONCLR = I2CONCLR_SIC;
	I2CMasterState = DATA_NACK;
	break;

	case 0x38:		/* Arbitration lost, in this example, we don't
					deal with multiple master situation */
	default:
	I22CONCLR = I2CONCLR_SIC;
	break;
  }
  VICVectAddr = 0;		/* Acknowledge Interrupt */
}
