#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>

int main(){
    const char *port="8082";
    struct addrinfo hint,*server;
     char message[1024];
    const int buf_len=1024;
    char buffer[buf_len];
    //configure
    memset(&hint,0,sizeof(struct addrinfo));
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
        perror("coudlnt configuer server");
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
    puts("socket creatd");
    while(1){
          //send to
    printf("\nSend message: \n");
    fgets(message,buf_len,stdin);
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
    puts("message sent");
    //recv from
    r=recvfrom(
        sockfd,
        buffer,
        buf_len,
        0,
        server->ai_addr,
        &server->ai_addrlen
    );
    if(r>0){
        printf("mumber of bytes %d\n",r);
        buffer[r]='\0';
        puts(buffer);
    }
    }
    freeaddrinfo(server);
    close(sockfd);
    //close
    return 0;
}