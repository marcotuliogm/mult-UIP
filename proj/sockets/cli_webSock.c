/**
 * \addtogroup apps
 * @{
 */

/**
 * @file chat.c
 *
 * @brief comunicação entre dois ambientes com sockets.
 *
 * Para executar o chat:
 *  primeiro deve configurar ele no arm, depois chamar o sistema que fara troca de mensagem.
 *
 * @author Marco Tulio Martins
 *
 * @defgroup CHAT CHAT usando sockets
 * @{

 */

#include "cli_webSock.h"
#include "../uart/uart.h"
#include <string.h>
#include <stdio.h>
#include "uip.h"
#include "psock.h"
//#include "uip-conf.h"

char cabec_or[20];	/** \brief cabecalho origem do chat. */
char cabec_dest[20];	/** \brief cabecalho destino do chat. */

/**
 * \brief Finaliza a comunicacao com sockets.
 */
void chat_end(struct chat_state *s);

/**
 * \brief Inicializa o servico de socket de acordo com a porta.
 */
void chat_init(){
	uip_listen(HTONS(CHAT_PORT));
}

/**
 * \brief Coloca flag de inicio de comunicacao no chat em 1
 *
 * Inicialia o processo de comunicacao usado pelo chat.
 * Coloca a saida serial fora do ar para a CLI e dedicada para o chat.
 */
void chat_start(){
	//conv com a UART. main do chat.

	sprintf(cabec_or, "%d-CHAT>", CHAT_PORT);
	sprintf(cabec_dest, ":%d->", CHAT_PORT);

	chat_init();
	//uart_puts("\nBem Vindo ao chat.\n");
	//uart_puts(cabec_or);

}

/**
 * \brief Funcao que gerencia conexao do chat.
 *
 * Monitora se e uma nova conexao, ou passa a informacao para ser tratada.
 */
void chat_appcall(void){
	char * dest;
	int tambuff;

	struct chat_state *s = (struct chat_state *)&(uip_conn->appstate.chat_st);


	//quando uma nova conexão é estabelecida.
	if(uip_connected()){
		DEBUG_SOCK_INFO("try connection\n");
		PSOCK_INIT(&prot, s->inbuff, sizeof(s->inbuff));
		DEBUG_SOCK_INFO("connection .. ok");
		s->state = SEND_CHAT;
	}

//	DEBUG_SOCK_INFO("Iniciando comunicacao.\n");
	chat_communication(s);

}


/**
 * \brief Funcao para gerenciar os estados da conexao do chat
 *
 * Dois estados habilitados no chat:
 * 	RECV_CHAT - quando esta aberto para receber dados da socket conectada a ele
 *
 * 	SEND_CHAT - pega informacao digitada na serial e envia para ele.
 */
int chat_communication(struct chat_state *s){
	char *dest;
	char estado[20];
	int a;
	sprintf(estado,"Estado: %d\n", s->state);
	DEBUG_INFO(estado);
	PSOCK_BEGIN(&prot);

	if(s->state==RECV_CHAT){
//			PSOCK_WAIT_UNTIL(&prot,PSOCK_NEWDATA(&prot));//{	//verifica se tem dados armazenado.
				DEBUG_SOCK_INFO("recebendo dados.\n");
				strcpy(s->inbuff,"\n");
				uart_puts(cabec_dest);	//monta cabecalho para origem (placa) escrever.
				PSOCK_READTO(&prot, '\n');//fica bloqueado esperando informação até que venha o caracter \n
//				strncpy(dest, s->inbuff, strlen(s->inbuff));
				dest = strtok(s->inbuff, "\n");
				uart_puts(dest);	//envia para a UART os dados que vieram.
				uart_putc('\n');

				if(!strcmp(dest,"quit")){
					s->state = QUIT_CHAT;
				}
				else {
					s->state = SEND_CHAT;
					memset(s->inbuff,'\0',strlen(s->inbuff));
				}
	//		}
	}
	//envia dados por sockets;
	if(s->state == SEND_CHAT){
		uart_puts(cabec_or);	//monta cabecalho para origem (placa) escrever.
		//do{
			UARTTakeString(s->buff, UART_CH_0);	//pega dados da uart
		//}while(strlen(s->buff)<2);//consistencia simples.
		if(!strcmp(s->buff,"quit")){
			//PSOCK_SEND_STR(&prot, "bye!!!");	//envia para a socket.
			global_chat_flag = 0;	//verifica se é para sair do chat
			s->state = QUIT_CHAT;
		}
		else {
			DEBUG_SOCK_INFO("enviando dados\n");
			PSOCK_SEND_STR(&prot, s->buff);	//envia para a socket.
			DEBUG_SOCK_INFO("dados enviados\n");
			s->state = RECV_CHAT;
		}
	}

	if(s->state == QUIT_CHAT){
		uart_puts("bye.\n");
		PSOCK_SEND_STR(&prot, "quit");	//envia para a socket.
		global_chat_flag = 0;	//verifica se é para sair do chat
		DEBUG_SOCK_INFO("socket finalizada.\n");
		PSOCK_CLOSE(&prot);
		PSOCK_CLOSE_EXIT(&prot);
		uip_unlisten(CHAT_PORT);
		s->state = 10;//estado nao tratado.
		fflush(stdout);
//		return 0;
	}
	PSOCK_END(&prot);

	return 0;
}



/** @} */
/** @} */

