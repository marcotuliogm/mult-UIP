/**
 * @file cli_websock.h
 *
 * @brief Declaracao e definicoes usadas pelo CHAT usando socket TCP.
 *
 *
 *
 * @author Marco Tulio Martins
 */


#ifndef CLI_WEBSOCK_H_
#define CLI_WEBSOCK_H_


#include <stdio.h>
#include <string.h>

#include "uipopt.h"

#include "../uart/uart.h"
#include "../cli/debug.h"
#include "../mult/api_services.h"

typedef enum {
	SEND_CHAT = 0,
	RECV_CHAT = 1,
	WAITING_CHAT = 2,
	QUIT_CHAT = 3
} state_t;


/**
 * \brief chat_state Campos usados par gerenciar CHAT.
 */
struct chat_state{
	state_t state;
	char buff[100];
	int bufflen;
	char inbuff[200];
};

struct psock prot;

void chat_init();
void chat_start();
void chat_appcall(void);
int chat_communication(struct chat_state *s);

#endif /* CLI_WEBSOCK_H_ */
