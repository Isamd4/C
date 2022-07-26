#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

int main(){
    
    const char *port="8082";
    struct addrinfo hint,*server;
    const char *message="Muhammad Isa Hassan";
    const int buf_len=1024;
    char buffer[buf_len];
    //configure server
    memset(&hint,0,sizeof(struct addrinfo));
    hint.ai_family=AF_INET;
    hint.ai_socktype=SOCK_DGRAM;

    //getaddrinfo
    int r=getaddrinfo(0,port,&hint,&server);
    if(r!=0){
        perror("couldnt configure server");
        exit(1);
    }
    puts("server configured");
    //socket
    int sockfd=socket(
        server->ai_family,
        server->ai_socktype,
        server->ai_protocol
    );
    if(sockfd==-1){
        perror("couldnt create socket");
        exit(1);
    }
    puts("socket created");
    //sendto
    r=sendto(
        sockfd,
        message,
        strlen(message),
        0,
        server->ai_addr,
        server->ai_addrlen
    );
    if(r<0){
        perror("couldnt send message");
        exit(1);
    }
    //recvfrom
    r=recvfrom(
        sockfd,
        buffer,
        buf_len,
        0,
        server->ai_addr,
        &server->ai_addrlen
    );
    if(r>0){
        buffer[r]='\0';
        printf("Result : %s\n",buffer);
    }
    //close
    freeaddrinfo(server);
    close(sockfd);

    return 0;
}