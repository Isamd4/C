#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

int main(){
    const char *port="8082";
    struct addrinfo hint,*server;
    struct sockaddr client_addr;
    socklen_t client_addr_len=sizeof(client_addr);
    const int buf_len=1024;
    char buffer[buf_len];
    const char *message="hi this is your udp server";
    //configure
    memset(&hint,0,sizeof(struct addrinfo));
    hint.ai_family=AF_INET;
    hint.ai_socktype=SOCK_DGRAM;
    //getaddrinfo
    int r=getaddrinfo(0,port,&hint,&server);
    if(r!=0){
        perror("coudnt configure server");
        exit(1);
    }
    puts("UDP server configured");
    //socket
    int sockfd=socket(
        server->ai_family,
        server->ai_socktype,
        server->ai_protocol
    );
    if(sockfd==-1){
        perror("socket not createed");
        exit(1);
    }
    puts("socket created");
    //bind
    r=bind(
        sockfd,
        server->ai_addr,
        server->ai_addrlen
    );
    if(r==-1){
        perror("couldnt bind socket with server");
        exit(1);
    }
    puts("server socket binded and listening");
    //receivefrom
    r=recvfrom(
        sockfd,
        buffer,
        buf_len,
        0,
        &client_addr,
        &client_addr_len
    );
    if(r>0){
        printf("number of bytes received is %d\n",r);
        for(int i=0;i<r;i++){
            putchar(buffer[i]);
        }
        puts("");
        puts("end of recieve");
    }
    //sendto
    r=sendto(
        sockfd,
        message,
        strlen(message),
        0,
        &client_addr,
        client_addr_len
    );
    if(r<0){
        perror("coulndt send message");
        exit(1);
    }
    puts("message sent");
    //close
    freeaddrinfo(server);
    close(sockfd);
    return 0;
}