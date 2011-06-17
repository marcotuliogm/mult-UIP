/**
 * @file debug.h
 *
 * @brief Parametros para configuracao de depuracao do projeto.
 *
 * @author Marco Tulio Martins
 */
#define DEBUG_MAIN 1 	//1 HABILITA  TODOS OS DEBUGS,0 DESABILITA.

#define DEBUG_ENTER_DEF 0 	//1 HABILITA PARA MOSTRAR ERROS DE DEBUG_ENTER E DEBUG_EXIT
#define DEBUG_CLI_T 0		//1 HABILITAR PARA MOSTRAR ERROS DEDEBUG_CLI
#define DEBUG_ERROR_T 0		//1 HABILITAR PARA MOSTRAR ERROS DEDEBUG_ERROR
#define DEBUG_INFO_T 0
#define DEBUG_SOCK_T 0
#define DEBUG_HTTP_T 1 //mostrar configuração de debug do web server
#define DEBUG_FTP_T 0
#define DEBUG_SFT_T 0

#define OUT_UART 1
#define OUT_TELNET 1

#ifndef DEBUG_H_
#define DEBUG_H_


typedef enum {COMMAND_NOT_FOUND =0, TRY_AGAYN, INVALID_DATA,EXEC_COMMAND_ERROR} msg_error;

static const char* error_cli[]=
{
		"COMMAND NOT FOUND",
		"TRY AGAIN",
		"DADOS INVALIDOS",
		"ERRO NA EXECUCAO DO COMANDO",
};

/**
 * @def DEBUG_SFT
 * @brief Mensagem de depuracao do protocolo SFT
 */
#define DEBUG_SFT(msg) \
	do { \
		if (DEBUG_MAIN) \
		if (DEBUG_SFT_T) \
			printf("\n%s - %s(%d) FTP report: %s\n", __FILE__, __FUNCTION__, __LINE__, msg); \
	} while(0)

/**
 * @def DEBUG_FTP
 * @brief Mensagem de depuracao do protocolo FTP Client
 */
#define DEBUG_FTP(msg) \
	do { \
		if (DEBUG_MAIN) \
		if (DEBUG_FTP_T) \
			printf("\n%s - %s(%d) FTP report: %s\n", __FILE__, __FUNCTION__, __LINE__, msg); \
	} while(0)

/**
 * @def DEBUG_HTTP
 * @brief Mensagem de depuracao do protocolo HTTP
 */
#define DEBUG_HTTP(msg) \
	do { \
		if (DEBUG_MAIN) \
		if (DEBUG_HTTP_T) \
			printf("\n%s - %s(%d) HTTP report: %s\n", __FILE__, __FUNCTION__, __LINE__, msg); \
	} while(0)

/**
 * @def DEBUG_ERROR
 * @brief Mensagem de depuracao de erro na execucao do programa.
 */
#define DEBUG_ERROR(msg) \
	do { \
		if (DEBUG_MAIN) \
		if (DEBUG_ERROR_T) \
			printf("\n%s - %s(%d) error: %s\n", __FILE__, __FUNCTION__, __LINE__, error_cli[msg]); \
	} while(0)

/**
 * @def DEBUG_INFO
 * @brief Mensagem de depuracao com informacao do comportamento da aplicacao.
 */
#define DEBUG_INFO(msg) \
	do { \
		if (DEBUG_MAIN) \
		if (DEBUG_INFO_T) \
			printf("%s - %s(%d) action: %s\n", __FILE__, __FUNCTION__, __LINE__, msg); \
	} while(0)

/**
 * @def DEBUG_CLI
 * @brief Mensagem de depuracao da CLI
 */
#define DEBUG_CLI(msg) \
	do { \
		if (DEBUG_MAIN) \
		if (DEBUG_CLI_T) \
			printf("%s - %s(%d) action: %s\n", __FILE__, __FUNCTION__, __LINE__, msg); \
	} while(0)

/**
 * @def DEBUG_SOCK_INFO
 * @brief Mensagem de depuracao do chat
 */
#define DEBUG_SOCK_INFO(msg) \
		do { \
			if (DEBUG_MAIN) \
			if (DEBUG_SOCK_T) \
				printf("%s - %s(%d) action: %s\n", __FILE__, __FUNCTION__, __LINE__, msg); \
		} while(0)

/**
 * @def DEBUG_ENTER
 * @brief Mensagem de depuracao do caminho percorrido pelas funcoes
 */
#define DEBUG_ENTER \
	do { \
		if (DEBUG_MAIN) \
		if (DEBUG_ENTER_DEF) \
			printf("%s - %s(%d): Entering\n", __FILE__, __FUNCTION__, __LINE__); \
	} while(0)

/**
 * @def DEBUG_ENTER
 * @brief Mensagem de depuracao do caminho percorrido pelas funcoes
 */
#define DEBUG_EXIT \
	do { \
		if (DEBUG_MAIN) \
		if (DEBUG_ENTER_DEF) \
			printf("%s - %s(%d): Exiting\n", __FILE__, __FUNCTION__, __LINE__); \
	} while(0)


void debug_cli(char *data);
void debug_func(char *data);
void PRT_ERROR(msg_error erro);

#endif		//DEBUG_H_
