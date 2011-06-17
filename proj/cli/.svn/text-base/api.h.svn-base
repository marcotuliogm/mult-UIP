/**
 * @file api.h
 *
 * @brief API com defines essenciais para a CLI.
 *
 * @author Marco Tulio Martins
 */

/**
 * @def TAM_STRING
 *
 * @brief tamanho das strings usado na CLI
 */
#define TAM_STRING 30
#define TAM_STRING_LONG 100
#define MAX_COMMAND_SIZE 150
#define MAX_RESP_SIZE 200

#define MAX_TAM_SIZE_STRING 2000


typedef unsigned char	U08_t;
typedef unsigned long	U32_t;
typedef unsigned long long U64_t;

/**
 * @def CLI_PROMPT
 *
 * @brief Chamada do terminal
 *
 * Nome do comando de entrada do terminal.
 */
#define CLI_PROMPT "\nTCCOS> "

/* MAC address configuration. */
#define uipMAC_ADDR0	0x0
#define uipMAC_ADDR1	0xbd
#define uipMAC_ADDR2	0x33
#define uipMAC_ADDR3	0x02
#define uipMAC_ADDR4	0x64
#define uipMAC_ADDR5	0x24

/* IP address configuration. */
int uipIP_ADDR0;
int uipIP_ADDR1;
int uipIP_ADDR2;
int uipIP_ADDR3;

unsigned short uip_GAT0;
unsigned short uip_GAT1;
unsigned short uip_GAT2;
unsigned short uip_GAT3;

unsigned short uip_Mask0;
unsigned short uip_Mask1;
unsigned short uip_Mask2;
unsigned short uip_Mask3;


/**
 * @brief tipo break_t
 *
 * Usado para o retorna das funcoes da CLI.
 *
 */
typedef enum {
	RET_OK =0,	/** @var RET_OK retorna 0 quando funcao foi executada corretamente*/
	RET_ERROR	/** @var RET_ERROR retorna 1 quando ocorreu algum erro na funcao*/
} break_t;

/**
 * @brief type_t
 *
 * Configura o tipo de comando solicitado pela CLI
 *
 */
typedef enum {
	GET_CMD=0, /** @var GET_CMD comandos do tipo GET */
	SET_CMD, /** @var SET_CMD comandos do tipo SET */
	HELP_CMD, /** @var HELP_CMD comandos de HELP */
	CHAT_CMD, /** @var CHAT_CMD comandos de CHAT */
	FTP_CMD /** @var FTP_CMD comandos do tipo FTP */
} type_t;

int vectorAddrIPv4[4];

/**
 * @brief str_t
 *
 * Estrutura usado para um comando da CLI.
 */

typedef struct{
	type_t tipo; /** @var tipo Define o tipo de variavel */
	char cmd_name[TAM_STRING]; /** @var cmd_name Define o nome da funcao para ser executada */
	char value[TAM_STRING_LONG]; /** @var  value Contem a informacao para ser configurado como parametro*/
	char resp[MAX_RESP_SIZE];/** @var  resp Retorna mensagem da funcao executada */
}str_t;

/**
 * @brief cli_str
 *
 * Estrutura usado para armazenar novas funcoes na lista de funcoes da CLI.
 */
typedef struct{
	break_t (*pFunc) (str_t * tp);	/** @var  tp Aponta para a funcao na lista da CLI*/
	char*	string;		/** @var  string Nome da funcao para ser chamado na CLI*/
	char* help; /** @var  help Mensagem usada para entender o comando.*/
}cli_str;

break_t do_get_temperature(str_t* tp);
break_t do_IP(str_t* tp);
break_t do_gateway(str_t* tp);
break_t do_mask(str_t* tp);
break_t do_MAC(str_t* tp);
break_t do_init(str_t* tp);
break_t do_lcd(str_t* tp);
break_t do_chat(str_t* tp);
break_t do_configIp(str_t* tp);
break_t do_ftp(str_t* tp);
