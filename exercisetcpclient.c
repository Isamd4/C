#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

int main(){
    const char *host="localhost";
    const char *port="8082";
    const char *http_message="My name is Muhammad\n";
    const int bufflen=1024;
    char buffer[bufflen];
    struct addrinfo hint,*server;
    //configure server
    memset(&hint,0,sizeof(struct addrinfo));
    hint.ai_family=AF_INET;
    hint.ai_socktype=SOCK_STREAM;
    //getaddr info
    int r=getaddrinfo(host,port,&hint,&server);
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
        perror("couldnt create socket");
        exit(1);
    }
    puts("socket created");
    //connect
    r=connect(
        sockfd,
        server->ai_addr,
        server->ai_addrlen
    );
    if(r==-1){
        perror("couldnt connnect to server");
        exit(1);
    }
    puts("connnected to server");
    //send
    r=send(
        sockfd,
        http_message,
        strlen(http_message),
        0
    );
    if(r<0){
        perror("message not sent");
        exit(1);
    }
    puts("message sent");
    //receive
    r=recv(
        sockfd,
        buffer,
        bufflen,
        0
    );
    if(r>0){
        printf("the message byte is %d\n",r);
        for(int i=0;i<r;i++){
            putchar(buffer[i]);
        }
        printf("\n");
        puts("message end");
    }
    //close
    freeaddrinfo(server);
    close(sockfd);
    return 0;
}