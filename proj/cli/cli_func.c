/**
 * @file cli_func.c
 *
 * @brief Arquivos com a implementacao das funcoes chamadas pela CLI
 *
 * @author Marco Tulio Martins
 */


#include <string.h>
#include "cli_func.h"
#include <uip.h>
#include <uip_arp.h>
#include "file_send.h"	//usado para o ftp....

enum {GET = 0, SET};

/**
 * @brief funcao que retorna o IP.
 */
char* get_IP(void);

/**
 * @brief Funcao para trocar mensagem inicial do LCD
 *
 * Funcao que altera a mensagem que esta no display 16x2
 * quando o sistema e inicializado.
 */
void set_lcd_init(){
	RTCTime time;
	char buff[30];

	lcd_clr();
	lcd_goto(1,0);
	//mostra IP.
	lcd_string("IP ");
	sprintf(buff, "%s",get_IP());
	lcd_string(buff);


}

/**
 * @brief Funcao para mostrar hora no display
 *
 * Funcao que pega a hora atravesdo RTC e mosta
 * no display no forma hh:mm:ss
 */
void show_lcd_time(){
	char *buff;
	RTCTime time;
	time = RTCGetTime();
	sprintf(buff, "%2d:%2d:%2d", time.RTC_Hour, time.RTC_Min, time.RTC_Sec);
	lcd_goto(2,0);
	lcd_string(buff);

}


/**
 * @brief Mensagem inicial da serial
 *
 * -------------------------------------------
 * -     TCC  -  UIP MULTIPLE APLICATION     -
 * -------------------------------------------
 * * @return RET_OK para execucao com sucesso da funcao
 */
break_t do_init(str_t* tp){
	strcpy(tp->resp,"\n-------------------------------------------");
	strcat(tp->resp,"\n-     TCC  -  UIP MULTIPLE APLICATION     -");
	strcat(tp->resp,"\n-------------------------------------------\n");
	set_lcd_init();
	return RET_OK;
}

/**
 * @brief Funcao que retorna a temperatura ambiente
 *
 * Funcao que le a temperatura do sensor de temperatura
 * presente no kit e retorna para quem solicitou
 *
 * @return RET_OK para execucao com sucesso da funcao
 */
break_t do_get_temperature(str_t* tp){
	char buff[20];
	DEBUG_CLI("do_get_temperature");
	strcpy(tp->resp,"em construcao");
	//tp->resp =  buff;
	return RET_OK;
}

/**
 * @brief Comando da CLI para chamada do Chat
 *
 * Comando usado para inicializar o chat
 * Deve ser passado como parametro junto ao comando a porta de acesso que sera usado no display.
 *
 * @return RET_OK para execucao com sucesso da funcao
 */
break_t do_chat(str_t* tp){
	char buff[20], porta[20];
	uint32_t port;

	global_chat_flag = 1; //enquanto 1 a UART fica bloqueada para o chat.
	DEBUG_CLI("do_chat troca de mensagens entre usuarios.\n");
	if(tp->value == NULL) return RET_ERROR;

	port = (uint32_t) atoi (tp->value);
	if(port < 0) return RET_ERROR;
	sprintf(buff, "Porta %d",port);
	DEBUG_CLI(buff);
	CHAT_PORT = port;
	chat_start();//chama com a porta definida que será usado no chat.
	uart_puts("\nCLI SUSPENSA.\n");

//	while(global_chat_flag==1); //fica aqui ateh vir comando de quit pelo chat para sair.

	return RET_OK;
}


/**
 * @brief Funcao para configuracao do IP
 *
 * Permite alterar o endereco de IP
 * obs: deve se tomar o cuidado de fazer a troca com uma conexao serial
 * pois a troca de IP é imediata.
 *
 * @return RET_OK para execucao com sucesso da funcao
 */
void set_IP(void){

	uip_ipaddr_t xIPAddr;
	char buff[40];

	uipIP_ADDR0 = vectorAddrIPv4[0];
	uipIP_ADDR1 = vectorAddrIPv4[1];
	uipIP_ADDR2 = vectorAddrIPv4[2];
	uipIP_ADDR3 = vectorAddrIPv4[3];

//	ethernet_init();
	DEBUG_CLI("\nconfig IP");

	uip_ipaddr( xIPAddr, uipIP_ADDR0, uipIP_ADDR1, uipIP_ADDR2, uipIP_ADDR3 );
	uip_sethostaddr( xIPAddr );
}

/**
 * @brief Funcao que retorna o endereco IP.
 *
 * Funcao que faz leitura do endereco de IP.
 *
 * @return Retorna o endereco de IP em uma string no formato xx.xx.xx.xx
 */
