/**
 * \addtogroup tools
 * @{
 */

/**
 * @file sft.c
 *
 * @brief Servidor usado peloprotocolo SFT.
 *
 *
 * Protocolo Desenvolvido para poder fazer transferencia de arquivos entre um servidor local
 * e o UIP.
 * Porta de comunicacao 45
 * 
 *
 *
 * @author Marco Tulio Martins
 *
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>	//para abrir diretorios.
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> //para comando de usleep
#include <ctype.h>


#define TAMMSG 1100
#define SFT_PORT 45
#define SFTD_BUFFER_DATA 1000

int s1, i, gif;
struct sockaddr_in server;
struct hostent *hp, *gethostbyname();
int id_stc; //id do pacote atual que esta sendo processado.

char msg_rec[TAMMSG];
char msg_send[TAMMSG];
char buff[TAMMSG];
struct sft_hdr *sft;

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
	CMD_ERROR
} cmd_sft;

/**
 * \brief sft_hdr Estrutura do pacote SFT. 
 */
struct sft_hdr{
	char cmd[2];
	char len[4];
	char mf;
	char nro_pkt[2];
	char buff_send[SFTD_BUFFER_DATA];

};

//declaração ;-)
int main(int argc, char *argv[]);
int sft_send_x(char* data);
int sft_sendu(char* data);
int sft_send_seq(char * data);
int sft_send_loop(char * data);


/**
 * \brief Envia um pacote com seguranca 
 * 
 * Divide os pacotes em 1000 bytes cada para enviar na rede e aguarda resposta do cliente.
 */ 
int send_pkt(void){
	int rval;
	struct sft_hdr *rec;
	char id[3];
//	char sftptr[1500];

	//envia sctruct.
//	printf("CMD %s\t", sft->cmd);
//	printf("LEN %s\t", sft->len);
//	printf("MF %c\t", sft->mf);
//	printf("ID %s\t", sft->id);
//	printf("NR_PKT %s\n", sft->nro_pkt);

	send(s1, (struct sft_hdr *)sft, sizeof(struct sft_hdr), 0);

	//limpa struct para novo pacote.
	bzero((struct sft_hdr *)sft, sizeof(struct sft_hdr));

	//espera resposta..
	rval = recv(s1, msg_rec, TAMMSG, 0) ;

	//carrega resposta no vetor de dados sft.
	rec = (struct sft_hdr *)msg_rec;

	sprintf(id, "%03d", id_stc);
	//printf("Meu ID: %s  \t ID do Pkt %s", id,rec->id);
	fflush(stdout);
	//if (strncmp(rec->id, id,3)!=0) return CMD_ERROR;	//não é o ID do pacote que foi enviado.
	if (!strncmp(rec->cmd, "01",2)) return CMD_SEND_WAIT;
	if (!strncmp(rec->cmd, "02",2)) return CMD_STOP;

	return -1;	//não retorna estado valido.
}

/**
 * \brief Monta pacotes para ser enviado pelo protocolo SFT
 * 
 * Abre arquivo com o conteudo, coloca no pacote e solicita envio.
 * 
 * \param data Nome do arquivo a ser lido
 */
