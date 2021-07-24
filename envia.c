#include <windows.h>
#include <winsock.h>
#include <stdio.h>
#include <string.h>

#define LOCAL_SERVER_PORT 1501
#define REMOTE_SERVER_PORT 1500
#define MAX_MSG 100

int main(int argc, char *argv[]) {
	int socks, rc, i,k;
    struct sockaddr_in cliAddr, remoteServAddr;
    char nome_arquivo[100];
	char vet_resposta[MAX_MSG][MAX_MSG];

    for(k = 0; k<MAX_MSG; k++)//inicializando o vetor de respostas com strings vazias
        strcpy(vet_resposta[k], "");
	
    WSADATA wsaData;
    LPHOSTENT hostEntry;

   
    
    printf("Digite o nome do arquivo: ");
    scanf("%s", &nome_arquivo);

    // INICIALIZA A DLL DE SOCKETS PARA O WINDOWS
    WSAStartup(MAKEWORD(2,1),&wsaData);

    // VALIDA ENDERECO DE IP RECEBIDO COMO ARGUMENTO
    hostEntry = gethostbyname("127.0.0.1");
    if (hostEntry == NULL){
       printf("Host desconhecido 127.0.0.1\n");
       return 1;
    }

    // VINCULAR A PORTA DO SERVIDOR REMOTO
    remoteServAddr.sin_family = hostEntry->h_addrtype ;
    remoteServAddr.sin_addr = *((LPIN_ADDR)*hostEntry->h_addr_list);
    remoteServAddr.sin_port = htons(REMOTE_SERVER_PORT);	// NUMERO DA PORTA VINDA PELA LINHA DE COMANDO

    // CRIANDO SOCKET
    socks = socket(AF_INET,SOCK_DGRAM,0);
    if(socks < 0) {
        printf("Socket nao pode ser aberto\n");
        return 1;
    }

    /* VINCULAR A PORTA DO CLIENTE */
    cliAddr.sin_family = AF_INET;
    cliAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    cliAddr.sin_port = htons(0);

    rc = bind(socks, (struct sockaddr *) &cliAddr, sizeof(cliAddr));
    if(rc<0) {
        printf("Nao pode vincular a porta\n");
        return 1;
    }

    // ENVIANDO OS DADOS    
    rc = sendto(socks, nome_arquivo, strlen(nome_arquivo)+1, 0,
        (LPSOCKADDR) &remoteServAddr,
        sizeof(struct sockaddr));
    rc = sendto(socks, "FIM", 4, 0,
        (LPSOCKADDR) &remoteServAddr,
        sizeof(struct sockaddr));

    if(rc<0) {
      printf("Nao pode enviar dados %d \n",i-1);
      closesocket(socks);
      return 1;
    }

    closesocket(socks);
    WSACleanup();
    recebe_resposta(vet_resposta);

    for(k=0; k<MAX_MSG; k++){      
        if(strcmp(vet_resposta[k],"") != 0)
            printf("Resposta Recebida: %s Posicao: %d\n", vet_resposta[k], k);
        else break;
    }

    return 0;
}
