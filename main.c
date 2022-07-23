#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>

int main(){
	const char *host="yahoo.com";
	const char *port="80";
	struct addrinfo hint,*resource;
	printf("Configuring host");
	memset(&hint,0,sizeof(struct addrinfo));
	int r=getaddrinfo(host,port,&hint,&resource);
	if(r!=0){
		perror("Error");
		exit(1);
	}
	puts("done");
	freeaddrinfo(resource);
	return 0;
}