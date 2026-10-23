#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

int main(){
	const char *host = "pool.ntp.org";
	const char *port = "123";
    struct addrinfo hint,*server;
    const int buffer_length=1024;
    char buffer[buffer_length];
    //getaddrinfo
    memset(&hint,0,sizeof(struct addrinfo));
    hint.ai_family=AF_INET;
    hint.ai_socktype=SOCK_STREAM;
    int r=getaddrinfo(host,port,&hint,&server);
    if(r!=0){
        perror("error host");
        exit(1);
    }
    puts("done with host");
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
    //connect
    r=connect(sockfd,
    server->ai_addr,
    server->ai_addrlen
    );
    puts("connected");
    //send and receive
    r=read(sockfd,buffer,buffer_length);
    if(r>0){
        printf("printing from the %d \n",r);
        for(int i=0;i<r;i++){
            putchar(buffer[i]);
        }
    }
    //close
    freeaddrinfo(server);
    close(sockfd);
    return 0;
}