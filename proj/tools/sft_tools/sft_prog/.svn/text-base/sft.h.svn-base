/**
 * @file sft.h
 *
 * @brief Declaracoes das funcoes usadas no servidor  do protocolo SFT.
 
 *
 * @author Marco Tulio Martins
 *
 */


#define TAMMSG 1100
#define SFT_PORT 45
#define SFTD_BUFFER_DATA 1000

int s1, i, gif;
int id_stc; //id do pacote atual que esta sendo processado.

char msg_rec[TAMMSG];
char msg_send[TAMMSG];
char buff[TAMMSG];

/**
 * \brief cmd_sft Tipos de comandos suportados pelo protocolo SFT.
 * 
 */
typedef enum {
        CMD_SEND =0,
        CMD_SEND_WAIT,
        CMD_STOP,
        CMD_SEND_SEQ,
        CMD_FINISHED,
        CMD_SEND_X,
        CMD_SENDU,
        CMD_ERROR,
        CMD_MSG
} cmd_sft;

/**
 * \brief sft_hdr Estrutura do pacote SFT.
 * Todos foram acrescentados 1 caracter para ser o \0.
 * A struct com o protocolo é enviada por parametro via socket.
 */
struct sft_hdr{
        char cmd[3];
        char len[5];
        char mf[2];
        char nro_pkt[3];
        char id[4];
        char buff_send[SFTD_BUFFER_DATA+1];

};



//declaração ;-)
int main(int argc, char *argv[]);
int sft_send_x(char* data);
int sft_sendu(char* data);
int sft_send_seq(char * data);
int sft_send_loop(char * data);

int send_pkt(void);
int sft_send(char* data);
int send_stop(void);
int trata_sft();
