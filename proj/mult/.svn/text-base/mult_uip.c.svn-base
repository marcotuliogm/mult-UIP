/**
 * @file mult_uip.c
 *
 * @brief Camada que Gerencia as multiplas Aplicacoes
 *
 * Gerencia os pacotes vindo da CLI e envia para a devida aplicacao.
 * Controle de servicos é feito pela camada de aplicacao.
 *
 * @author Marco Tulio Martins
 *
 * @defgroup MULT Camada Mult
 * @{

 */



#include <stdio.h>
#include "uip.h"

/**
 * \brief Inicializa os servicos disponiveis pela camada mult.
 */
void services_init(void){
	CHAT_PORT = 5555; //inicializa o chat com uma porta default. que será alterada de acordo com a def do chat.
	global_chat_flag = 0; //chat começa desabilitado, só é habilitado pela cli.
	telnetd_init(); //telnet faz comunicação através da porta 23
	httpd_init(); //http faz comunicação através da porta 80.
	chat_init();	//porta inicial, ela será alterada de acordo com o chat.
	//ftp_init();		//inicializa serviço de ftp
	sftd_init();
}

/**
 * \brief Administra aplicacoes.
 *
 * Principal funcao da camada mult
 *
 * Gerencia os pacotes vindos da rede e de acordo com a porta que esta sendo feito a requisicao
 * e encaminhado para o devido pacote.
 */
void services_appcall(void){

	struct services_state *proc;

//	proc = (struct services_state)uip_conn->appstate;

//	if (uip_connected()){//uip_connected - aloca uma nova conexão.

	//verificação de qual serviço deve ser atendido é feita através da porta utilizada por ele.
	if(uip_conn->lport == HTONS(TELNET_PORT)){
		DEBUG_INFO("Atendendo servicos de TELNET");
		//função tratadora dos serviços de telnet.
		telnetd_appcall();
	}
	else if(uip_conn->lport == HTONS(HTTP_PORT)){
		DEBUG_INFO("Atendendo servicos de HTTP");
		//função tratadora dos serviços do webServer.
		httpd_appcall();
	}
	else if(uip_conn->lport == HTONS(CHAT_PORT)){
		DEBUG_INFO("Atendendo servicos de CHAT");
		//função tratadora dos serviços do chat.
		chat_appcall();
	}
	else if(uip_conn->lport == HTONS(SFTD_PORT)){
		DEBUG_INFO("Atendendo servicos de SFT");
		//função tratadora dos serviços do sft.
		sftd_appcall();
	}

	//IMPLEMENTADO OS SERVIÇO DE FTP APENAS PARA CLIENTE.
	/*
	else if(uip_conn->lport == HTONS(FTP_PORT)){
		DEBUG_INFO("Atendendo servicos de FTP");
		//função tratadora dos serviços do webServer.
		ftp_appcall();
	}
*/

}


/** @} */
