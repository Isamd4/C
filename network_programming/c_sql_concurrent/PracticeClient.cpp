#include <stdlib.h>
#include <iostream>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <string.h>

using namespace std;

int main(){
    const char *port="8083";
    struct addrinfo hint,*server;

    memset(&hint,0,sizeof(struct addrinfo));
    hint.ai_family=AF_INET;
    hint.ai_socktype=SOCK_STREAM;
    //addrinfo
    int r=getaddrinfo(0,port,&hint,&server);
    if(r!=0){
        perror("couldnt create server");
        exit(1);
    }
    puts("server");
    int sockfd=socket(
        server->ai_family,
        server->ai_socktype,
        server->ai_protocol
    );
    if(sockfd==-1){
        perror("couldnt initialize socket");
        exit(1);
    }
    puts("socket");
    r=connect(
        sockfd,
        server->ai_addr,
        server->ai_addrlen
    );
    if (r!=0){
        perror("no connection");
        exit(1);
    }
    puts("connected");
    return 0;
}