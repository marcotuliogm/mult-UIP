/**
 * @file cli.c
 *
 * @brief Interpretacao dos comandos digitados na CLI
 *
 * Recebe a informacao feita na CLI
 * Interpreta, monta token com todos os parametros e chama funcao correspondente para
 * tratar solicitacao.
 *
 * @author Marco Tulio Martins
 *
 * @defgroup CLI Controle da CLI
 * @{

 */

#include "cli.h"
#include "menu_cli.h"
//#include "cli_func.h"
#include <string.h>


#define AUTO_COMPLETE 0x1b //get_char 0x5b

static const cli_str* acpi;
static str_t tkn;	//token genérico, guarda a informação do processo em execução.
static char msg_dest[MAX_TAM_SIZE_STRING];

extern cli_str const api_cli[];				/* - tabela de parametros */
static const cli_str* menu_cli = api_cli;

char buff_cmd[MAX_COMMAND_SIZE];
int buff_cmd_counter=0;


/**
 * @brief Monta a estrutura para acesso a tabela da CLI.
 *
 * Função tratadora da entrada de comando, monta a estrutura.
 * 
 * @param data Comando digitado na CLI.
 *
 * @return RET_OK para execucao com sucesso da funcao ou -1
 * */
int mount_token(char *data)
{
	char* buff="\0";
	char* buff2="\0";
	//char buff[MAX_COMMAND_SIZE];
	U08_t i,j;

	memset(tkn.cmd_name, '\0',TAM_STRING);
	memset(tkn.value, '\0',TAM_STRING);
	memset(tkn.resp, '\0',TAM_STRING);

	if((j = strlen(data))==0){
		DEBUG_ERROR(INVALID_DATA);
		return -1;
	}
	else {
		for(i=0;i<j;i++){	//Passa todo o texto que veio para maiusculo.
			data[i] = toupper(data[i]);
			if(data[i] == ' '){
			}
		}
	}
	
	DEBUG_CLI(data);
	buff = strtok(data," ");

	//strcpy(tkn.tipo, buff);//type command (get/set)
	//tkn.tipo = (strcmp(buff, "GET")==0) ? GET_CMD : SET_CMD;
	if(strcmp(buff, "GET")==0) tkn.tipo = GET_CMD;
	else if(strcmp(buff, "SET")==0) tkn.tipo = SET_CMD;
	else if(strcmp(buff, "HELP")==0) tkn.tipo = HELP_CMD;
	else if(strcmp(buff, "CHAT")==0) tkn.tipo = CHAT_CMD;
	else if(strcmp(buff, "FTP")==0) tkn.tipo = FTP_CMD;
	else return -1;
	buff2 = strtok(NULL, " ");
	if (tkn.tipo == CHAT_CMD || tkn.tipo == FTP_CMD){
		strncpy(tkn.value, buff2,j);//get porta(chat) ou IP(ftp).
		strncpy(tkn.cmd_name, buff,strlen(buff));//coloca o nome de CHAT ou FTP.
		return 0;
	}
	strncpy(tkn.cmd_name, buff2,j);//get name
	buff2 = strtok(NULL, " ");
	strcpy(tkn.value, buff2);//get value
	//tkn.resp = "\0";

	//sprintf(buff, "Dados: Tipo %02d - Comando %s - Value - %s", tkn.tipo,tkn.cmd_name, tkn.value);
	//debug_cli(buff);

	return 0;
}

/**
 * @brief Atende solicitacao da CLI
 *
 * Chama função para montar estrutura do comando
 * Busca função correspondete.
 *
 * Ao fim da execução dest_msg esta com a resposta do sistema.
 *
 * @param data Comando digitado na CLI
 * @param dest Destino da resposta da CLI para o comando digitado.
 * @return RET_OK para execucao com sucesso da funcao ou RET_ERROR
 */
