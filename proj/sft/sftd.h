/**
 * @file sftd.h
 *
 * @brief Declaracao e definicoes usadas pelo protocolo SFT
 *
 *
 * Protocolo Desenvolvido para poder fazer transferencia de arquivos entre um servidor local
 * e o UIP.
 *
 *
 * @author Marco Tulio Martins
 */
#ifndef SFTD_H_
#define SFTD_H_

#include "uipopt.h"
#include "uip-conf.h"
#include "../mult/api_services.h"

/**
 * \brief Tamanho do buffer de cada pacote.
 */
#define SFTD_BUFFER_DATA 1500

/**
 * \brief sft_st Estrutura com os estados do protocolo.
 *
 * Lista de tipos de estados que pode estar o protocolo.
 */
typedef enum {
	ST_INIT=0,		//ESTADO INICIAL
	ST_RECV,		//RECEBENDO PACOTE DO HOST
	ST_WAIT_RECV,	//AGUARDANDO PARTES DO ARQUIVO QUE ESTÁ SENDO RECEBIDO
	ST_FINISHED_RECV,//FINALIVOU O RECEBIMENTO
	ST_SEND,		//ENVIANDO PACOTES PARA O HOST
	ST_WAIT_SEND,	//AGUARDANDO CONFIRMAÇÃO PARA ENVIO DE OUTRA PARTE DO PACOTE
	ST_FINISHED_SEND,//TERMINOU O ENVIO O PACOTE
	ST_STOP,
	ST_RECVU,		//RECEBENDO PACOTES DO HOST SEM SEGURANCA
	ST_CLOSE
} sft_st;

/**
 * \brief Servicos disponiveis pelo protocolo SFT
 *
 * Comandos aceitos e que são interpretados pelo protocolo.
 */
typedef enum {
	CMD_RECV =0,
	CMD_RECV_ANS,
	CMD_STOP,
	CMD_RECV_SEQ,
	CMD_FINISHED,
	CMD_RECV_X,
	CMD_RECVU,
	CMD_ERROR,
	CMD_MSG
} cmd_sft;

/**
 * \brief Estrutura protocolo SFT.
 *
 * Estrutura com definicao de todos os parametros nececessarios na estrutura do
 * protocolo SFT.
 */
struct sftd_state {
  char buff[SFTD_BUFFER_DATA];
  int len_buff;
  char data_send[SFTD_BUFFER_DATA];
  int len_send;
  int mf;
  int nr_pkt;
  int id;
  cmd_sft cmd;
  cmd_sft cmd_answer;
  sft_st state;
};


/**
 * \brief Estrutura de um pacote SFT
 * Acrescentado de 1 caracter == '\0'.
 */
struct sft_hdr_uip{
	char cmd[3];
	char len[5];
	char mf[2];
	char nro_pkt[3];
	char id[4];
	char buff_send[SFTD_BUFFER_DATA+1];
};


void sftd_init(void);

void sftd_appcall(void);
void sft_senddata(struct sftd_state *s);
int sft_trata(struct sftd_state *s);
void sft_closed(struct sftd_state *s);


//funcoes para os comandos

void carrega_img(struct sftd_state *s);
void carrega_imgu(struct sftd_state *s);
void carrega_img_old(struct sftd_state *s);
void info_host_ans(struct sftd_state *s, int cmd);
void envia_msg(struct sftd_state *s);



#endif /* SFTD_H_ */
