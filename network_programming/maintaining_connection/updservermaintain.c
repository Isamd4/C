#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>

int main(){
    const char *port="8082";
    struct addrinfo hint,*server;
    const int buf_len=1024;
    char buffer[buf_len];
    struct sockaddr client;
    char host[buf_len];
    char message[buf_len];
    socklen_t client_len=sizeof(client);
    //confiugure
    memset(
        &hint,
        0,
        sizeof(struct addrinfo)
    );
    hint.ai_family=AF_INET;
    hint.ai_socktype=SOCK_DGRAM;
    //getaddrinfo
    int r=getaddrinfo(
        0,
        port,
        &hint,
        &server
    );
    if(r!=0){
        perror("couldnt configuer sever");
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
        perror("couldnt connect the socket");
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
        perror("couldnt bind socket");
        exit(1);
    }
    puts("binded");
   while(1){
     //recv
    r=recvfrom(
        sockfd,
        buffer,
        buf_len,
        0,
        &client,
        &client_len
    );
    if(r>0){
        printf("number of buyte %d\n",r);
        buffer[r]='\0';
        printf("client<< %s",buffer);
    }
    //get name info
    r=getnameinfo(
        &client,
        client_len,
        host,
        buf_len,
        0,
        0,
        NI_NUMERICHOST
    );
    if(r!=0){
        perror("couldnt get name");
        exit(1);
    }
    puts("gotten");
    strcpy(message,host);
    strcat(message,"<<<");
    strcat(message,buffer);
    //send
    r=sendto(
        sockfd,
        message,
        strlen(message),
        0,
        &client,
        client_len
    );
    if(r<0){
        perror("couldnt send to client");
        exit(1);
    }
    puts("sent to client");
   }
    //close
    freeaddrinfo(server);
    close(sockfd);
    return 0;
}