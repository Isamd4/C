#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <mysql/mysql.h>
void connection(MYSQL *conn);
void getStudent(MYSQL *conn, char *reg_no,char *message);
void deleteStudent(MYSQL *conn, char *reg_no,char *message);
void addStudent(MYSQL *conn, char *student,char *message);
void updateStudent(MYSQL *conn, char *reg_no,char *student,char *message);
void decideOperation(MYSQL *conn,char *buffer,char *message);
void getAllStudents(MYSQL *conn,char *message);
int main(){
        //sql connection start
	MYSQL *conn;
	conn = mysql_init(NULL);
	connection(conn);
    //sqlconnection end
    const char *port="8083";
    struct addrinfo hint,*server;
    struct sockaddr client;
    socklen_t client_len=sizeof(client);
    const int buflen=1024;
    char buffer[buflen];
    char message[buflen];
    //configure server 
    
    memset(&hint,0,sizeof(struct addrinfo));
    hint.ai_family=AF_INET;
    hint.ai_socktype=SOCK_STREAM;

    //getaddrinfo

    int r=getaddrinfo(0,port,&hint,&server);
    if(r!=0){
        perror("couldnt configure server");
        exit(1);
    }
    puts("server done");

    //socket

    int sockfd=socket(
        server->ai_family,
        server->ai_socktype,
        server->ai_protocol
    );
    if(sockfd==-1){
        perror("coudlnt create socket");
        exit(1);
    }
    puts("socket done");

    //bind

    r=bind(
        sockfd,
        server->ai_addr,
        server->ai_addrlen
    );
    if(r==-1){
        perror("binding error");
        exit(1);
    }
    puts("binded");

    //listen

    r=listen(
        sockfd,
        1
    );
    if(r==-1){
        perror("error listening");
        exit(1);
    }
    puts("i am listening");
    //accept

    while(1){
          int clientfd=accept(
        sockfd,
        &client,
        &client_len
    );
    if(clientfd==-1){
        perror("couldnt accpet");
        exit(1);
    }
    puts("accepted");
    //recv
    r=recv(
        clientfd,
        buffer,
        buflen,
        0
    );
    if(r>0){
        printf("number of bytes rceived %d \n",r);
        buffer[r]='\0';
        printf("client<< %s\n",buffer);
        decideOperation(conn,buffer,message);
    }
    //send
    r=send(
        clientfd,
        message,
        strlen(message),
        0
    );
    if(r<0){
        printf("coudlnt send boss");
        exit(1);
    }
    //close
   close(clientfd);
    }
    mysql_close(conn);
   freeaddrinfo(server);
   close(sockfd);
    return 0;
}
//connection
void connection(MYSQL *conn){
    	
	char *server = "localhost";
	char *user = "root";
	char *password = ""; /* set me first */
	char *database = "c";
    	/* Connect to database */
	if (!mysql_real_connect(conn, server, user, password, 
                                      database, 0, NULL, 0)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
}
void getStudent(MYSQL *conn,char *reg_no,char *message){
    MYSQL_RES *res;
	MYSQL_ROW row;
	int num_fields;
    char buffer[1024];
    	/* send SQL query */
    printf("%s\n",reg_no);
    strcpy(buffer,"=\'");
    strcat(buffer,reg_no);
    strcat(buffer,"\'");
    char query[]="SELECT *FROM students WHERE reg_no";
    strcat(query,buffer);
	if (mysql_query(conn, query)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
   
	res = mysql_use_result(conn);
    num_fields=mysql_num_fields(res);
	/* output table name */
	if ((row = mysql_fetch_row(res)) != NULL){
        for(int i=0;i<num_fields;i++){
            strcat(message,row[i]);
            strcat(message,",");
        }
    }
    else{
        strcpy(message,"student does'nt exits");
    }

    //closer
    mysql_free_result(res);
}
void deleteStudent(MYSQL *conn,char *reg_no,char *message){
    puts("hi its delete");
}
void addStudent(MYSQL *conn,char *student,char *message){
    puts("its add student");
}
void updateStudent(MYSQL *conn,char *rege_no,char *student,char *message){
    puts("its update menht");
}
void getAllStudents(MYSQL *conn,char *message){
  MYSQL_RES *res;
	MYSQL_ROW row;
	int num_fields;
    char buffer[1024];
    char query[]="SELECT *FROM students";
	if (mysql_query(conn, query)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
    res=mysql_store_result(conn);
    num_fields=mysql_num_fields(res);
    row=mysql_fetch_row(res);
    while(row!=NULL){
         strcat(message,"|");
        for(int i=0;i<num_fields;i++){
             strcat(message,row[i]);
            strcat(message,",");
        }
        row=mysql_fetch_row(res);
    }

    //closer
    mysql_free_result(res);
}
void decideOperation(MYSQL *conn,char *buffer,char *message){
    char *token=strtok(buffer,"@");
    strcpy(message,"server<< ");
    char *student;
    int tok=atoi(token);
    switch (tok)
    {
    case 1:
        getAllStudents(conn,message);
        break;
    case 2:
        token=strtok(NULL,"@");
        getStudent(conn,token,message);
        break;
    case 3:
        token=strtok(NULL,"@");
        addStudent(conn,token,message);
        break;
    case 4:
        token=strtok(NULL,"@");
        student=strtok(NULL,"@");
        updateStudent(conn,token,student,message);
        break;
    case 5:
        token=strtok(NULL,"@");
        deleteStudent(conn,token,message);
        break;
    default:
        break;
    }
}