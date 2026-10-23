#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

int main(){
    const char *port="8082";
    struct addrinfo hint,*server;
    struct sockaddr client;
    socklen_t client_len=sizeof(client);
    const int buflen=1024;
    char buffer[buflen];
    char message[buflen];
    //configure server 
    
    memset(&hint,0,sizeof(struct addrinfo));
    hint.ai_family=AF_INET;
    hint.ai_socktype=SOCK_STREAM;

    //getaddrinfo

    int r=getaddrinfo(0,port,&hint,&server);
    if(r!=0){
        perror("couldnt configure server");
        exit(1);
    }
    puts("server done");

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
    puts("socket done");

    //bind

    r=bind(
        sockfd,
        server->ai_addr,
        server->ai_addrlen
    );
    if(r==-1){
        perror("binding error");
        exit(1);
    }
    puts("binded");

    //listen

    r=listen(
        sockfd,
        1
    );
    if(r==-1){
        perror("error listening");
        exit(1);
    }
    puts("i am listening");
    //accept

    while(1){
          int clientfd=accept(
        sockfd,
        &client,
        &client_len
    );
    if(clientfd==-1){
        perror("couldnt accpet");
        exit(1);
    }
    puts("accepted");
    //recv
    r=recv(
        clientfd,
        buffer,
        buflen,
        0
    );
    if(r>0){
        printf("number of bytes rceived %d \n",r);
        buffer[r]='\0';
        printf("client<< %s",buffer);
    }
    //send
    strcpy(message,"server<< ");
    strcat(message,buffer);
    r=send(
        clientfd,
        message,
        strlen(message),
        0
    );
    if(r<0){
        printf("coudlnt send boss");
        exit(1);
    }
    //close
   close(clientfd);
    }
   freeaddrinfo(server);
   close(sockfd);
    return 0;
}