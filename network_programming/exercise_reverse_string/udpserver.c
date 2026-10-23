#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
void mystring(char *str, char *reversed){
      int j=0;
    int i=strlen(str)-1;
    while(i>=0){
        reversed[j]=str[i];
        j++;
        i--;
    }
    reversed[j+1]='\0';
}
int main(){
    
    const char *port="8082";
    struct addrinfo hint, *server;
    struct sockaddr client_addr;
    socklen_t client_addr_len=sizeof(client_addr);
    const int buf_len=1024;
    char buffer[buf_len];
    //configure server

    memset(&hint,0,sizeof(struct addrinfo));
    hint.ai_family=AF_INET;
    hint.ai_socktype=SOCK_DGRAM;

    //getaddrifo
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
        perror("coulndt create socket");
        exit(1);
    }
    puts("socket created");

    //bind
    r=bind(
        sockfd,
        server->ai_addr,
        server->ai_addrlen
    );
    if(r==-1){
        perror("coudlnt bind the server and socket");
        exit(1);
    }
    puts("server socket binded and waiting for a client");

    //recfrom
    
    r=recvfrom(
        sockfd,
        buffer,
        buf_len,
        0,
        &client_addr,
        &client_addr_len
    );
    if(r>0){
        printf("the client sent %d mumber of bytes\n",r);
        buffer[r]='\0';
        printf("recieved %s\n",buffer);
    }
    char reversed[strlen(buffer)+1];
    mystring(buffer,reversed);
    //sendto
    r=sendto(
        sockfd,
        reversed,
        strlen(reversed),
        0,
        &client_addr,
        client_addr_len
    );
    if(r<0){
        perror("couldnt send string");
        exit(1);
    }
    printf("reversed string sent %s",reversed);
    //close
    freeaddrinfo(server);
    close(sockfd);
    return 0;
}