//para compilar

///gcc -lpthread -o client client_msg.c 


#define _GNU_SOURCE
#include <string.h>
#include <stdlib.h>
#include <unistd.h> //para comando de usleep
#include <ctype.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>


//configuração das threads.
#include <pthread.h>
pthread_t tid1, tid2;

#include "../sft.h"

struct sockaddr_in server;
struct hostent *hp, *gethostbyname();


char msg[TAMMSG];
#define MSG_CLI 150
#define FONT "database.sft"

//arquivo usado para banco de dados.
FILE *dtSft;

//identificador para as mensagens.
int id_msg =0;
int flag =0, tchau, id_send;

int gravar_mensagem(void);
int ler_mensagens(void);
void *menu();
void * gerencia(void);
int monta_pkt(char *aux);
int send_pkt_msg(struct sft_hdr *sft);
int envia_mensagem();

/**
 * \brief Cria um banco de dados para o cliente.
 * 
 */

int cria_database(void){
	
	dtSft = fopen(FONT, "w+");
	if(dtSft == NULL){
		printf("\nNao foi possivel criar um database para o cliente.");
		return -1;
	}
	
	fclose (dtSft);
	return 0;
}


int gera_menu(void){
	int i;
	
	printf("ESCOLHA UMA DAS OPCOES\n");
	printf("0 - Escrever mensagem.\n");
	printf("1 - Ler mensagens do banco de dados.\n");
	//printf("2 - Enviar proxima mensagem.\n");
	printf("2 - Sair do cliente SFT.\n");
	printf(":> ");
	scanf("%d", &i);
	
	return i;
}


void *menu(){
//int menu(){
	int clt;	
	
	do{
		clt = gera_menu();
		
		switch (clt){
			case 0:	//escreve mensagem no servidor.
				if(gravar_mensagem()==-1) 
					printf("Error!!!\n");

				break;
			case 1: //le mensagem do banco de dados.
				ler_mensagens();
				break;
			case 2://sai do serviço.
				printf("Tchau....\n");
				tchau = 1;
				return 0;
				break;
			default: break;
		}
	}while(1);	

	return 0;
}

#if 0	
int envia_mensagem(){	
	int fp_tam, nro=0;
	char * fl_buff='\0';
	char * aux='\0';
	size_t result;
	
	printf("\nBuscnado mensagem...\n");
			
	//configura abertura de arquivo para ser lido a nova mensagem.
	dtSft = fopen(FONT, "r");
			
	if(dtSft == NULL){
		printf("\nerror.");
		return -1;
	}
			
	// obtain file size:
	fseek (dtSft, 0 , SEEK_END);
	fp_tam = ftell (dtSft);
	rewind (dtSft);

	// allocate memory to contain the whole file:
	fl_buff = (char*) malloc (sizeof(char)*fp_tam);
	if (fl_buff == NULL) {fputs ("Memory error",stderr); return -1;}

	// copy the file into the buffer:
	result = fread (fl_buff,1,fp_tam,dtSft);
	if (result != fp_tam) {fputs ("Reading error",stderr); return -1;}
	// terminate
	fclose (dtSft);

	aux = strtok(fl_buff, ":");
	nro = atoi(aux);
			
	while(aux !=NULL){
		aux = strtok(NULL,"\n");
		//printf("%d: %s  - old_id %d", nro, aux, old_id);
		fflush(stdout);
		
		printf("\nNRO: %d NRO_SEND: %d\n", nro, id_send);
		//monta mensagem e envia para o display
		if(nro==id_send){
			monta_pkt(aux);
			id_send++; //proxima vez que for chamada pega a proxima mensagem.
			if(id_send==999) id_send =0;
			break;
		}
				
		aux = strtok(NULL, ":");
		nro = atoi(aux);
		if(aux==NULL) break;    //acabou o texto.
				
	}//enquanto arquivo aberto.
	
	return 0;
}
#endif

/**
 * \brief Funcao principal do Cliente de mensagem
 * 
 * Cria a socket que fara a comunicacao com o UIP.
 * recebe o comando de entrada e envia para ser interpretado pelo protocolo SFT.
 * 
 */ 
