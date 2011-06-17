#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

#define TAMMSG 80

int main()
{       
    int sock;                                          
    struct sockaddr_in server; 

    struct sockaddr client;
    socklen_t sc = sizeof(struct sockaddr);                               
 
    int j;
      
    int rval, i=0, f, s0;                                          
    char msg[TAMMSG],porta[16] ;
                                                               
    /* Cria socket. */                  

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {                                            
      perror("Abrindo stream socket");                 
      exit(1);                                           
    } 
         
    printf("Porta:"); fflush(stdin);                                                          
    scanf("%15s", porta);
    printf("Servidor esperando mensagens na porta .%s.\n",porta);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;                         
    server.sin_port = htons(atoi(porta)); 
    if (bind(sock, (struct sockaddr *)&server, sizeof server ) < 0) {
        perror("Binding stream socket");                 
        exit(1);
    }


    /*Aguarda conexoes do cliente */ 
       
    listen(sock,10);
    
    
    while (1) {
    
      s0 = accept(sock,&client,&sc);
   
        do {
		
           rval = recv(s0, msg, TAMMSG, 0) ;
           msg[rval] = 0;
		   printf("%d: %s",i,msg);
		   i++;
		} while (msg[0] != '.' && rval != 0); 
   
        close(s0);
        return 0;
      }  
   
   close(sock);
   
 } 
