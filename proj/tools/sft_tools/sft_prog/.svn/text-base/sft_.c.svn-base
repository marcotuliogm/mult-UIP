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
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> //para comando de usleep
#include <ctype.h>


#include "sft.h"

struct sockaddr_in server;
struct hostent *hp, *gethostbyname();


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




/** @} */
