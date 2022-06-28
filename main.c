#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef enum {true,false} bool;
typedef struct Student
{
    int no_classes_attended;
    char reg_num[10];
    char department[20];
    int created;
} Student;
Student addStudent();
void getAllStudents(FILE *filePointer);
int addNewStudent();
int addStudentToFile(FILE* filePointer,Student s);
Student editStudentProcess(FILE* temporary);
Student formatStudentString(char student[]);
Student searchStudent(char regNum[],FILE *filePointer,int mode);
int compareStrings(char one[],char two[]);
void getStudent(FILE *filePointer);
void editStudent();
void deleteStudent();
Student getStudentProcess(FILE *filePointer,FILE *temporary, Student s1);
void printStudent(Student s);
char permanentFile[]="students.txt";
char temporaryFile[]="temporary.txt";
void welcomeMessage(FILE *filePointer);
void markAttendace();
void markAttendaceProcess(FILE *temporaryFile,Student s1);
int main()
{
    FILE *filePointer;
    filePointer=fopen(permanentFile,"r");
    if(filePointer==NULL)
    {
        fclose(filePointer);
        filePointer=fopen(permanentFile,"w");
    }
    fclose(filePointer);
    welcomeMessage(filePointer);
    return 0;
}
 welcomeMessage(FILE *filePointer)
{
   while(1){
     printf("---------------WELCOME TO YOUR STUDENT ATTENDENCE SYSTEM---------\n\n");
    printf("----------Press 1 to Add Student----------\n");
    printf("----------Press 2 to View Student----------\n");
    printf("----------Press 3 to Delete Student----------\n");
    printf("----------Press 4 to Edit Student----------\n");
     printf("----------Press 5 to View All Student----------\n");
     printf("------------Press 6 TO mark Attendance---------\n");
     printf("-----------Press 7 to exit System----------------\n");
    int answer;
    scanf("%d",&answer);
     fflush(stdin);
    switch(answer){
    case 1:
        addNewStudent(filePointer);
        break;
    case 2:
        getStudent(filePointer);
        break;
    case 3:
        deleteStudent();
        break;
    case 4:
        editStudent();
        break;
    case 5:
        getAllStudents(filePointer);
    case 6:
        markAttendace();
    case 7:
         exit(0);
         break;
    default:
        printf("Invalid Input");
    }
   }
}
int addNewStudent(FILE *filePointer)
{
    printf("-----Add the Student ---- \n");
    filePointer=fopen(permanentFile,"a");
    Student s=addStudent();
    int added=addStudentToFile(filePointer,s);
    if(added>=0)
    {
        printf("\nThe studwent is created succesfully");
    }
    else
    {
        printf("Shuru kakeji Alhaji %d",added);
    }
    fclose(filePointer);
    return added;
}
Student addStudent()
{
    Student s1;
    char reg[20];
    char dept[20];
    int noOfAttendance;
    printf("Enter registration number of student: ");
    gets(reg);
    printf("Enter the department: ");
    gets(dept);
    noOfAttendance=1;
    strcpy(s1.reg_num,reg);
    strcpy(s1.department,dept);
    s1.no_classes_attended=noOfAttendance;
    return s1;
}
int addStudentToFile(FILE *filePointer,Student s)
{
    int added=fprintf(filePointer,"%s,%s,%d\n",s.reg_num,s.department,s.no_classes_attended);
    return added;
}
void getAllStudents(FILE *filePointer)
{
    char student[255];
    filePointer=fopen(permanentFile,"r");
    int count=0;
    printf("\n\n..............................................\n");
    printf("Registration Number | Department               | Lectures attended \n");
    fgets(student,255,filePointer);
    while(!feof(filePointer))
    {
        count=count + 1;
        Student s1 =formatStudentString(student);
        printStudent(s1);
        fgets(student,255,filePointer);//reading
    }
    printf("\nTotal Number of Students : %d\n",count);
    fclose(filePointer);
}
Student formatStudentString(char student[255])
{
    Student s1;
    char *token;
    char s[2]=",";
    token=strtok(student,s);
    strcpy(s1.reg_num,token);
    token=strtok(NULL,s);
    strcpy(s1.department,token);
    token=strtok(NULL,s);
    s1.no_classes_attended=atoi(token);
    return s1;
}
Student searchStudent(char regNum[],FILE *filePointer,int mode)
{
    FILE* temporary;
    char studentLine[255];
    temporary=fopen(temporaryFile,"w");
    Student s1;
    fgets(studentLine,255,filePointer);
    while(!feof(filePointer))
    {
        s1= formatStudentString(studentLine);
        if(strcmp(s1.reg_num,regNum)==0 )
        {

            if(mode==0)
            {
                return getStudentProcess(filePointer,temporary,s1);

            }
            else if(mode==1)
            {
                editStudentProcess(temporary);
            }
            else if(mode==3){
                printf("wanna makr\n");
                markAttendaceProcess(temporary,s1);
            }
        }
        else
        {
            addStudentToFile(temporary,s1);
        }
        fgets(studentLine,255,filePointer);
    }
    printf("End of serach\n");
    fclose(temporary);
    if(!fclose(filePointer)==0)
    {
       perror("");
    };
    removingAndRenaming();
    return s1;
}
void getStudent(FILE *filePointer)
{
    filePointer=fopen(permanentFile,"r");
    printf("\n");
     printf("Enter the registration Number of the student : \n");
    char regNum[20];
    gets(regNum);
    searchStudent(regNum,filePointer,0);
       if(fclose(filePointer)!=0)
    {
       perror("Error :");
    };
}
Student getStudentProcess(FILE *filePointer,FILE *temporary,Student s1)
{
    printf("\n--------The student details--------\n");
    printStudent(s1);
    fclose(temporary);
    fclose(filePointer);
    return s1;
}
Student editStudentProcess(FILE *temporary)
{
    fclose(temporary);
    Student s=addStudent();
    int attendance;
    printf("No of attended class \n");
    scanf("%d",&attendance);
    s.no_classes_attended=attendance;
    temporary=fopen(temporaryFile,"a");
    addStudentToFile(temporary,s);
}
void editStudent()
{
    FILE *filePointer=fopen(permanentFile,"r");
    printf("\n");
     printf("Enter the registration Number of the student You want to edit: \n");
    char regNum[20];
    gets(regNum);
    Student s = searchStudent(regNum,filePointer,1);
     printf("The student is edited\n");
}
void markAttendaceProcess(FILE *temporary,Student s1){
    fclose(temporary);
    s1.no_classes_attended=s1.no_classes_attended+1;
    temporary=fopen(temporaryFile,"a");
    addStudentToFile(temporary,s1);
}
void markAttendace(){
     FILE *filePointer=fopen(permanentFile,"r");
    printf("\n");
     printf("Enter the registration Number of the student You want to Mark \n");
    char regNum[20];
    gets(regNum);
    Student s = searchStudent(regNum,filePointer,3);
     printf("Attendance for %s is marked\n",regNum);
}
void deleteStudent()
{
    FILE *filePointer=fopen(permanentFile,"r");
     printf("Enter the registration Number of the student you want to delete: \n");
    char regNum[20];
    gets(regNum);
    searchStudent(regNum,filePointer,2);
    printf("The student is deleted\n");
}
void printStudent(Student s1)
{
    printf("%s            %s             %d\n",s1.reg_num,s1.department,s1.no_classes_attended);
    printf("\n");
}
void removingAndRenaming()
{
    if(remove(permanentFile)!=0)
    {
        perror("remove: ");
    }
    rename(temporaryFile,permanentFile);
}