int main(int argc, char *argv[])
{
	id_send = 0;
	if (argc < 2) {
		printf("Erro uso correto:\n") ;
		printf("\n    %s <hostname> \n\n", argv[0]) ;
		exit(1) ;
	}

	////////////////////CONFIGURA CONEXÃO COM O DESTINO///////////////////////////////////////////////////////
	s1=socket(AF_INET, SOCK_STREAM, 0);

	if (s1<0) {
		 perror("opening stream socket");
		 exit(1);
	}

	hp = gethostbyname(argv[1]);
	if (hp == 0) {
		fprintf(stderr,"%s: Unkown host\n",argv[1]);
		exit(2);
	}

	server.sin_family = AF_INET;
	server.sin_port = htons(SFT_PORT);
	server.sin_addr = *((struct in_addr *)hp->h_addr);
	memset(&(server.sin_zero), '\0', 8);  // zero the rest of the struct 


	printf("\nConectando...\n") ;
	if (connect(s1, (struct sockaddr *)&server, sizeof server ) < 0) {
		perror("connectando stream socket");
		exit(1);
	}

	printf("\nConectado com sucesso...\n") ;
	id_stc =0; //inicaliza id das mensagens;
	
	//coloca o arquivo como ocupado.
	flag = 1;
	
	//flag para terminar o programa.
	tchau =0;
	//cria database
	if(cria_database()==-1) {
		printf("\nerror!!!\n");
		return 0;
	}
	
	system("clear");			
	
	//menu();
	pthread_create(&tid1,NULL,menu, NULL);
	pthread_create(&tid2,NULL,gerencia, NULL);

	pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);


	return 0;
	
}
	
	
int gravar_mensagem(void){
	char data[20];

	dtSft = fopen(FONT, "a");
	if(dtSft == NULL){
		printf("\nerror\n");
		return -1;
	}

	//coloca o arquivo como ocupado.
	flag = 1;
	
	//ADICIONA O ID A MENSAGEM
	sprintf(data, "%d: ", id_msg);
	fputs(data, dtSft);
	id_msg++;

	system("clear");		
	
	printf("Escreva a sua mensagem.");
	printf("\n:> ");
	//pega a informação do terminal.
	fgets_unlocked(msg, TAMMSG, stdin);//não pergunte como, mas funciona.... Não tente arrumar ;-)
	scanf("%[^\n]s", msg);
	setbuf(stdin, NULL);//tira o \0 do ultimo caracter. SUJEIRA
	
	//GRAVA A MENSAGEM DIGITADA.
	fputs (msg,dtSft);
	fputc ('\n',dtSft);
	
	fclose (dtSft);
	
	//libera o arquivo para ser usado por outro processo.
	system("clear");		
	flag = 0;
	return 0;
}
	
int ler_mensagens(void){
	char *base;
	size_t len= TAMMSG;
	
	flag = 1;
	dtSft = fopen(FONT, "r");
	if(dtSft == NULL){
		printf("\nerror\n");
		return -1;
	}
	
	system("clear");		
	printf("Mensagens:\n");
	printf("-----------------------------------------------------------------\n");
	
	base = malloc(TAMMSG);

	while (getline(&base, &len, dtSft) > 0){
		printf("%s\n", base);
	}
	
	if (base)
		free(base);
		
	printf("-----------------------------------------------------------------\n\n\n\n");
	fclose(dtSft);
	
	flag = 0;
	return 0;
}	
	

/**
 * \brief Monta o pacote para envio de pacote
 * 
 * \param aux Mensagem a ser enviada.
 */	
