/**
 * @file sftd.c
 *
 * @brief Protocolo para transferencia de arquivos.
 *
 *
 * Protocolo Desenvolvido para poder fazer transferencia de arquivos entre um servidor local
 * e o UIP
 *
 *
 * @author Marco Tulio Martins
 *
 * @defgroup SFTD Protocolo SFT
 * @{

 */
#include "sftd.h"
#include "uip.h"

#include "grlib/grlib.h"
#include "../uart/uart.h"
#include "../cli/debug.h"


/** \brief Tamanho do buffer temporario usado pelo RLE*/
#define TAM_BUFF_TEMP 3
#define TOTAL_PIXEL 26136
int contImg, n_buff, flag_buff_temp;//flag_buff_temp usada para controlar quando pacotes estão com pixel repetidos divididos.
int buff_temp[TAM_BUFF_TEMP]; //3 vlr maximo de pixel repetidos
int break_geral;

/** \brief FLAG usada para poder verificar quando for envio de dados sem seguranca do buffer. */
int flag_sendu;

/**
 * \brief Inicializa servico do protocolo SFT
 */
void sftd_init(void){
	uip_listen(HTONS(SFTD_PORT));
}

/**
 * \brief Funcao tratados de pacotes do tipo SFT.
 *
 *
 * Funcao gerencia controle de pacotes do tipo SFT.
 */
void sftd_appcall(void){

	struct sftd_state *s = (struct sftd_state *)&(uip_conn->appstate.sftd_st);

	//uma nova conexão é estabelecida.
	if(uip_connected()){
		DEBUG_SFT("try connection\n");
		DEBUG_SFT("connection .. ok");
		s->state = ST_INIT;
		flag_sendu=0;
	}

	if(s->state == ST_CLOSE) {
		s->state = ST_INIT;
		uip_close();
		return;
	}

	if(uip_closed() || uip_aborted() || uip_timedout()) {
		DEBUG_SFT("closed... SFT\n");
		sft_closed(s);
	}

	if(uip_newdata()) {
		DEBUG_SFT("new data sft\n");
		sft_trata(s);
	}

	if(uip_rexmit() || uip_newdata() || uip_acked() || uip_connected() || uip_poll()) {
		DEBUG_SFT("send data sft\n");
		sft_senddata(s);
	}

}

/**
 * \brief Coloca protocolo em estado de fechado.
 *
 * \param s Estrutura do protocolo SFT
 */
void sft_closed(struct sftd_state *s){
	  memb_free(&s, s->buff);
	  s->state = ST_CLOSE;
}

/**
 * \brief Envia pacotes do tipo SFT
 *
 * \param s Estrutura do protocolo SFT
 */
void sft_senddata(struct sftd_state *s){
	uip_send(s->data_send, s->len_send);
}

/**
 * \brief Trata pacotes do tipo SFT
 *
 * Verifica os campos do protocolo SFT e
 * realiza o devido tratamento do pacote vindo da rede.
 *
 * \param s Estrutura do protocolo SFT
 *
 * return Retorna 0 se tudo ocorreu ok, ou -1.
 */
