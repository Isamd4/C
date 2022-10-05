#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

int main(){
    const char *port="8082";
    struct addrinfo hint,*server;
    const char *message="u17mt1111";
    const int buf_len=1024;
    char buffer[buf_len];
    //configure server

    memset(&hint,0,sizeof(struct addrinfo));
    hint.ai_family=AF_INET;
    hint.ai_socktype=SOCK_STREAM;

    //getaddrinfo

    int r=getaddrinfo(0,port,&hint,&server);
    if(r!=0){
        perror("couldnt cofigure server");
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
        perror("coudlnt create socket");
        exit(1);
    }
    puts("socket created");

    //connect
    r=connect(
        sockfd,
        server->ai_addr,
        server->ai_addrlen
    );
    if(r==-1){
        perror("coudlnt connect");
        exit(1);
    }
    puts("connected");
    //send
    r=send(
        sockfd,
        message,
        strlen(message),
        0
    );
    if(r<0){
        perror("couldnt send");
        exit(1);
    }
    puts("message sent to serfver");

    //receive
    r=recv(
        sockfd,
        buffer,
        buf_len,
        0
    );
    if(r>0){
        printf("received byte %d\n",r);
        buffer[r]='\0';
        printf("%s\n",buffer);
    }
    //close
    freeaddrinfo(server);
    close(sockfd);
    return 0;
}