/*
 * uIP_Task.h
 *
 *  Created on: 06/01/2011
 *      Author: fanl
 */

#ifndef UIP_TASK_H_
#define UIP_TASK_H_


/* How long to wait before attempting to connect the MAC again. */
#define uipINIT_WAIT    100

/* Shortcut to the header within the Rx buffer. */
#define xHeader ((struct uip_eth_hdr *) &uip_buf[ 0 ])

/* Standard constant. */
#define uipTOTAL_FRAME_HEADER_SIZE	54


void ethernet_init();
void ethernet_task();
void set_eth_addr(void);

#endif /* UIP_TASK_H_ */