char* get_IP(void){
	char buff[30];
	uip_ipaddr_t xIPAddr;

	uip_gethostaddr(xIPAddr);
	sprintf(buff,"%d.%d.%d.%d", uip_ipaddr1(xIPAddr), uip_ipaddr2(xIPAddr),
								uip_ipaddr3(xIPAddr), uip_ipaddr4(xIPAddr));

	return buff;
}

/**
 * @brief Funcao que permite ver o endereco MAC
 *
 * Faz leitrura do endereco MAC do Kit
 * Funcao de leitura apenas.
 *
 * @return RET_OK para execucao com sucesso da funcao
 */
break_t do_MAC(str_t* tp){
	char buff[20];
	if(tp->tipo == SET)
		strcpy(tp->resp, "Apenas comando de GET.");
	else if (tp->tipo == GET){
		sprintf(buff, "%d:%d:%d:%d:%d:%d", uipMAC_ADDR0, uipMAC_ADDR1, uipMAC_ADDR2,uipMAC_ADDR3,
				uipMAC_ADDR4,uipMAC_ADDR5);
	}
	return RET_OK;
}

/**
 * @brief Funcao da CLI para configuracao do IP
 *
 * Funcao para ler ou alterar o endereco de IP.
 *
 * @param tp Estrutura com os dados para interpretacao do comando.
 * @return RET_OK para execucao com sucesso da funcao ou RET_ERROR
 */
break_t do_IP(str_t* tp){
	char* buff="\0";
	char arq[20];
	char dest[30];

	DEBUG_CLI("Return IP address");

	if(tp->tipo == SET){	//tipo SET
		strcpy(arq,tp->value);
		if(strlen(arq)<=0) return RET_ERROR;
		buff = strtok(arq, " ,.");
		vectorAddrIPv4[0] = atoi(buff);
		buff = strtok(NULL, " ,.");
		vectorAddrIPv4[1] = atoi(buff);
		buff = strtok(NULL, " ,.");
		vectorAddrIPv4[2] = atoi(buff);
		buff = strtok(NULL, " ,.");
		vectorAddrIPv4[3] = atoi(buff);
		set_IP();
		sprintf(dest,"set to %d.%d.%d.%d",uipIP_ADDR0, uipIP_ADDR1, uipIP_ADDR2, uipIP_ADDR3);
		strcpy(tp->resp, dest);
		//tp->resp = dest;
	}
	else {	//get
		sprintf(dest,"%s", get_IP());
		strcpy(tp->resp, dest);
		DEBUG_CLI(tp->resp);
		//sprintf(tp->resp,"%s", get_IP());
	}
	return RET_OK;

}


/**
 * @brief Escreve dados no LCD
 *
 * Permite escrever uma mensagem no display de LCD.
 *
 * @param data Mensagem para ser mostrada no display
 * @param col Coluna onde o cursor deve estar.
 * @param linha Linha onde o cursor deve estar.
 * @return RET_OK para execucao com sucesso da funcao ou RET_ERROR
 */
void writeDataLcd(char* data, int col, int linha)
{
	char dest[20];
	lcd_clr();
	sprintf(dest,"Coluna: %d linha: %d",col, linha);
	DEBUG_CLI(dest);
	lcd_goto(linha, col);
	lcd_string(data);
}

/**
 * @brief Permite escrever uma mensagem no LCD
 *
 * Escreve mensagem no display 16x2
 * Funcao permite apenas comandos de SET.
 *
 * @param tp Estrutura com os dados para interpretacao do comando.
 * @return RET_OK para execucao com sucesso da funcao ou RET_ERROR
 */
break_t do_lcd(str_t* tp){
	char arq[40], msg[16];//16 é o maximo para a linha
	char* buff="\0";
	int posX = 0, posY = 0;

	DEBUG_CLI("do_lcd");
	if(tp->tipo == SET){	//tipo SET
			strcpy(arq,tp->value);
			if(strlen(arq)<=0) return RET_ERROR;
			buff = strtok(arq, ";"); //mensagem para aparecer no display
			strcpy(msg, buff);
			buff = strtok(NULL, " ,.");
			posX = atoi(buff);
			if (posX > 16 || posX < 0) return RET_ERROR;
			buff = strtok(NULL, " ,.");
			posY = atoi(buff);
			if (posY > 2 || posY < 0) return RET_ERROR;
			writeDataLcd(msg, posX, posY);
			strcpy(tp->resp,"Dados Escritos com sucesso.");
		}
		else {	//get
			strcpy(tp->resp, "Apenas comando de SET.");
			DEBUG_CLI(tp->resp);
			//sprintf(tp->resp,"%s", get_IP());
		}
		return RET_OK;
}


/**
 *
 */
void out_data(char * data){
	if(OUT_UART) uart_puts(data);
	else if(OUT_TELNET) shell_output(data,"");
}