int sft_send(char* data){
	FILE *fp;
	long fp_tam=0;
	size_t result;
	char* aux='\0';
	char* fp_buff='\0';
	int i=0, vlr[30000], qt_pkt=0, j=0,l=0,m=0, resp=0;
	struct sft_hdr s;

	//obtem os dados do arquivo, grava tudo em fp_buff.
	data = strtok(data, "\n");
	fp = fopen(data, "r");
	if(fp == NULL){
		printf("\nArquivo solicitado não existe na raiz deste diretorio.");
		return -1;
	}

	// obtain file size:
	fseek (fp , 0 , SEEK_END);
	fp_tam = ftell (fp);
	rewind (fp);

	// allocate memory to contain the whole file:
	fp_buff = (char*) malloc (sizeof(char)*fp_tam);
	if (fp_buff == NULL) {fputs ("Memory error",stderr); return -1;}

	// copy the file into the buffer:
	result = fread (fp_buff,1,fp_tam,fp);
	if (result != fp_tam) {fputs ("Reading error",stderr); return -1;}
	// terminate
	fclose (fp);

	//fp_buff contem o arquivo para ser tratado.
	aux = strtok(fp_buff, " ,\"\\");
	//printf("\n> %s", aux);
	i=0;
	sscanf(aux, "0x%x", &vlr[i]);
	while(1){
		i++;
		aux = strtok(NULL, "\\ \",\n");
		if(aux==NULL) break;	//acabou a figura
		//printf("\n> %s", aux);
		sscanf(aux, "0x%x", &vlr[i]);
		//printf(" -  %d\t%d", vlr[i], i);
		fflush(stdout);
	}

	free (fp_buff); //--gdb   (excutar programa) --------------------------------- TLDP.org (tutoriais)


	//vlr é um vetor com todas as posicões da imagem.
	qt_pkt = i/1000;	//total é composto por qt_pkt + 1;
	m = 0;	//m é o contado para o vetor.


	//altera o ID do pacote.
	id_stc++;
	if(id_stc == 999) id_stc = 0;

	//envia os pacotes completos com 1000
	if (qt_pkt > 1){
		for(j=0;j<qt_pkt;j++){
			//printf("\nsend_pkt");
			//monta pacote:
			sprintf(s.cmd, "%02d", CMD_SEND);	//comando
			sprintf(s.len, "%d", SFTD_BUFFER_DATA);	//pacotes dentro deste loop são de 1000
			s.mf = '1';	//more fragments is 1 até o ultimo pacote.
			sprintf(s.nro_pkt, "%02d", j);	//nro do pacote.

			//graa um pacote no buffer a ser enviado..
			for(l=0;l<SFTD_BUFFER_DATA;l++){
				s.buff_send[l]= vlr[m];	//carrego o caracter no buff do pacote.
				fflush(stdout);
				m++;	//m é o contador para o vetor de caracteres da imagem.
			}

			sft = &s;

			//se não esta aguardando um novo pacote.
			resp = send_pkt();

			//se erro, envia o pacote novamente.Apenas 1 vez.
			if(resp== CMD_ERROR){
				sft = &s;
				//se não esta aguardando um novo pacote.
				resp = send_pkt();
			}
			if(resp== CMD_STOP) return CMD_STOP;
			if(resp!= CMD_SEND_WAIT) return -1;

		}
	}

	//se não é maior que um, mas é um pacote envia o resto dos caracteres.
	//envia o resto dos dados.
	j = i%1000;	//pega o resto dos caracteres.
	//monta pacote:
	sprintf(s.cmd, "%02d", CMD_SEND);	//comando
	sprintf(s.len, "%04d", j);	//pacotes com o tamanho do resto dos arquivos.
	s.mf = '0';	//more fragments is 0, ultimo pacote.
	sprintf(s.nro_pkt, "%02d", qt_pkt);	//nro do pacote.
	//grava 0 pacote no buffer a ser enviado..
	//printf("send last pkt");
	for(l=0;l<j;l++){
		s.buff_send[l] = vlr[m];	//carrego o caracter no buff do pacote.
		//printf("s-> %x  vlr-> %x ", s.buff_send[l], vlr[m]);
		m++;	//m é o contador para o vetor de caracteres da imagem.
	}
	sft = &s;
	//se não esta aguardando um novo pacote.
	resp = send_pkt();
	//limpa struct para novo pacote.
	bzero(&s, sizeof(struct sft_hdr));


	//printf("Arquivo enviado com sucesso.\n");
	//se erro, envia o pacote novamente.Apenas 1 vez.
	if(resp== CMD_ERROR){
		sft = &s;
		//se não esta aguardando um novo pacote.
		resp = send_pkt();
	}
	if(resp== CMD_STOP) return CMD_STOP;
	if(resp== CMD_SEND_WAIT) return CMD_SEND_WAIT;

	//send_stop();

	//free (aux);

	return 0;
}

/**
 * \brief Finaliza arquivo enviado
 * 
 * Manda um send_stop quando finaliza o envio de arquivos.
 */ 
int send_stop(void){
	struct sft_hdr s;
	sprintf(s.cmd, "%02d", CMD_STOP);	//comando
	sprintf(s.len, "0");	//pacotes com tamano 0.
	s.mf = '0';	//more fragments é 0, apenas um pacote.
	sprintf(s.nro_pkt, "01");	//nro do pacote.
	s.buff_send[0] = '0';
	//grava o ID do pacote.
	id_stc++;
	if(id_stc == 999) id_stc = 0;

	sft = &s;
//	sprintf(sft->id, "%03d", id_stc);
	send(s1, (struct sft_hdr *)sft, sizeof(struct sft_hdr), 0);
	//resp = send_pkt();
	//printf("\nresp %d", resp);
	//if(resp!=CMD_STOP) return -1;

	return 0;
}

