#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

int main(){
    const char *host="0";
    const char *port="8085";
    struct addrinfo hint,*server;

    printf("configuring host\n");
    memset(&hint,0,sizeof(struct addrinfo));
    hint.ai_family=AF_INET;
    hint.ai_socktype=SOCK_STREAM;
    int r=getaddrinfo(host,port,&hint,&server);
    if(r!=0){
        perror("error host");
        exit(1);
    }
    puts("done with host config");
    int sockfd=socket(
        server->ai_family,
        server->ai_socktype,
        server->ai_protocol
    );
    if(sockfd==-1){
        perror("couldnt create the server socket");
        exit(1);
    }
    puts("server socket created");
    r=bind(
        sockfd,
        server->ai_addr,
        server->ai_addrlen
    );
    if(r==-1){
        perror("couldnt bind socket with server");
        exit(1);
    }
    puts("binded the server");
    r=listen(sockfd,1);
    if(r==-1){
        perror("couldnt listen");
        exit(1);
    }
    printf("listening");
    freeaddrinfo(server);
    close(sockfd);
}