/**
 * @brief Funcao para alterar o GATEWAY do kit.
 *
 * Permite alterar o endereco do gateway de rede.
 * obs: realizar esta alteracao apenas quando estiver conectado via serial.
 *
 */
void set_Gateway(void){
	uip_ipaddr_t xIPAddr;

	DEBUG_CLI("config Gateway");
	uip_ipaddr( xIPAddr, uip_GAT0, uip_GAT1, uip_GAT2, uip_GAT3);
	uip_setdraddr(xIPAddr);
}

/**
 * @brief Funcao da CLI paraconfiguracao do Gateway
 *
 * Funcao permite ler e alterar endereco do gateway
 *
 * @param tp Estrutura com os dados para interpretacao do comando.
 * @return RET_OK para execucao com sucesso da funcao ou RET_ERROR
 */
break_t do_gateway(str_t* tp){
	char* buff="\0";
	char arq[20];
	char dest[30];
	int end[4];
	uip_ipaddr_t xGtAddr;

	DEBUG_CLI("Return Gateway");

	if(tp->tipo == SET){	//tipo SET
		strcpy(arq,tp->value);
		if(strlen(arq)<=0) return RET_ERROR;
		buff = strtok(arq, " ,.");
		uip_GAT0 = atoi(buff);
		buff = strtok(NULL, " ,.");
		uip_GAT1 = atoi(buff);
		buff = strtok(NULL, " ,.");
		uip_GAT2 = atoi(buff);
		buff = strtok(NULL, " ,.");
		uip_GAT3 = atoi(buff);
		set_Gateway();
		sprintf(dest,"set to %d.%d.%d.%d",uip_GAT0, uip_GAT1, uip_GAT2, uip_GAT3);
		strcpy(tp->resp, dest);
		//tp->resp = dest;
	}
	else {	//get
		uip_getdraddr(xGtAddr);
		sprintf(dest,"%d.%d.%d.%d", uip_ipaddr1(xGtAddr), uip_ipaddr2(xGtAddr),
									uip_ipaddr3(xGtAddr), uip_ipaddr4(xGtAddr));
		strcpy(tp->resp, dest);
		DEBUG_CLI(tp->resp);
		//sprintf(tp->resp,"%s", get_IP());
	}
	return RET_OK;

}

/**
 * @brief Permite alterar a mascara de rede
 *
 */
void set_Mask(void){
	uip_ipaddr_t xIPAddr;

	DEBUG_CLI("config Mask");
	uip_ipaddr( xIPAddr, uip_Mask0, uip_Mask1, uip_Mask2, uip_Mask3);
	uip_setnetmask(xIPAddr);

}

/**
 * @brief Funcao da CLi para configuracao da mascara de rede
 *
 * Funcao permite ler ou alterar o endereco de mascara.
 *
 * @param tp Estrutura com os dados para interpretacao do comando.
 * @return RET_OK para execucao com sucesso da funcao ou RET_ERROR
 */
break_t do_mask(str_t* tp){
	char* buff="\0";
	char arq[20];
	char dest[30];
	int end[4];
	uip_ipaddr_t xMaskAddr;

	DEBUG_CLI("Return Mask");

	if(tp->tipo == SET){	//tipo SET
		strcpy(arq,tp->value);
		if(strlen(arq)<=0) return RET_ERROR;
		buff = strtok(arq, " ,.");
		uip_Mask0 = atoi(buff);
		buff = strtok(NULL, " ,.");
		uip_Mask1 = atoi(buff);
		buff = strtok(NULL, " ,.");
		uip_Mask2 = atoi(buff);
		buff = strtok(NULL, " ,.");
		uip_Mask3 = atoi(buff);
		set_Gateway();
		sprintf(dest,"set to %d.%d.%d.%d", uip_Mask0, uip_Mask1, uip_Mask2, uip_Mask3);
		strcpy(tp->resp, dest);
		//tp->resp = dest;
	}
	else {	//get

		uip_getnetmask(xMaskAddr);
		sprintf(dest,"%d.%d.%d.%d", uip_ipaddr1(xMaskAddr), uip_ipaddr2(xMaskAddr),
									uip_ipaddr3(xMaskAddr), uip_ipaddr4(xMaskAddr));
		strcpy(tp->resp, dest);
		DEBUG_CLI(tp->resp);
		//sprintf(tp->resp,"%s", get_IP());
	}
	return RET_OK;

}

/**
 * @brief Funcao da CLI que retorna enderecos da rede.
 *
 * Funcao que permite gera como retorno o endereco de Mascara, Gateway e IP.
 */
