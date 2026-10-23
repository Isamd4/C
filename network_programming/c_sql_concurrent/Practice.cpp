#include <stdlib.h>
#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
using namespace std;

int main(){
    const char *port="8083";
    struct addrinfo hint,*server;
    struct sockaddr client;

    socklen_t client_len=sizeof(client);
    memset(&hint,0,sizeof(struct addrinfo));
    hint.ai_family=AF_INET;
    hint.ai_socktype=SOCK_STREAM;

    //getaddrinfo
    int r=getaddrinfo(0,port,&hint,&server);
    if(r!=0){
        perror("Server not Initialized");
        exit(1);
    }
    puts("Server");
    //socket
    int sockfd=socket(
        server->ai_family,
        server->ai_socktype,
        server->ai_protocol
    );
    if(sockfd==-1){
        perror("Socket not created");
        exit(1);
    }
    puts("socket");
    //bind
    r=bind(sockfd,server->ai_addr,server->ai_addrlen);
    if(r!=0){
        perror("bind failed");
        exit(1);
    }
    puts("binded");
    //listen
    r=listen(sockfd,1);
    if(r==-1){
        perror("couldnt listen");
        exit(1);
    }
    puts("listening");
    //accept
    int clientfd=-1;
    while(clientfd!=0){
        clientfd=accept(
        sockfd,
        &client,
        &client_len
        );
        if(clientfd==-1){
            perror("couldnt accept");
            exit(1);
        }
        puts("accepted");
    }
    //send

    //receive

    //close
    freeaddrinfo(server);
    close(sockfd);
    return 0;
}