/**
 * \brief Envia uma sequencia de imagens.
 * 
 * Biblioteca de arquivos deve conter imagens numeradas na sequencia 1, 2, 3 ... sem estenção.
 *
 *struct dirent {
    long     d_ino;                 //número de inodes
    off_t    d_off;                 // offset para este dirent
    unsigned short d_reclen;        // tamanho de d_name
    char     d_name[NAME_MAX+1];    // nome do arquivo
	}
 * 
 * \param data equivale a bibliotecas de imagens a ser enviada.
 * \return Retorna 0 se ok e -1 se erro.
 * */
int sft_send_seq(char * data){
	DIR *diretorio;
	char path[30], conf[30];
	FILE *fp;
	long fp_tam;
	size_t result;
	char* fp_buff;
	int qt_arq, i, tempo, tipo;

	//abre diretório da biblioteca só para verificar se é valido, futuramente otimizar para acesso pelo diretório aos arquivos.
	data = strtok(data, "\n");
	if((diretorio = opendir(data))== NULL){
		printf("error opening directory %s\n",data);
		perror("opendir");
		return -1;
	}
	closedir( diretorio ); //fecha diretório

	sprintf(path, "%s/", data);//monta caminho onde está a imagem.
	//printf("\nCaminho: %s", path);
	fflush(stdout);

	//arquivo de configuração.
	sprintf(conf, "%s/%s.conf", data, data);
	fp = fopen(conf,"r");
	if(fp == NULL){
		printf("\nArquivo %s solicitado não existe na raiz deste diretorio.", conf);
		return -1;
	}
	fseek (fp , 0 , SEEK_END);
	fp_tam = ftell (fp);
	rewind (fp);
	fp_buff = (char*) malloc (sizeof(char)*fp_tam);
	result = fread (fp_buff,1,fp_tam,fp);
	fclose (fp);
	//var fp_buff contem os dados do arquivo.

	tempo = atoi(strtok(fp_buff, " ,"));
	qt_arq = atoi(strtok(NULL, " ,"));
	tipo = atoi(strtok(NULL, " ,"));

	//printf("\nTempo: %02d QTD ARQ %d\n", tempo, qt_arq);
	int ll;
	while(1){
		for(i=1;i<=qt_arq;i++){
			sprintf(path, "%s/%d", data, i);//monta caminho onde está a imagem.
			//printf("%s ", path);
			if (sft_send(path)== CMD_STOP){
				//send_stop();
				return CMD_STOP;
			}

			//tempo de espera...
			if(tipo==1) sleep(tempo); //apresentação
			/*GIF*/else if(tipo==2) for(ll=0;ll<(1000000*tempo);ll++);/*usleep(tempo);*/
		}
	}

	return 0;
}

/**
 * \brief Chama o envio de imagens sequenciais.
 * 
 * Chama a funcao sft_send_seq para envio das imagens da biblioteca.
 * Verifica até que o arquivo chegue ao fim eenvia comando de stop, para poder para de enviar arquivos.
 */ 
int sft_send_loop(char * data){
	int resp;
	//while(1){
		resp = sft_send_seq(data);

		//limpa memória.
		//memset(&sft, '\0', sizeof(struct sft_hdr));
		if ( resp == CMD_STOP){
			printf("send stop\n");
			send_stop();
		//	break;
		}
		else if (resp == -1 ) return -1;
		//if(fgetc(stdin)!=EOF) break;
	//}
	return 0;
}


/**
 * \brief Verifica o que foi digitado pedindo.
 * 
 * Trata o pedido de tarefa feita pelo agente externo.
 * 
 * \return Retorna -1 em erro e 0 se ok.
 */ 
int trata_sft(){

	char* tr;
	char* data;
	int estado, j, a;

	//inicializa gif em0.
	gif = 0;
	//pega comando
	tr = strtok(buff, " ,.");//pode ser espaço ou "," ou "."
	//pega dados do arquivo.
	data = strtok(NULL, " ,.");
	j = strlen(tr);
	for(a=0;a<j;a++){	//Passa todo o texto que veio para maiusculo.
		tr[a] = toupper(tr[a]);
		if(tr[a] == ' '){}
	}
	//printf("|%s|", tr);
	if(!strcmp(tr, "SEND")) estado = CMD_SEND;
	else if(!strcmp(tr, "SENDSEQ")) estado = CMD_SEND_SEQ;
	else if(!strcmp(tr, "SENDX")) estado = CMD_SEND_X;
	else if(!strcmp(tr, "SENDU")) estado = CMD_SENDU;
	else return -1;

	switch (estado){
		case CMD_SEND:
			if(sft_send(data)==-1) return -1;
			break;
		case CMD_SEND_SEQ:
			if(sft_send_loop(data)==-1) return -1;
			break;
		case CMD_SEND_X:
			if(sft_send_x(data)==-1) return -1;
			break;
		case CMD_SENDU:
			if(sft_sendu(data)==-1) return -1;
			break;
		default: break;
	}

	memset(sft,'\0', sizeof(sft));
	return 0;
}


