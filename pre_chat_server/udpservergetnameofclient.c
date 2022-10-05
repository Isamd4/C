#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

int main(){
    const char *port ="8082";
    struct addrinfo hint,*server;
    const int buf_len=1024;
    char buffer[buf_len];
    char message[buf_len];
    struct sockaddr client;
    socklen_t client_len=sizeof(client);
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
    puts("server created");
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
    puts("socke4t done");
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
    puts("socket server binded and listening");
    //receivefrom
    r=recvfrom(
        sockfd,
        buffer,
        buf_len,
        0,
        &client,
        &client_len
    );
    if(r>0){
        printf("you receved %d \n",r);
        buffer[r]='\0';
        printf("client: %s\n",buffer);
    }
    //getnameinfo
    r=getnameinfo(
        &client,
        client_len,
        message,
        buf_len,
        0,
        0,
        NI_NUMERICHOST
    );
    if(r!=0){
        perror("couldnt get the hostname");
        exit(1);
    }
    puts("hostname gotten");
    //sendto
    r=sendto(
        sockfd,
        message,
        strlen(message),
        0,
        &client,
        client_len
    );
    if(r<0){
        perror("couldnt send message");
        exit(1);
    }
    puts("message sent");
    //close
    freeaddrinfo(server);
    close(sockfd);

    return 0;
}