#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

int main(){
    const char *host="0";
    const char *port="8082";
    //addrinfo and server
    struct addrinfo hint,*server;
    struct sockaddr clientaddr;
    const int buffersize=1024;
    char buffer[buffersize];

   socklen_t client_len= sizeof( clientaddr);
    //getaddrinfo
        //configuring host
        memset(&hint,0,sizeof(struct addrinfo));
        hint.ai_family=AF_INET;
        hint.ai_socktype=SOCK_STREAM;
        int r=getaddrinfo(host,port,&hint,&server);
        if(r!=0){
            perror("couldnt create server");
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
        perror("couldnt create socket");
        exit(1);
    }
    puts("server socket created");
    //connect or bind
    r=bind(sockfd,server->ai_addr,server->ai_addrlen);
    if(r==-1){
        perror("couldnt bind");
        exit(1);
    }
    puts("server and the socket binded");
    //listen 
    r=listen(sockfd,1);
    if(r==-1){
        perror("couldnt listen");
        exit(1);
    }
    puts("listening baby");

    //accept
    int clientfd=accept(
        sockfd,
        &clientaddr,
        &client_len
    );
    if(clientfd==-1){
        perror("couildnt create client");
        exit(1);
    }
    puts("client created ");
    //send and receive
    //send
    r =recv(
        clientfd,
        buffer,
        buffersize,
        0
    );
    if(r>0){
        printf("we have recieved some bytes %d\n",r);
        for(int i=0;i<r;i++){
            putchar(buffer[i]);
        }
    }
    puts("end of buffering");
    //close
    close(clientfd);
    freeaddrinfo(server);
    close(sockfd);
    return 0;
}