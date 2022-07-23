#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

int main(){
    const char *port="8080";
    const char *host="0";
    struct addrinfo hint,*server;
    printf("configiuring host\n");
    memset(&hint,0,sizeof(struct addrinfo));
    hint.ai_family=AF_INET;
    hint.ai_socktype=SOCK_STREAM;
    int r=getaddrinfo(host,port,&hint,&server);

    if(r!=0){
        perror("error");
        exit(1);
    }
    puts("done addrinfor");

    int sockfd=socket(
        server->ai_family,
        server->ai_socktype,
        server->ai_protocol
    );
    if(sockfd==-1){
        perror("unable to create socket");
        exit(1);
    }
    puts("done creating socket");
    freeaddrinfo(server);
    close(sockfd);
}