#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

int main(){
    struct addrinfo hint,*server;
    const char *port="8082";
    const char* host="0";
    struct sockaddr clientaddr;
    const  int buffer_size=1024;
    char buffer[buffer_size];
    const char *http_data = 
		"HTTP/1.1 200 OK\r\n"
		"Connection: close\r\n"
		"Content-Type: text/html\r\n\r\n"
		"<h1>Hello from your server!</h1>";
    socklen_t clientlen=sizeof(clientaddr);
    printf("configuring server\n");
    //getaddrinfo
    memset(&hint,0,sizeof(struct addrinfo));
    hint.ai_family=AF_INET;
    hint.ai_socktype=SOCK_STREAM;
    int r=getaddrinfo(host,port,&hint,&server);
    if(r!=0){
        perror("couldnt create server");
        exit(1);
    }
    puts("done with the get addr info");
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
    puts("donew with the socket");
    //bind
    r=bind(sockfd,server->ai_addr,server->ai_addrlen);
    if(r==-1){
        perror("couldnt bind");
        exit(1);
    }
    puts("binded the socket with the sever");
    //listen
    r=listen(
        sockfd,
        1
    );
    if(r==-1){
        perror("couldnt listen");
        exit(1);
    }
    puts("i am listening");
    //accept
    int clientfd=accept(
        sockfd,
        &clientaddr,
        &clientlen
    );
    if(clientfd==-1){
        perror("couldnt accept from the client");
        exit(1);
    }
    puts("client addr created");
    //recieve
    r=recv(
        clientfd,
        buffer,
        buffer_size,
        0
    );
    if(r>0){
        printf("the buffer received %d\n",r);
        for(int i=0;i<r;i++){
            putchar(buffer[i]);
        }
    }
    //send
    r=send(clientfd,http_data,strlen(http_data),-0);
    if(r<1){
        perror("couldnt send");
        exit(1);
    }
    puts("finish sending");
    //close
    close(clientfd);
    freeaddrinfo(server);
    close(sockfd);
}