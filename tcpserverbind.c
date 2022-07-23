#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

int main(){
    const char *host="0";//0 for localhost, but had it being its client then i can say like google.com
    const char *port="8080";// for client could be only 80
    struct addrinfo hint,*server;

    printf("Configuring host \n");
    memset(&hint,0,sizeof(struct addrinfo));
    hint.ai_family=AF_INET;
    hint.ai_socktype=SOCK_STREAM;
    int r= getaddrinfo(host,port,&hint,&server);
    if(r!=0){
        perror("failed");
        exit(1);
    }
    puts("done");
    int sockfd=socket(
        server->ai_family,
        server->ai_socktype,
        server->ai_protocol
    );
    if(sockfd==-1){
        perror("couldnt create socket");
        exit(1);
    }
    puts("done creating socket");

    int binding=bind(sockfd,server->ai_addr,server->ai_addrlen);
    if(binding==-1){
        perror("unable to bind");
        exit(1);
    }
    puts("binded");
    /*
    For tcp client it uses connect
    int connecting= connect(sockfd,server->ai_addr,server->ai_addrlen);
    */
    freeaddrinfo(server);
    close(sockfd);
}