int monta_pkt(char *aux){
	struct sft_hdr sf;
	int len, resp;
	
	//ID	
	sprintf(sf.id, "%03d", id_stc);
	id_stc++;
	if (id_stc==999) id_stc = 0;
	
	//comando
	//sprintf(sf->cmd, "%02d", CMD_MSG);
	strcpy(sf.cmd, "08");
		
	//more fragments
	sprintf(sf.mf, "0");
	
		
	//nro_pkt
	sprintf(sf.nro_pkt, "00");
		
	//tamanho do pacote
	len = strlen(aux);
	sprintf(sf.len, "%04d", len);
	
	//mensagem para o display
	sprintf(sf.buff_send, "%s", aux);
	

	resp = send_pkt_msg(&sf);
	
	//printf("... mensagem enviada com sucesso.\n");
	fflush(stdout);

	//se erro, envia o pacote novamente.Apenas 1 vez.
	if(resp== CMD_ERROR){
			//se não esta aguardando um novo pacote.
			resp = send_pkt_msg(&sf);
	}
	if(resp == CMD_STOP) return CMD_STOP;
	if(resp!= CMD_SEND_WAIT) return -1;

	return 0;
}


int send_pkt_msg(struct sft_hdr *sft){
        int rval, cmd;
        struct sft_hdr *rec;
        char id[3], cmd_s[3];

        //envia sctruct.
//      printf("CMD %s\t", sft->cmd);
//      printf("LEN %s\t", sft->len);
//      printf("MF %s\t", sft->mf);
//      printf("ID %s\t", sft->id);
//      printf("NR_PKT %s\n", sft->nro_pkt);

		//printf("Mensagem enviada:\n%s: %s  - old_id %s", sft->id, sft->buff_send, sft->len);
		fflush(stdout);

        send(s1, (struct sft_hdr *)sft, sizeof(struct sft_hdr), 0);

        //limpa struct para novo pacote.
        bzero((struct sft_hdr *)sft, sizeof(struct sft_hdr));

        //espera resposta..
        rval = recv(s1, msg_rec, TAMMSG, 0) ;
	
        //carrega resposta no vetor de dados sft.
        rec = (struct sft_hdr *)msg_rec;

        sprintf(id, "%03d", id_stc);

        fflush(stdout);

        //if (strncmp(rec->id, id,3)!=0) return CMD_ERROR;      //não é o ID do pacote que foi enviado.
        strncpy(cmd_s, rec->cmd, 2);
        cmd_s[3]='\0';
        cmd = strtol(cmd_s, NULL, 0);

        return cmd;
}


void *gerencia(){	
	int fp_tam, old_id = 0, nro=0;
	char * fl_buff='\0';
	char * aux='\0';
	size_t result;
	
	while(1){
		
		//flag serve para informar se o arquivo não está sendo usado para escrita ou se está disponivel.
		if(flag==0){
		
			//verifica se o ID foi alterado.
			if(old_id!=id_msg){	
			
//				printf("\nMensagem sendo lida.\n");
				
				//configura abertura de arquivo para ser lido a nova mensagem.
				old_id = id_msg; //grava o ID como o anterior.
				dtSft = fopen(FONT, "r");
				
				if(dtSft == NULL){
					printf("\nerror.");
					continue;
				}
				
				// obtain file size:
				fseek (dtSft, 0 , SEEK_END);
				fp_tam = ftell (dtSft);
				rewind (dtSft);

				// allocate memory to contain the whole file:
				fl_buff = (char*) malloc (sizeof(char)*fp_tam);
				if (fl_buff == NULL) {fputs ("Memory error",stderr); continue; /*return -1;*/}

				// copy the file into the buffer:
				result = fread (fl_buff,1,fp_tam,dtSft);
				if (result != fp_tam) {fputs ("Reading error",stderr); continue;/*return -1;*/}
				// terminate
				fclose (dtSft);

				aux = strtok(fl_buff, ":");
				
				while(aux !=NULL){
					nro = atoi(aux);
					aux = strtok(NULL,"\n");
					//printf("%d: %s  - old_id %d", nro, aux, old_id);
					fflush(stdout);
					
					//monta mensagem e envia para o display
					if(nro==(old_id-1)){
						monta_pkt(aux);
						break;
					}
					
					aux = strtok(NULL, ":");
					if(aux==NULL) break;    //acabou o texto.
					
				}//enquanto arquivo aberto.
				
			}//flag ==1
	
		
		}//id's diferentes
		
		if(tchau) break;
	}
	
}