/**
 * \brief Funcao principal do Programa
 * 
 * Cria a socket que fara a comunicacao com o UIP.
 * recebe o comando de entrada e envia para ser interpretado pelo protocolo SFT.
 * 
 */ 
int main(int argc, char *argv[])
{

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
    

  if (connect(s1, (struct sockaddr *)&server, sizeof server ) < 0) {
    perror("connectando stream socket");
    exit(1);
  }

  printf("\nSFT Conectado com sucesso...\n") ;
  id_stc = 0;//id do primeiro pacote
  
  do {
         printf("\nSFT>") ;
         fgets(buff,TAMMSG,stdin);//pega a informação do terminal.
         if(strncmp(buff, "quit",4)==0) break;

         if(trata_sft()== -1){
        	 printf("Comando nao existe\n");
        	 continue;
         }



		} while (msg_send[0] != '.');
   
  close(s1);
  exit(0);
 
}


//data = nome do arquivo a ser lido
int sft_send_x(char* data){
	FILE *fp;
	long fp_tam;
	size_t result;
	char* aux;
	char* fp_buff;
	int i, vlr[30000], qt_pkt, j,l,m;
	struct sft_hdr s;

	//obtem os dados do arquivo, grava tudo em fp_buff.
	data = strtok(data, "\n");
	fp = fopen(data, "r");
	if(fp == NULL){
		printf("\nArquivo solicitado não existe na raiz deste diretorio.");
		return -1;
	}

	// obtain file size:
	fseek (fp , 0 , SEEK_END);
	fp_tam = ftell (fp);
	rewind (fp);

	// allocate memory to contain the whole file:
	fp_buff = (char*) malloc (sizeof(char)*fp_tam);
	if (fp_buff == NULL) {fputs ("Memory error",stderr); return -1;}

	// copy the file into the buffer:
	result = fread (fp_buff,1,fp_tam,fp);
	if (result != fp_tam) {fputs ("Reading error",stderr); return -1;}
	// terminate
	fclose (fp);
	//////////////////////////////////////////////////////////////////////////////////////////////

	//fp_buff contem o arquivo para ser tratado.
	aux = strtok(fp_buff, " ,\"\\");
	//printf("\n> %s", aux);
	i=0;
	sscanf(aux, "x%x", &vlr[i]);
	while(1){
		i++;
		aux = strtok(NULL, "\\ \",\n");
		if(aux==NULL) break;	//acabou a figura
		//if (strlen(aux)<2) continue;	//é uma nova linha.
		//printf("\n> %s", aux);
		sscanf(aux, "x%x", &vlr[i]);
		//printf(" -  %d\t%d", vlr[i], i);
		fflush(stdout);
	}
	//while(1);
	//vlr é um vetor com todas as posicões da imagem.
	qt_pkt = i/1000;	//total é composto por qt_pkt + 1;
	m = 0;	//m é o contado para o vetor.


	//grava o ID do pacote.
		id_stc++;
		if(id_stc == 999) id_stc = 0;


	//monta pacote:
	sprintf(s.cmd, "%02d", CMD_SEND_X);	//comando
	//envia os pacotes completos com 1000
	if (qt_pkt > 1){
		for(j=0;j<qt_pkt;j++){
			//printf("\nsend_pkt");
			sprintf(s.len, "1000");	//pacotes dentro deste loop são de 1000
			s.mf = '1';	//more fragments is 1 até o ultimo pacote.
			sprintf(s.nro_pkt, "%02d", j);	//nro do pacote.

			//graa um pacote no buffer a ser enviado..
			for(l=0;l<1000;l++){
				s.buff_send[l]= vlr[m];	//carrego o caracter no buff do pacote.
				fflush(stdout);
				m++;	//m é o contador para o vetor de caracteres da imagem.
			}

			sft = &s;

			//se não esta aguardando um novo pacote.
			if(send_pkt()!= CMD_SEND_WAIT) return -1;

		}
	}

	//se não é maior que um, mas é um pacote envia o resto dos caracteres.
	//envia o resto dos dados.
	j = i%1000;	//pega o resto dos caracteres.
	sprintf(s.len, "%04d", j);	//pacotes com o tamanho do resto dos arquivos.
	s.mf = '0';	//more fragments is 0, ultimo pacote.
	sprintf(s.nro_pkt, "%02d", qt_pkt+1);	//nro do pacote.
	//grava 0 pacote no buffer a ser enviado..
	//printf("send last pkt");
	for(l=0;l<j;l++){
		s.buff_send[l] = vlr[m];	//carrego o caracter no buff do pacote.
		//printf("s-> %x  vlr-> %x ", s.buff_send[l], vlr[m]);
		m++;	//m é o contador para o vetor de caracteres da imagem.
	}
	sft = &s;
	//se não esta aguardando um novo pacote.
	if(send_pkt()!=CMD_SEND_WAIT) return -1;
	printf("Arquivo enviado com sucesso.\n");

	//send_stop();

	//free (fp_buff);
	return 0;
}