int CLI_Builder(char *data, char *dest){

	acpi = menu_cli;
	const cli_str* mCmd;
	char *buff;
	int ret=0,i=0;
	
	DEBUG_ENTER;

	strcpy(msg_dest, " ");

	if (strlen(data)<=2) return -1;
	
	if(mount_token(data)<0){
		unknown_cmd(data);
		strcpy(dest, msg_dest);
		return -1;
	}

	if(tkn.tipo == HELP_CMD){	//verifica se é um comando de help para todas as funções.
			help_func();
			strcpy(dest, msg_dest);
			return ret;
	}

	mCmd = acpi;
	for(i=0;;i++){
	//for(mCmd = acpi;;acpi++){//percorre toda a tabela de comandos a procura docomando.


		buff = mCmd->string;
		
		if(buff == NULL){
			DEBUG_ERROR(COMMAND_NOT_FOUND);
			unknown_cmd(tkn.cmd_name);
			strcpy(dest, msg_dest);
			return -1;
		}
		if(!strcmp(buff, tkn.cmd_name))	break; //achou a função correspondente.
		mCmd = acpi++;
	}

	DEBUG_CLI("Chamando função tratadora do comando.\n");
	ret = ExecCommand(mCmd);
	strcpy(dest, msg_dest);
	
	//limpa memória de conteudos....
	memb_free(msg_dest, msg_dest);
	memb_free(&tkn, tkn.resp);
	memb_free(&tkn, tkn.cmd_name);
	memb_free(&tkn, tkn.value);
	memb_free(&tkn, tkn.tipo);
	memb_free(&mCmd, buff);


	DEBUG_EXIT;
	//uart_puts(msg_dest);
	return ret;
}

/**
 * @brief Funcao que executa solicitacao feita na CLI
 *
 * Apos o comando ser interpretado
 * funcao chamada o agente responsavel em processar o pedido
 *
 * @param tp Estrutura com os dados para interpretacao do comando.
 */
void cmdExec(int (*ApiFunc) (str_t *tp)) {
	break_t error;
	DEBUG_ENTER;
	//executa função referente a chamada.
	
	error = (ApiFunc) (&tkn);	//executa função referente a chamada,passando como parametro a struct para retorno.
	DEBUG_CLI("OK.");
	if(error == RET_ERROR){
		DEBUG_ERROR(EXEC_COMMAND_ERROR);
		strcat(msg_dest,"COMMAND ERROR\n");
	}
	else {
		strcat(msg_dest, tkn.resp);
		strcat(msg_dest, "\n");
	}
	DEBUG_EXIT;
}

/**
 * @brief Funcao gerencia a mensagem de resposta
 *
 * Configura o pedido feito pela CLI
 * Chama a funcao solicitada para ser executada
 * Configura mensagem de retorno
 *
 * @param acp Estrutura apontando para o objeto na lista de funcoes da CLI.
 * @return RET_OK para execucao com sucesso da funcao ou RET_ERROR
 */
int ExecCommand(const cli_str* acp){

	strcpy(msg_dest, "\n");
	strcat(msg_dest, tkn.cmd_name);
	strcat(msg_dest, ": ");
	cmdExec(acp->pFunc);

	//debug_cli(msg_dest);
	
	return 0;
}

/**
 * @brief Pega mensagem da serial
 *
 * Pega os dados digitados na CLI ate que venha uma nova linha.
 *
 * @param str string para o retorno do comando digitado
 * @param channel Canal que sera usado pela serial.
 *
 */
void UARTTakeString(char *str, char channel)
{
	int vd = 0xff;
	char tp[MAX_COMMAND_SIZE]="\0";
	char cond = '\n';
	do{
			vd = UARTReceiveByte(channel);
			uart_putc(vd);
		if (vd != cond) sprintf(tp, "%s%c", tp,vd);
	}while (vd != cond);
	strcpy(str, tp);
}

/**
 * @brief Configura com o IP inicial
 *
 * Configura o endereco de IP para um valor fixo 192.168.1.13
 *
 */
