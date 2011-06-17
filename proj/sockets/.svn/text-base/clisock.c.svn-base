/*****************************************************
Programa para testar comunicacao usando sockets
cliente

Versao multiplataforma (Funciona e Linux e em Windows)

Marcos A. Stemmer
-------
Ao fazer o projeto no Dev-cpp acrescente a biblioteca libwsock32.a
	No menu Project
	Submenu Project Options
	Tab	Parameters
	Botao	Add Library Object
	Selecionar o arquivo:
		C/Dev-Cpp/lib/libwsock32.a
 *****************************************************/
#include <stdio.h>

#ifdef unix
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#define closesocket close
#else
#include <winsock2.h>
#endif
#define MAXMSG 512


/* Rotina geral de tratamento de erro fatal
   escreve a mensagem e termina o programa */
void erro_fatal(char *mensagem)
{
	printf("%s:%s\n",mensagem,strerror(errno));
	exit(EXIT_FAILURE);
}


/* Cria um soquete e estabelece a conexão */
int cria_soquete(const char *hostname, int port)
{
	int sock;
	struct hostent *hostinfo;
	struct sockaddr_in sockinfo;

	sock=socket(PF_INET, SOCK_STREAM, 0);
	if(sock < 0) erro_fatal("socket");

	sockinfo.sin_family=AF_INET;

	/* Configura a porta de serviço do socket */
	/* A função htons converte números do host para o formato padrão da internet */
	sockinfo.sin_port=htons(port);

	/* Pede informações a partir do nome do host */
	hostinfo=gethostbyname(hostname);

	if(hostinfo==NULL) {
		fprintf(stderr,"%s: Unknown host\n", hostname);
		exit(EXIT_FAILURE);
	}

	/* Coloca o IP do host no sin_addr */
	sockinfo.sin_addr= *(struct in_addr *)hostinfo->h_addr;

	/* conecta com o host */
	if(connect(sock, (struct sockaddr *)&sockinfo, sizeof(sockinfo)) < 0)
		erro_fatal("Connect (client)");
	return sock;
}

/* Espera pela chegada de dados no soquete */
/* Retorna 0 em caso de timeout */
int espera(int sock, int tempo)
{
	struct timeval timeout;
	fd_set sock_fd_set;
	int r;

	timeout.tv_sec=tempo;
	timeout.tv_usec=0;

	FD_ZERO(&sock_fd_set);
	FD_SET(sock,&sock_fd_set);

	r=select(FD_SETSIZE, &sock_fd_set, NULL, NULL, tempo>0? &timeout: NULL);

	if(r<0)	erro_fatal("select");
	return r;
}

/* Escreve uma mensagem no soquete. Funciona apenas para mensagens de texto.
Para dados crus, use "send" diretamente */
int escreve_soq(int sock, char *mensagem)
{
	int n;

	n = send(sock, mensagem, strlen(mensagem)+1,0);

	if(n < 0) erro_fatal("write");
	return n;
}


/*
Programa principal:
recebe o nome do host na linha de comando
envia uma mensagem e espera receber uma resposta
 */
int main(int argc, char **argv)
{
	int soq, nbytes;
	char *hostname;
	char buf[MAXMSG];

#ifndef unix
	WSADATA wsaData;
	if(WSAStartup(MAKEWORD(1,1),&wsaData)) {
		fputs("Erro no WAStartup\n",stderr);
	}
#endif

	/* O nome do host vem da linha de comando ou é host local "127.0.0.1" */
	hostname = argc > 1? argv[1]: "127.0.0.1";

	/* Cria o soquete com o nome do host e o numero da porta */
	soq = cria_soquete(hostname, 5555);

	do	{
		printf("Mensagem=");
		fgets(buf, MAXMSG, stdin);

		if(*buf < ' ') break;

		/* Envia uma mensagem pro soquete */
		escreve_soq(soq, buf);

		/* Espera pela resposta */
		if(espera(soq,5)==0) erro_fatal("Ninguem responde");

		/* Le a resposta */
		nbytes = recv(soq, buf, MAXMSG, 0);

		if(nbytes<0) erro_fatal("read");

		/* Escreve na saída padrão qual foi a resposta recebida */
		printf("\nRecebi a seguinte resposta do servidor:\n%s\n", buf);
	} while(1);

	closesocket(soq);
	return 0;
}