int sft_trata(struct sftd_state *s){
	char estado[20];
	char *dataptr;
	char ax[5];
	char ax1[5], id[5];
	int i;

	sprintf(estado,"Estado: %d\n", s->state);
	DEBUG_SFT(estado);

	dataptr = (char *)uip_appdata;

	//significa que é um comando sem seguranca e o primeiro pacote com o cabecalho ja foi tratado.
	if(flag_sendu>0){
		DEBUG_SFT("FLAG_SENDU");
		s->len_buff = strlen(dataptr);
		if (s->len_buff == 0){
			flag_sendu = 0;
			return 0;
		}
		s->nr_pkt = flag_sendu++;
		strcpy(s->buff,dataptr);
		carrega_imgu(s);
		s->state = ST_RECVU;
		return 0;
	}

	//para ser um protocolo tem que ser maior que 8. é o header do protocolo. E nao pode ser continuacao do protocolo sendu.
	if(strlen(dataptr)>5 && (flag_sendu==0)){

		//pega o tipo de comando.
		strncpy(ax, dataptr, 2);
		s->cmd = atoi(ax);
		sprintf(estado, "\ncmd: %d\t", s->cmd);
		DEBUG_SFT(estado);
//		printf(estado);

		//tamanho do pacote
		ax[0] = dataptr[2]; ax[1] = dataptr[3]; ax[2] = dataptr[4]; ax[3] = dataptr[5];
		s->len_buff = atoi(ax);
		sprintf(estado, "len: %d\t", s->len_buff);
		DEBUG_SFT(estado);
//		printf(estado);

		//se tem mais fragmentos da mesma mensagem.
		s->mf = atoi(dataptr[6]);
		sprintf(estado, "MF: %d\t", s->mf);
		DEBUG_SFT(estado);
//		printf(estado);

		//pega o nro do pacote.
		ax1[0] = dataptr[7]; ax1[1] = dataptr[8];
		s->nr_pkt = atoi(ax1);
		sprintf(estado, "NRO PKT: %d\t", s->nr_pkt);
		DEBUG_SFT(estado);
//		printf(estado);
		if(s->nr_pkt==0) printf("");

//		fflush(stdout);
		for(i=0; i<s->len_buff;i++) s->buff[i] = dataptr[i + 9];

		switch (s->cmd){

			//comando que recebe uma imagem.
			case CMD_RECV:
				s->state = ST_RECV;
				carrega_img(s);
				break;

			case CMD_STOP:
				s->state = ST_STOP;
				info_host_stop(s);
				break;
			case CMD_RECV_X:
				s->state = ST_RECV;
				carrega_img_old(s);
				break;
			case CMD_RECVU:
				s->state = ST_RECVU;
				flag_sendu = 1;
				carrega_img(s);
			default:
				info_host_error(s);
				break;

		}

		//maquina de estados do protocolo.
		switch(s->state){
			case ST_WAIT_RECV:
				info_host_wait(s);
				break;

			case ST_FINISHED_RECV:
				uart_puts("\nSFT enviou uma imagem para o display.");
			default: break;

		}

		if((FIO4PIN & (1<<8))==0){
			//DEBUG_ENTER;
			printf("\nfim do envio de imagens.\n");
			s->state = ST_STOP;
			for(i=0; i < 26136;){
				WriteSpiData(0xff); i++;
				WriteSpiData(0xff); i++;
				WriteSpiData(0xff); i++;
			}
			info_host_stop(s);
		}
	}

	return 0;
}

/**
 * \brief Monta protocolo SFT informando para destino aguardar.
 *
 * \param s Estrutura do protocolo SFT
 */
void info_host_wait(struct sftd_state *s){
	char bf[100];
	int len;

	//copia comando de resposta.
	sprintf(bf, "%02d", CMD_RECV_ANS);
	//strncpy(s->data_send, bf, 2);

	//tamamnho da resposta do comando.
	strncat(bf, "0002", 4);

	//more fragments == 0.
	strncat(bf, "0",1);

	//nro do pacote
	strncat(bf, "00",2);

	//mensagem
	strncat(bf, "ok", 2);

	//id
//	strncat(bf, "%03d", s->id);
//	len  = 14;
	len  = 11;
	uip_send(bf, len);
}

/**
 * \brief Monta protocolo SFT informando para destino que ele não suporta este tipo de servico.
 *
 * \param s Estrutura do protocolo SFT
 */
void info_host_error(struct sftd_state *s){
	char bf[100];
	int len;

	//copia comando de resposta.
	sprintf(bf, "%02d", CMD_ERROR);
	//strncpy(s->data_send, bf, 2);

	//tamamnho da resposta do comando.
	strncat(bf, "0002", 4);

	//more fragments == 0.
	strncat(bf, "0",1);

	//nro do pacote
	strncat(bf, "00",2);

	//mensagem
	strncat(bf, "xx", 2);

	//id
//	strncat(bf, "%03d", s->id);
//	len  = 14;
	len  = 11;
	uip_send(bf, len);
}




/**
 * \brief Monta e envia protocolo SFT pedindo STOP no envio de dados.
 *
 * \param s Estrutura do protocolo SFT
 */
void info_host_stop(struct sftd_state *s){
	char bf[100];
	int len;

	//copia comando de resposta.
	sprintf(bf, "%02d", CMD_STOP);
	//strncpy(s->data_send, bf, 2);

	//tamamnho da resposta do comando.
	strncat(bf, "0002", 4);

	//more fragments == 0.
	strncat(bf, "0",1);

	//nro do pacote
	strncat(bf, "00",2);

	//mensagem
	strncat(bf, "ok", 2);

	//id
//	strncat(bf, "%03d", s->id);
//	len  = 14;
	len  = 11;
	uip_send(bf, len);
}

