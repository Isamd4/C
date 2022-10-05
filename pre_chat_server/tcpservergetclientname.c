#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

int main(){

    const char *port="8082";
    struct addrinfo hint,*server;
    struct sockaddr client;
    socklen_t client_len=sizeof(client);
    const int buf_len=1024;
    char buffer[buf_len];
    char message[buf_len];
    //configure
    memset(&hint,0,sizeof(struct addrinfo));
    hint.ai_family=AF_INET;
    hint.ai_socktype=SOCK_STREAM;

    //getaddrinfo
    int r=getaddrinfo(0,port,&hint,&server);
    if(r!=0){
        perror("coudlnt configure server");
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
        perror("coudlnt crfeate socket");
        exit(1);
    }
    puts("socket creaetd");

    //bind
    r=bind(
        sockfd,
        server->ai_addr,
        server->ai_addrlen
    );
    if(r==-1){
        perror("bind failed");
        exit(1);
    }
    puts("server socket binded");

    //listen
    r=listen(sockfd,1);
    if(r==-1){
        perror("coudlnt listend");
        exit(1);
    }
    puts("listening boyya");
    //accept
    int clientfd=accept(
        sockfd,
        &client,
        &client_len
    );
    if(clientfd==-1){
        perror("client not received");
        exit(1);
    }
    puts("client received");

    //getclientname
    r=getnameinfo(
        &client,
        client_len,
        message,
        buf_len,
        0,0,
        NI_NUMERICHOST
    );
    if(r!=0){
        perror("couldnt get host name");
        exit(1);
    }
    puts("host name gotten");
    //send
    r=recv(
        clientfd,
        buffer,
        buf_len,
        0
    );
    if(r>0){
        printf("number of %d\n",r);
        buffer[r]='\0';
        printf("%s\n",buffer);
    }
    //receive
    r=send(
        clientfd,
        message,
        strlen(message),
        0
    );
    if(r<0){
        perror("couldnt send message");
        exit(1);
    }
    puts("messages sent");
    //close
    close(clientfd);
    freeaddrinfo(server);
    close(sockfd);
    return 0;
}