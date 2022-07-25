//includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

int main(){
    const char *port="8082";
    int host=0;
    struct addrinfo hint,*server;
    struct sockaddr clientaddr;
    const char *http_data = 
		"HTTP/1.1 200 OK\r\n"
		"Connection: close\r\n"
		"Content-Type: text/html\r\n\r\n"
		"<h1>Hello from your server!</h1>";
    socklen_t clientaddrlen=sizeof(clientaddr);
    const int bufflen=1024;
    char buffer[bufflen];
    //configuring server
    memset(&hint,0,sizeof(struct addrinfo));
    hint.ai_family=AF_INET6;
    hint.ai_socktype=SOCK_STREAM;
    hint.ai_flags=AI_PASSIVE;
    //getaddrifo
    int r=getaddrinfo(0,port,&hint,&server);
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
    puts("socket created");
    //setsockopt
    int option=0;
    r=setsockopt(sockfd,IPPROTO_IPV6,IPV6_V6ONLY,(void *)&option,sizeof(option));
    if(r==-1){
        perror("couldnt make it accept any ip");
        exit(1);
    }
    puts("configuration done");
    //bind
    r=bind(sockfd,server->ai_addr,server->ai_addrlen);
    if(r==-1){
        perror("couldnt bind the socket with server");
        exit(1);
    }
    puts("socket and server binded");
    //listen
    r=listen(sockfd,1);
    if(r==-1){
        perror("couldnt listen");
        exit(1);
    }
    puts("i am listening");
    //accept
    int clientfd=accept(sockfd,&clientaddr,&clientaddrlen);
    if(clientfd==-1){
        perror("couldnt accept client");
        exit(1);
    }
    //receive
    r=recv(
        clientfd,
        buffer,
        bufflen,
        0
    );
    if(r>0){
        printf("the number of messages %d\n",r);
        for(int i=0;i<r;i++){
            putchar(buffer[i]);
        }
    }
    puts("end of recieve");
    //send
    r=send(clientfd,http_data,strlen(http_data),0);
    if(r<0){
        perror("couldt send data");
        exit(1);
    }
    puts("sending data success");
    //close
    close(clientfd);
    freeaddrinfo(server);
    close(sockfd);
}