/**
 *
 * \brief Antigafuncao usada para tratar imagens do tipo geral.
 *
 * \param s Estrutura do protocolo SFT
 */
void carrega_img_old(struct sftd_state *s){
	int i;
	char a[20];

	DEBUG_ENTER;

	if(s->nr_pkt == 0){
		//Carrega ponteiro do display para o inicio dele.
		WriteSpiCommand(0x2b);	// PASET Linha inicial e final
		WriteSpiData(0);
		WriteSpiData(131);
		WriteSpiCommand(0x2a);	// CASET Coluna inicial e final
		WriteSpiData(0);
		WriteSpiData(131);
		WriteSpiCommand(0x2c);	// Write Memory

	}

	for(i=0;i < s->len_buff; i++){
		//como o LPC2378 possui pouca memória o byte recebido é enviado direto para o display.
		//printf("%x ",s->buff[i]);
		WriteSpiData(s->buff[i]);
	}

	//carrega o estado do protocolo.
	s->state = (s->mf==1) ?  ST_WAIT_RECV : ST_FINISHED_RECV;

}

/**
 * \brief Carrega Imagem vinda do protocolo para o display.
 *
 * Pega pixels vindos por parametros e envia eles para o display de LCD da Nokia presente no kit de desenvolvimento.
 * A imagem (ou arquivo) nao e armazenado devido ao pouco espaco de memoria existente no microprocessador.
 *
 * \param s Estrutura do protocolo SFT
 */
void carrega_img(struct sftd_state *s){
	int i;
	char a[20];

	DEBUG_ENTER;
	if(s->nr_pkt == 0){
		//Carrega ponteiro do display para o inicio dele.
		WriteSpiCommand(0x2b);	// PASET Linha inicial e final
		WriteSpiData(0);
		WriteSpiData(131);
		WriteSpiCommand(0x2a);	// CASET Coluna inicial e final
		WriteSpiData(0);
		WriteSpiData(131);
		WriteSpiCommand(0x2c);	// Write Memory

		contImg = 0; 	//contador da imagem é inicializado.
		for(i=0;i<TAM_BUFF_TEMP;i++) buff_temp[i]=0;//zera o buffer de dados temporarios para caracteres repetidos.
		flag_buff_temp = 0;
		n_buff = 0;
		break_geral = 0;
	}

	if(contImg < TOTAL_PIXEL && break_geral==0){	//atualmente total_pixel é 26136.
		for(i=0;i<s->len_buff;){//é limitado em relação ao tamanho do buffer do arquivo.
			if(contImg >= TOTAL_PIXEL) break;	//ja completou o lcd.

			if(flag_buff_temp > 0){	//significa que tem pixel repetidos.
				while(n_buff--){	//grava três pixels
					if(flag_buff_temp == 3){ //significa que dois pixels é dobuffer anterior.
						WriteSpiData(buff_temp[0]);	// R1G1
						WriteSpiData(buff_temp[1]);	// B1R2
						WriteSpiData(s->buff[i]);	// G2B2
						contImg+=3;
					}
					else{	//um é do buffer antigo.
						WriteSpiData(buff_temp[0]);	// R1G1
						WriteSpiData(s->buff[i]);	// B1R2
						WriteSpiData(s->buff[i+1]);	// G2B2
						contImg+=3;
					}
				}
				i = flag_buff_temp -1;	//quantidade de pacotes utilizados do buffer que chegou.
				if(flag_buff_temp!=1) flag_buff_temp = 0; //se for1 é igual a n_buff...
			}//flag 1

			//n_buff só é alterado caso o ultimo dado do pacote anterior não tenha sido ele mesmo.
			if(flag_buff_temp ==1) flag_buff_temp = 0;	//n_buff fica com o valor do ultimo pacote.
			else n_buff = s->buff[i] & 0xff; //nro de pixels repetidos.
			if(n_buff == 0xfa) { break_geral = 1; break; }//break total...................................................
			while(n_buff--){	//grava três pixel
				WriteSpiData(s->buff[i+1]);	// R1G1
				WriteSpiData(s->buff[i+2]);	// B1R2
				WriteSpiData(s->buff[i+3]);	// G2B2
				contImg+=3;
			}
			i +=4;//contador do buffer do pacote.

			if(i > s->len_buff){//	significa que não pode carregar as imagens, devido ao buffer chegar ao fim.
				if(i == s->len_buff + 4) break;	//todos os dados vão estar no proximo pacote.
				flag_buff_temp = i - s->len_buff; //máximo é três mínimo é 1.
				n_buff = s->buff[i - 3] & 0xff; //nro de pixels repetidos.
				if(n_buff == 0xfa){ break_geral = 1; break; }//break total...................................................
				if(flag_buff_temp == 3){	//significa que tem mais dois no buffer.
					buff_temp[0] = s->buff[i-2];
					buff_temp[1] = s->buff[i-1];
				}
				else buff_temp[0] = s->buff[i-1];	//então é apenas um pacote.
			}
		}//total do pacote vindo da internet.

	}//total_pixel

	//carrega o estado do protocolo.
	s->state = (s->mf==1) ?  ST_WAIT_RECV : ST_FINISHED_RECV;

	//configura flag_sendu para 0, significa que pacote terminou de receber.
	if(break_geral == 1) flag_sendu = 0;

}



