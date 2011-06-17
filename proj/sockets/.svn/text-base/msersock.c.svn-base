/*****************************************************
 Programa para testar comunicação usando sockets
 **  servidor **

 Versao multiplataforma (Funciona e Linux e em Windows)
 Suporta varios clientes simultaneamente

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
#include <arpa/inet.h>
#define closesocket close
#else
#include <winsock2.h>
#endif

#define PORT 5555
#define MAXMSG 512
#define MAXCLI 64
/* Rotina geral de tratamento de erro fatal
 escreve a mensagem e termina o programa */
void erro_fatal(char *mensagem) {
	perror(mensagem);
	exit(EXIT_FAILURE);
}

/*Função cria socket para o servidor*/
int cria_soquete_serv(int porta) {
	int sock, b;
	struct sockaddr_in nome;

	/* Cria um soquete */
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock < 0)
		erro_fatal("socket");

	/* Preenche os campos da estrutura tipo struct sockaddr_in */
	nome.sin_family = AF_INET; /* Seleciona protocolo IPv4 */ //AF_INET - para conexoes com protocolos internet
	nome.sin_port = htons(porta); /* Numero da porta de serviço	*/
	nome.sin_addr.s_addr = htonl(INADDR_ANY); /* Aceita conexões vindas de qualquer IP */

	/* bind: Liga um socket a uma porta de servico */
	b = bind(sock, (struct sockaddr *) &nome, sizeof(nome));//bind - junta a socket com a estrutura.
	if (b < 0)
		erro_fatal("bind");

	/* Libera para atender conexoes na porta */
	/* listen(int socket, int tamanho_da_fila); */
	if (listen(sock, 1) < 0)
		erro_fatal("socket");
	return sock;
}

/* Envia uma mensagem atraves do socket */
int envia_pro_cliente(int filedes, char *msg) {
	int nbytes;

	nbytes = send(filedes, msg, strlen(msg) + 1, 0);
	if (nbytes < 0)
		erro_fatal("send");
	return nbytes;
}

/* Apenas a rotina atende_o_cliente deve ser modificada
 para acrescentar novas funcionalidades ao servidor.
 * Le uma solicitacao do cliente.
 * Se fracassou retorna -1.
 * Se ok, processa a solicitacao, e prepara uma resposta
 * Retorna o numero de bytes recebidos
 */
int atende_o_cliente(int sock) {
	char buffer[MAXMSG]; /* Buffer para armazenar a mensagem recebida */
	char msgg[MAXMSG];
	int nbytes; /* Numer de bytes recebidos */

	nbytes = recv(sock, buffer, MAXMSG - 1, 0); /* Le a mensagem recebida */
	if (nbytes <= 0)
		return -1; /* Retorna -1 em caso de erro */

	printf("Recebi do cliente #%d:\n%s\n", sock, buffer); /* mensagem local */
	sprintf(msgg,"Vc eh o cliente: %d", sock);
	envia_pro_cliente(sock,msgg ); /* Resposta para o cliente */

	return nbytes; /* Se OK retorna o numero de bytes recebido */
}

int numcli; /* Numero de clientes */

/* Versão multi-cliente */
int main() {

	int sock_ini, fd_sock;
	int i, erro;
	struct sockaddr_in clientname;
	int cli[MAXCLI]; /* Lista dos Handles dos sockets dos clientes */

	fd_set ativo_fd_set, le_fd_set;
	int size;

	/* Esta inicializacao e' necessaria no Windows */
#ifndef unix
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(1, 1), &wsaData)) {
		fputs("Erro no WAStartup\n", stderr);
	}
#endif

	/* Cria um socket para receber o contato inicial com um cliente */
	sock_ini = cria_soquete_serv(PORT);

	printf("Socket inicial: handle=%d\n", sock_ini);

	FD_ZERO(&ativo_fd_set);

	FD_SET(sock_ini, &ativo_fd_set);
	numcli = 0;
	do {
		/* A funcao select espera que algo aconteca nos sockets */
		le_fd_set = ativo_fd_set;
		erro = select(FD_SETSIZE, &le_fd_set, NULL, NULL, NULL);
		if (erro < 0)
			erro_fatal("select");

		/* Entra neste if se esta' chegando um novo cliente */
		if (FD_ISSET(sock_ini, &le_fd_set)) {
			if (numcli >= MAXCLI) {
				fputs("Ultrapassou o numero maximo de clientes.", stderr);
				continue;
			}

			size = sizeof(clientname);

			/* Aceita a conexao e cria um socket para comunicar-se com este cliente */
			fd_sock = accept(sock_ini, (struct sockaddr *) &clientname, &size);

			if (fd_sock < 0)
				erro_fatal("accept");

			printf("Servidor: aceitei conexão do host \"%s\", handle=%d\n",
					inet_ntoa(clientname.sin_addr), fd_sock);

			FD_SET(fd_sock, &ativo_fd_set);

			cli[numcli] = fd_sock; /* Acrescenta na lista */
			numcli++; /* Um cliente a mais */

			printf("Agora tem %d clientes\n", numcli);
			continue;
		} //para quando tem clientes novos,
		//parte comum para todos os clientes.

		/* Aqui o cliente ja tem uma conexao estabelecida */
		/* Verifica qual cliente desbloqueou o select */
		for (i = 0; i < numcli; i++) {
			if (!FD_ISSET(cli[i], &le_fd_set))
				continue;
			/* Quando identificou o cliente, verifica o que ele quer */
			erro = atende_o_cliente(cli[i]);

			if (erro < 0) { /* atende_o_cliente retorna negativo quando o cliente saiu */

				closesocket(cli[i]); /* Fecha o socket deste cliente */
				FD_CLR(cli[i], &ativo_fd_set); /* Elimina o cliente da lista */

				printf("O cliente #%d saiu; ", cli[i]);
				numcli--; /* Um cliente a menos */

				printf("Agora tem %d clientes.\n", numcli);

				if (i < numcli) /* Elimina o cliente que saiu da lista de clientes */
					memmove(cli + i, cli + i + 1, (numcli - i) * sizeof(int));
			}
		}
	} while (1);
	return 0;
}

