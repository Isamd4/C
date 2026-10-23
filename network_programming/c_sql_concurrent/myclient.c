#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
void getAllStudents(int sockfd);
void getStudent(int sockfd);
void updateStudent(int sockfd);
void deleteStudent(int sockfd );
void formatAllStudents(char *students);
void prompting(int sockfd);
char *promptForStudent();
void addStudent(int sockfd);
void sending(char *message,int sockfd);
void getCount();
int main()
{
    const char *port = "8083";
    struct addrinfo hint, *server;
    const char *message = "u16cs1054";
    const int buf_len = 1024;
    char buffer[buf_len];
    // configure server

    memset(&hint, 0, sizeof(struct addrinfo));
    hint.ai_family = AF_INET;
    hint.ai_socktype = SOCK_STREAM;

    // getaddrinfo

    int r = getaddrinfo(0, port, &hint, &server);
    if (r != 0)
    {
        perror("couldnt cofigure server");
        exit(1);
    }
    puts("server configured");
    // socket
    int sockfd = socket(
        server->ai_family,
        server->ai_socktype,
        server->ai_protocol);
    if (sockfd == -1)
    {
        perror("coudlnt create socket");
        exit(1);
    }
    puts("socket created");

    // connect
    r = connect(
        sockfd,
        server->ai_addr,
        server->ai_addrlen);
    if (r == -1)
    {
        perror("coudlnt connect");
        exit(1);
    }
    puts("connected");

    // send
    int entry=2;
    if(entry==1){
         getAllStudents(sockfd);
    }
    else{
        getStudent(sockfd);
    }
    // receive
    r = recv(
        sockfd,
        buffer,
        buf_len,
        0);
    if (r > 0)
    {
        printf("received byte %d\n", r);
        buffer[r] = '\0';
        if(entry==1){
            formatAllStudents(buffer);
        }
        else{
            printf("%s\n",buffer);
        }
    }
    // close
    freeaddrinfo(server);
    close(sockfd);
    return 0;
}
void prompting(int sockfd){
    puts("Welcome to the student record system");
    puts("Press 1 to get all record\nPress 2 to get a student\nPress 3 to insert a student\nPress 4 to edit a student\nPress 5 to delete a student");
    int entry;
    scanf("%d",&entry);
    switch (entry)
    {
    case 1:
        getAllStudents(sockfd);
        break;
    case 2:
        getStudent(sockfd);
        break;
    case 3:
        addStudent(sockfd);
        break;
    case 4:
        updateStudent(sockfd);
        break;
    case 5:
        deleteStudent(sockfd);
        break;
    default:
        break;
    }
}
void getStudent(int sockfd)
{
    //propmt for the student
    char reg_no[10];
    char all[1024]="2@";
    scanf("%s",reg_no);
    // send
    strcat(all,reg_no);
    sending(all,sockfd);
    puts("message sent to serfver");
}
void addStudent(int sockfd){
    char all[1024]="3@";
    char *student=promptForStudent();
    sending(student,sockfd);
    printf("you sent %s\n",student);
}
void deleteStudent(int sockfd){

     char reg_no[10];
    char all[1024]="5@";
    scanf("%s",reg_no);
    // send
    strcat(all,reg_no);
    sending(all,sockfd);
     printf("send delete of %s\n",reg_no);
}
void getAllStudents(int sockfd)
{
     char all[1024] = "1@";
     strcat(all,"hi am muhammad isa");
    sending(all,sockfd);
}
void updateStudent(int sockfd){
    char all[1024]="4@";
    char *student=promptForStudent();
     strcat(all,student);
    sending(student,sockfd);
}
char *promptForStudent(){
    char student[1024];
    char *entry;
    printf("Enter the student reg_no");
    scanf("%s",entry);
    strcpy(student,entry);
    strcat(student,",");
    printf("Enter the student first name");
    scanf("%s",entry);
    strcat(student,entry);
    strcat(student,",");
    printf("Enter the studnent last nane");
    scanf("%s",entry);
    strcat(student,entry);
    strcat(student,",");
    printf("Enter the student Department");
    scanf("%s",entry);
    strcat(student,entry);
    strcat(student,",");
    printf("Enter the student current CGPA");
    scanf("%s",entry);
    strcat(student,entry);
    return student;
}
void formatAllStudents(char *students){
    char s[2]="|";
    char *token=strtok(students,s);
    while(token!=NULL){
        printf("%s\n",token);
        token=strtok(NULL,s);
    }
}
void sending(char *message,int sockfd){
    int r = send(
        sockfd,
        message,
        strlen(message),
        0);
    if (r < 0)
    {
        perror("couldnt send");
        exit(1);
    }
    puts("message sent to serfver");
}