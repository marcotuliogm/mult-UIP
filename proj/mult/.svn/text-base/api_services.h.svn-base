/**
 * @file api_services.h
 *
 * @brief API dos servicos implementados para camada de aplicacao
 *
 * API com as bibliotecas necessarias para os protocolos implementados na camada de
 * aplicacao.
 *
 * Servicos Disponiveis:
 *
 * CLI_WEBSOCKS
 *
 * TELNET
 *
 * HTTP
 *
 * SFT
 *
 * @author Marco Tulio Martins
 *
 */


#ifndef API_SERVICES_H_
#define API_SERVICES_H_

/*
 * INCLUDES DOS SERVIÇOS PARA A CAMADA DE APLICAÇÃO
 * */
/*#include "smtp.h"*/
#include "../sockets/cli_webSock.h"
#include "../telnetd/telnetd.h"
#include "../httpd/httpd.h"
#include "../dhcp/dhcpc.h"
#include "../sft/sftd.h"
/*#include "resolv.h"*/
/*#include "webclient.h"*/


//import project ftp
//#include "../ftp/ftp.h"

//portas...
#define TELNET_PORT 23
#define HTTP_PORT 80
#define FTP_PORT 21
#define SFTD_PORT 45
uint32_t CHAT_PORT;

int global_chat_flag; //flag que verifica se o chat está em funcionamento.

/**
 * \brief Servicos disponiveis para multiplas aplicacoes no UIP
 *
 * Estrutura com os servicos disponiveis na camada de aplicacao.
 * Servicos Disponiveis pelo UIP.
 */
typedef union {
	struct httpd_state * http_st;
	struct telnetd_state telnet_st;
	struct chat_state * chat_st;
	//struct ftp_state * ftp_st;
	struct sftd_state * sftd_st;
}uip_tcp_appstate_t;


#endif /* API_SERVICES_H_ */
