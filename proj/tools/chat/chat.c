/**
 * \addtogroup tools
 * @{
 */

/**
 * @file chat.c
 *
 * @brief Comunicacao usando sockets.
 *
 * Para executar o chat:
 *
 *  Chamar no terminal o chat
 *  	chat <porta>
 *
 * 	Colocar o servidor para executar...
 * 		acesse: cd tools/
 * 		chame o programa ./chat <endereco> <port>
 *
 *
 * @author Marco Tulio Martins
 *
 */


#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TAMMSG 80

/**
 * \brief Funcao que gerencia execucao programa
 *
 *
 * Programa abre uma conexao com socket TCP entre o host e o UIP.
 *
 * Deve ser passado por parametro o hostname e a porta de conexao
 */
int main(int argc, char *argv[])
{

  int s1;

  struct sockaddr_in server;
  struct hostent *hp, *gethostbyname();

  char msg_rec[TAMMSG] ;
  char msg_send[TAMMSG] ;
  int rval;

  if (argc < 3) {
     printf("Erro uso correto:\n") ;
     printf("\n    %s <hostname> <portnumber>\n\n", argv[0]) ;
     exit(1) ;
  }
    
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
  server.sin_port = htons(atoi(argv[2]));
  server.sin_addr = *((struct in_addr *)hp->h_addr);
  memset(&(server.sin_zero), '\0', 8);  // zero the rest of the struct 
    

  if (connect(s1, (struct sockaddr *)&server, sizeof server ) < 0) {
    perror("connectando stream socket");
    exit(1);
  }

  printf("\nConexao Estabelecida.\n") ;
  
  do {

		 //recebe mensagem...
		 printf("\n:>");
		 rval = recv(s1, msg_rec, TAMMSG, 0) ;
         msg_rec[rval] = 0;
		 printf("%s\n",msg_rec);
		 if(strncmp(msg_rec, "quit", "quit")==0) {
			 printf("bye.\n");
//			 send(s1, "quit", strlen("quit"), 0);
			 break;
		 }

         //envia mensagem...
         printf("\nTalk>") ;
         fgets(msg_send,TAMMSG,stdin);//pega a informação do terminal.
         //printf("Enviei: %s\n",msg_send);
         //sprintf(msg,"%s\n",msg);
		 send(s1, msg_send, strlen(msg_send), 0);
		 if(strcmp(msg_send, "quit")==0){
			 printf("bye.\n");
			 send(s1, "quit", strlen("quit"), 0);
			 break;
		 }
		 
  } while (msg_send[0] != '.' && rval != 0);

  printf("\nFim da conexao.\n");
  close(s1);
  exit(0);
 
}

/** @} */
