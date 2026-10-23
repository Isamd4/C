#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

int main(){
    const char *port="8085";
    const char *host="0";
    struct addrinfo hint,*server;
    struct sockaddr clientadd;
    socklen_t clientLen;
    printf("configuring server\n");
    memset(&hint,0,sizeof(struct addrinfo));
    hint.ai_family=AF_INET;
    hint.ai_socktype=SOCK_STREAM;
    int r=getaddrinfo(host,port,&hint,&server);
    if(r!=0){
        perror("could configure host");
        exit(1);
    }
    puts("done with the server");
    //socket
    int sockfd=socket(
        server->ai_family,
        server->ai_socktype,
        server->ai_protocol
    );
    if(sockfd==-1){
        perror("couldnt configure socket");
        exit(1);
    }
    puts("sokcet done");
    //bind
    r=bind(sockfd,server->ai_addr,server->ai_addrlen);
    if(r==-1){
        perror("couldnt bind the socket with server");
        exit(1);
    }
    puts("binded");
    
    r=listen(sockfd,1);
    if(r==-1){
        perror("error listening");
        exit(1);
    }
    puts("listening");
    
    //accepting
    clientLen=sizeof(clientadd);
    int clientfd=accept(sockfd,&clientadd,&clientLen);
    if(clientfd==-1){
        perror("couldnt accept");
        exit(1);
    }
    puts("hi accepted");
    close(clientfd);
    freeaddrinfo(server);
    close(sockfd);
}