/**
 * \brief Monta pacotes para ser enviado pelo protocolo SFT
 * 
 * Abre arquivo com o conteudo, coloca no pacote e solicita envio.
 * Esta funcao nao possui controle de envio nem seguranca na transmissao de arquivos.
 * 
 * \param data Nome do arquivo a ser lido
 */
int sft_sendu(char* data){
	FILE *fp;
	long fp_tam=0;
	size_t result;
	char* aux='\0';
	char* fp_buff='\0';
	int tst[30000];
	int i=0, vlr[30000], qt_pkt=0,l=0,m=0, resp=0;
	struct sft_hdr s;

	//obtem os dados do arquivo, grava tudo em fp_buff.
	data = strtok(data, "\n");
	fp = fopen(data, "r");
	if(fp == NULL){
		printf("\nArquivo solicitado não existe na raiz deste diretorio.");
		return -1;
	}

	// obtain file size:
	fseek (fp , 0 , SEEK_END);
	fp_tam = ftell (fp);
	rewind (fp);

	// allocate memory to contain the whole file:
	fp_buff = (char*) malloc (sizeof(char)*fp_tam);
	if (fp_buff == NULL) {fputs ("Memory error",stderr); return -1;}

	// copy the file into the buffer:
	result = fread (fp_buff,1,fp_tam,fp);
	if (result != fp_tam) {fputs ("Reading error",stderr); return -1;}
	// terminate
	fclose (fp);
	//////////////////////////////////////////////////////////////////////////////////////////////

	//fp_buff contem o arquivo para ser tratado.
	aux = strtok(fp_buff, " ,\"\\");
	//printf("\n> %s", aux);
	i=0;
	sscanf(aux, "0x%x", &vlr[i]);
	while(1){
		i++;
		aux = strtok(NULL, "\\ \",\n");
		if(aux==NULL) break;	//acabou a figura
		//if (strlen(aux)<2) continue;	//é uma nova linha.
		//printf("\n> %s", aux);
		sscanf(aux, "0x%x", &vlr[i]);
		if(i>999){
				//sprintf(&tst[m], "%c", vlr[i]);	//carrego o caracter no buff do pacote.
				tst[m] = vlr[i];
				//printf("tst %x   -  vlr %x", tst[m], vlr[i]);
				//sscanf(aux, "0x%x", tst[m]);
				m++;	//m é o contador para o vetor de caracteres da imagem.
			}
		//printf(" -  %d\t%d", vlr[i], i);
		fflush(stdout);
	}


	free (fp_buff); //--gdb   (excutar programa) --------------------------------- TLDP.org (tutoriais)

	//grava o ID do pacote.
	id_stc++;
	if(id_stc == 999) id_stc = 0;

	//monta primeiro pacote.
	//vlr é um vetor com todas as posicões da imagem.
	qt_pkt = i;	//total é composto por qt_pkt + 1;
	sprintf(s.cmd, "%02d", CMD_SENDU);	//comando
	sprintf(s.len, "%d", SFTD_BUFFER_DATA);	//pacotes dentro deste loop são de 1000
	sprintf(s.nro_pkt, "%02d", 0);	//nro do pacote.
	s.mf = '1';	//more fragments is 1 até o ultimo pacote.
	for(l=0;l<SFTD_BUFFER_DATA;l++){
		s.buff_send[l]= vlr[l];	//carrego o caracter no buff do pacote.
		fflush(stdout);
	}

	sft = &s;
	
	//se não esta aguardando um novo pacote.
	resp = send_pkt();

	//se erro, envia o pacote novamente.Apenas 1 vez.
	if(resp== CMD_ERROR){
		sft = &s;
		//se não esta aguardando um novo pacote.
		resp = send_pkt();
	}
	if(resp== CMD_STOP) return CMD_STOP;
	if(resp!= CMD_SEND_WAIT) return -1;

	//se não veio comando de stop, envia todos o resto do buffer.
	//envia sctruct.

	send(s1, tst, sizeof(tst), 0);



	//se não é maior que um, mas é um pacote envia o resto dos c
	return 0;
}



/** @} */
