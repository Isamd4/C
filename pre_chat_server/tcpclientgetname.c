#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>

int main(){
    const char *port="8082";
    struct addrinfo hint,*server;
    const char *message="HI i am client Muhammad";
    const int buf_len=1024;
    char buffer[buf_len];
    //configure sever
    memset(&hint,0,sizeof(struct addrinfo));
    hint.ai_family=AF_INET;
    hint.ai_socktype=SOCK_STREAM;
    //getaddrinfo
    int r=getaddrinfo(
        0,
        port,
        &hint,
        &server
    );
    if(r!=0){
        perror("server not configured");
        exit(1);
    }
    puts("configured server");
    //socket
    int sockfd=socket(
        server->ai_family,
        server->ai_socktype,
        server->ai_protocol
    );
    if(sockfd==-1){
        perror("socket not created");
        exit(1);
    }
    puts("done with socket");
    //connect
    r=connect(
        sockfd,
        server->ai_addr,
        server->ai_addrlen
    );
    if(r==-1){
        perror("socket not connected to server");
        exit(1);
    }
    puts("connected to server");
    //send
    r=send(
        sockfd,
        message,
        strlen(message),
        0
    );
    if(r<0){
        perror("couldnt send message");
        exit(1);
    }
    //receive
    r=recv(
        sockfd,
        buffer,
        buf_len,
        0
    );
    if(r>0){
        printf("number of bytes recieved %d\n",r);
        buffer[r]='\0';
        printf("received : %s\n",buffer);
    }
    puts("end of client");
    //close
    freeaddrinfo(server);
    close(sockfd);

    return 0;
}