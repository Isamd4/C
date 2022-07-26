#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

int main(){
    const char *port="8082";
    struct addrinfo hint,*server;
    const char *message="Hi muhammad client";
    const int buf_len=1024;
    char buffer[buf_len];
    //configure server
    memset(&hint,0,sizeof(struct addrinfo));
    hint.ai_family=AF_INET;
    hint.ai_socktype=SOCK_DGRAM;
    //getaddrinfo
    int r=getaddrinfo(0,port,&hint,&server);
    if(r!=0){
        perror("server not configured");
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
    //send
    r=sendto(
        sockfd,
        message,
        strlen(message),
        0,
        server->ai_addr,
        server->ai_addrlen
    );
    if(r<0){
        perror("couldnt send to udp server");
        exit(1);
    }
    puts("message sent to udp server");
    //receive
    r=recvfrom(
        sockfd,
        buffer,
        buf_len,
        0,
        server->ai_addr,
        &server->ai_addrlen
    );
    if(r>0){
        printf("number of received %d \n",r);
        for(int i=0;i<r;i++){
            putchar(buffer[i]);
        }
        puts("");
        puts("end of message");
    }
    //close
    freeaddrinfo(server);
    close(sockfd);
    return 0;
}