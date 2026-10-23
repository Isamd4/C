#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

int main(){
    const char *port="8082";
    struct addrinfo hint,*server;
    const char *message="hi Muhammad isa Client";
    const int buf_len=1024;
    char buffer[buf_len];
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
    puts("server configured");
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
    puts("socket done");
    //connect
    r=connect(
        sockfd,
        server->ai_addr,
        server->ai_addrlen
    );
    if(r==-1){
        perror("couldnt connect with server");
        exit(1);
    }
    puts("connected to the udp server");
    //send 
    r=send(
        sockfd,
        message,
        strlen(message),
        0
    );
    if(r<0){
        perror("couldnt send to udp server");
        exit(1);
    }
    puts("message sent to udp server");
    //recive
    r=recv(
        sockfd,
        buffer,
        buf_len,
        0
    );
    if(r>0){
        printf("the bytes recieved %d\n",r);
        for(int i=0;i<r;i++){
            putchar(buffer[i]);
        }
        puts("");
        puts("end of received");
    }
    //close
    freeaddrinfo(server);
    close(sockfd);
    return 0;
}