/**
 * \brief Carrega Imagem vinda do protocolo para o display.
 *
 * Pega pixels vindos por parametros e envia eles para o display de LCD da Nokia presente no kit de desenvolvimento.
 * A imagem (ou arquivo) nao e armazenado devido ao pouco espaco de memoria existente no microprocessador.
 *
 * \param s Estrutura do protocolo SFT
 */
void carrega_imgu(struct sftd_state *s){
	int i;
	char a[20];

	DEBUG_ENTER;

	if(contImg < TOTAL_PIXEL && break_geral==0){	//atualmente total_pixel é 26136.
		for(i=0;i<s->len_buff;){//é limitado em relação ao tamanho do buffer do arquivo.
			if(contImg >= TOTAL_PIXEL) break;	//ja completou o lcd.

			if(flag_buff_temp > 0){	//significa que tem pixel repetidos.
				while(n_buff--){	//grava três pixels
					if(flag_buff_temp == 3){ //significa que dois pixels é dobuffer anterior.
						WriteSpiData(buff_temp[0]);	// R1G1
						WriteSpiData(buff_temp[1]);	// B1R2
						WriteSpiData(s->buff[i]);	// G2B2
						contImg+=3;
					}
					else{	//um é do buffer antigo.
						WriteSpiData(buff_temp[0]);	// R1G1
						WriteSpiData(s->buff[i]);	// B1R2
						WriteSpiData(s->buff[i+1]);	// G2B2
						contImg+=3;
					}
				}
				i = flag_buff_temp -1;	//quantidade de pacotes utilizados do buffer que chegou.
				if(flag_buff_temp!=1) flag_buff_temp = 0; //se for1 é igual a n_buff...
			}//flag 1

			//n_buff só é alterado caso o ultimo dado do pacote anterior não tenha sido ele mesmo.
			if(flag_buff_temp ==1) flag_buff_temp = 0;	//n_buff fica com o valor do ultimo pacote.
			else n_buff = s->buff[i] & 0xff; //nro de pixels repetidos.
			if(n_buff == 0xfa) { break_geral = 1; break; }//break total...................................................
			while(n_buff--){	//grava três pixel
				WriteSpiData(s->buff[i+1]);	// R1G1
				WriteSpiData(s->buff[i+2]);	// B1R2
				WriteSpiData(s->buff[i+3]);	// G2B2
				contImg+=3;
			}
			i +=4;//contador do buffer do pacote.

			if(i > s->len_buff){//	significa que não pode carregar as imagens, devido ao buffer chegar ao fim.
				if(i == s->len_buff + 4) break;	//todos os dados vão estar no proximo pacote.
				flag_buff_temp = i - s->len_buff; //máximo é três mínimo é 1.
				n_buff = s->buff[i - 3] & 0xff; //nro de pixels repetidos.
				if(n_buff == 0xfa){ break_geral = 1; break; }//break total...................................................
				if(flag_buff_temp == 3){	//significa que tem mais dois no buffer.
					buff_temp[0] = s->buff[i-2];
					buff_temp[1] = s->buff[i-1];
				}
				else buff_temp[0] = s->buff[i-1];	//então é apenas um pacote.
			}
		}//total do pacote vindo da internet.

	}//total_pixel
	else flag_sendu = 0;


	//configura flag_sendu para 0, significa que pacote terminou de receber.
	if(break_geral == 1) flag_sendu = 0;

}


/** @} */