void set_IpDefault(void){
	vectorAddrIPv4[0] = 192;
	vectorAddrIPv4[1] = 168;
	vectorAddrIPv4[2] = 1;
	vectorAddrIPv4[3] = 13;
	set_IP();
}

/**
 * @brief Configura com o Gateway inicial
 *
 * Configura o endereco de Gateway para um valor fixo 192.168.1.1
 *
 */
void set_GatewayDefault(void){
	uip_GAT0 = 192;
	uip_GAT1 = 168;
	uip_GAT2 = 1;
	uip_GAT3 = 1;
	set_Gateway();
}

/**
 * @brief Configura com a mascara inicial
 *
 * Configura o endereco de mascara para um valor fixo 255.255.255.0
 *
 */
void set_MaskDefault(void){
	uip_Mask0 = 255;
	uip_Mask1 = 255;
	uip_Mask2 = 255;
	uip_Mask3 = 0;
	set_Mask();
}


/**
 * @brief Funcao que mostra mensagem de HELP
 *
 * Apresenta todas as funcionalidades diponiveis com um help.
 * Varre todos os comandos da CLI e exibe
 * mensagem configurada no help de cara funcao.
 *
 */
static void help_func(void){

	acpi = menu_cli;
	const cli_str* mCmd;
	char buff[200]="\0";
	int i;

	DEBUG_ENTER;

	mCmd = acpi++;
	strcpy(msg_dest,"\nFuncoes Basicas do Módulo Principal\n");
	for(i=0;;i++){
	//for(mCmd = acpi;mCmd->string != NULL;++acpi){//percorre toda a tabela de comandos.
		if(mCmd->string == NULL) break;
		sprintf(buff, "\n%20s\t - \t%s",mCmd->string,mCmd->help);
		strncat(msg_dest, buff, strlen(buff));

		mCmd = acpi++;
	}

	DEBUG_EXIT;
}


/**
 * @brief Funcao para comandos desconhecidos
 *
 * Funcao que imprime mensagem de comando desconhecido.
 *
 */
static void unknown_cmd(char *str)
{
	char buff[100];
  if(strlen(str) > 0) {
    sprintf(buff, "\nUnknown command: %s\n", str);
    strcpy(msg_dest, buff);
  }
}


/** @} */



/**
 * @brief Pega mensagem da serial assincrono.
 *
 * Pega os dados digitados na CLI ate que venha uma nova linha.
 * Não é bloqueante.
 *
 * @param str string para o retorno do comando digitado
 * @param channel Canal que sera usado pela serial.
 *
 * @return 0 - se Ok, -1 se buffer ainda não esta completo.
 *
 */
int UARTTakeStringAsync(char *str, char channel)
{
	char vd = 0xff, i;
//	char tp[MAX_COMMAND_SIZE]="\0";
	char cond = '\n';

	vd = UARTReceiveByte(channel);

	if(buff_cmd_counter == 0){
		//verifica se o comando é seta para cima. Se sim, retorna o comando anterior.
		if(vd == 0x01b){
			vd = UARTReceiveByte(channel);
			printf("*");
			//if(buff_cmd !=NULL){
			 if(vd = 0x5b){
				uart_puts(buff_cmd);
				strcpy(str, buff_cmd);
				return 0;
			}
		}
		//senão limpa buffer
		for(i=0;i<MAX_COMMAND_SIZE;i++)	buff_cmd[i]='\0';
	}

	//senão é um comando de fim de linha.
	if(vd == cond || vd == 0x0D){
		DEBUG_ENTER;
		sprintf(buff_cmd, "%s\0", buff_cmd,vd);
		strcpy(str, buff_cmd);
		buff_cmd_counter = 0;
		return 0;
	}

	//verifica se não foi digitado enter.
	else if (vd != cond){
		uart_putc(vd);
		sprintf(buff_cmd, "%s%c", buff_cmd,vd);
		//buff_cmd[buff_cmd_counter] == vd;
		buff_cmd_counter++;
		return -1;
	}

	return 0;
}

