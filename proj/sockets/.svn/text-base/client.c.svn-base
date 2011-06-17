#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

#define TAMMSG 80

int main(int argc, char *argv[])
{

  int s1;

  struct sockaddr_in server;
  struct hostent *hp, *gethostbyname();

  char msg[TAMMSG] ;

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

  printf("\nTalk>") ;
  
  do {
         scanf("%s", &msg);
         printf("Enviei: %s\n",msg);
         sprintf(msg,"%s\n",msg);
		 send(s1, msg, strlen(msg), 0);
  }while(msg[0] != '.'); 
  
  close(s1);
  exit(0);
 
}