break_t do_configIp(str_t* tp){
	char* buff="\0";
	char arq[20];
	char dest[80];
	int end[4];

	DEBUG_CLI("Return network info");

	if(tp->tipo == SET){	//tipo SET
		sprintf(dest,"Just get command.");
		strcpy(tp->resp, dest);
		//tp->resp = dest;
	}
	else {	//get
		sprintf(arq,"\nMask: %d.%d.%d.%d", uip_Mask0, uip_Mask1, uip_Mask2, uip_Mask3);
		strcpy(dest, arq);
		sprintf(arq,"\nGateway: %d.%d.%d.%d", uip_GAT0, uip_GAT1, uip_GAT2, uip_GAT3);
		strcat(dest, arq);
		sprintf(dest,"\nIP:  %d.%d.%d.%d",uipIP_ADDR0, uipIP_ADDR1, uipIP_ADDR2, uipIP_ADDR3);
		strcat(dest, arq);
		strcpy(tp->resp, dest);
		DEBUG_CLI(tp->resp);
		//sprintf(tp->resp,"%s", get_IP());
	}
	return RET_OK;

}



/**
 * @brief Funcao para conectar como FTP Client
 *
 * Funcao para conexao como FTP Client
 *
 * Descontinuado pois o kit possui pouca memória para armazenamento de arquivo para transmissao.
 *
 * @param tp Estrutura com os dados para interpretacao do comando.
 * @return RET_OK para execucao com sucesso da funcao ou RET_ERROR
 */
break_t do_ftp(str_t* tp){

	strcpy(tp->resp, "Servico Indisponivel");

#if 0
	char* end_ip='\0';

	unsigned char TSTflag = 0;
	struct uip_conn* ftp_conn = NULL;
	arn_CHAR srvSystType[MAX_SRV_SYSTTYPE];
	ftp_cmd_status_t stst;
	DEBUG_FTP("ftp client CLI");

	end_ip = tp->value;

	/* modelo de comunicação ftp client, realizado tendo como base exemplo utilizado por
	 * Fabio Giovagnini <fabio.giovagnini@aurion-tech.com>
	 * Serviço implementado para validar multiplas aplicações rodando em paralelo.
	 *
	 * Enviando o arquivo SendFile_FTP.txt, composto pelo vetor de caracteres
	 * presente no array MSG, em file_send.h
	 * */
	while(TSTflag != 7 || TSTflag >= 200){
		//printf("ststs - %d", stst);
		if (TSTflag == 0) {
			stst = ftp_connect(end_ip,21,&ftp_conn);
			TSTflag = 7;
			if (stst == ftpcs_FAILED) {TSTflag = 201; break;}
			if (stst == ftpcs_SUCCESS) {TSTflag = 1; break;}
			}
		else if (TSTflag == 1) {
			DEBUG_FTP("ftp login..");
			stst = ftp_login(ftp_conn,"mtgm");
			DEBUG_FTP(" . ok");
			if (stst == ftpcs_PASSWORD_REQ) {TSTflag = 10; break;}
			if (stst == ftpcs_FAILED) {TSTflag = 202; break;}
			if (stst == ftpcs_SUCCESS) {TSTflag = 2; break;}
			}
		else if (TSTflag == 2) {
			DEBUG_FTP("ftp syst..");
			stst = ftp_syst(ftp_conn,srvSystType);
			DEBUG_FTP("ftp ok..");
			if (stst == ftpcs_FAILED) {TSTflag = 203; break;}
			if (stst == ftpcs_SUCCESS) {TSTflag = 3; break;}
			}
		else if (TSTflag == 3) {
			stst = ftp_port(ftp_conn,5001);
			if (stst == ftpcs_FAILED) {TSTflag = 204; break;}
			if (stst == ftpcs_SUCCESS) {TSTflag = 4; break;}
			}
		else if (TSTflag == 4) {
			stst = ftp_send(ftp_conn,"SendFile_FTP.txt",(arn_uCHAR*)MSG,strlen(MSG));
			if (stst == ftpcs_FAILED) {TSTflag = 205; break;}
			if (stst == ftpcs_SUCCESS) {TSTflag = 5; break;}
			}
		else if (TSTflag == 5) {
			stst = ftp_quit(ftp_conn);
			if (stst == ftpcs_FAILED) {TSTflag = 206; break;}
			if (stst == ftpcs_SUCCESS) {TSTflag = 6; break;}
			}
		else if (TSTflag == 6) {
			ftp_conn = 0;
			TSTflag = 7;
			}
		else if (TSTflag == 10) {
			stst = ftp_password(ftp_conn,"mtgm");
			if (stst == ftpcs_FAILED) {TSTflag = 210; break;}
			if (stst == ftpcs_SUCCESS) {TSTflag = 2; break;}
			}
	}

#endif

	return RET_OK;
}
