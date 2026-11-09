#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/errno.h>
#include <sys/wait.h>
#include <stdlib.h>
int main(){
    // I want to play with processes: pid, fork, pipe, dup2 and STDOUT_FILENO.
    // The plan: the slave (child) writes to its stdout as if it was the terminal,
    // whilst the master (parent) reads everything the slave writes.
    // What are my constraints?
    // STDOUT_FILENO is a write only file descriptor. If the slave does printf or write to it,
    // the text goes straight to the terminal, and the master cant read it back with read().
    // So I use a pipe: pipe_fd[1] is the write end, pipe_fd[0] is the read end.
    // In the slave, I marry the pipe write end to STDOUT_FILENO with dup2, so whatever the slave
    // prints goes into the pipe, and the master reads it from pipe_fd[0].

    printf("Hi, I am the master %d\n",getpid());
    // flush before fork, otherwise this line is still in the buffer and the child gets a copy of it
    fflush(stdout);
    // create the pipe before fork, so both master and slave inherit the two ends
    int pipe_fd[2];
    if(pipe(pipe_fd)!=0){
        int err = errno;
        fprintf(stderr,"Error With Pipe Creation %s\n",strerror(err));
        exit(EXIT_FAILURE);
    }
    // fork returns 0 in the slave, and the slave's pid in the master
    pid_t child = fork();
    if (child == 0){
        // Slave: I only write, so lets close the read end here
        if (close(pipe_fd[0])!=0){
            int err = errno;
            fprintf(stderr,"Error With Close Read Pipe from Slave %s\n",strerror(err));
            exit(EXIT_FAILURE);
        }
        // lets marry the write end of the pipe to STDOUT_FILENO.
        // After dup2, fd 1 points to the pipe, so printf and write(STDOUT_FILENO) go into the pipe
        if(dup2(pipe_fd[1],STDOUT_FILENO)==-1){    
            int err = errno;
            fprintf(stderr,"Error With Dup2 %s\n",strerror(err));
            exit(EXIT_FAILURE);
        }
        // Now I can just close pipe_fd[1], STDOUT_FILENO is still holding the write end open
        if (close(pipe_fd[1])!=0){
            int err = errno;
            fprintf(stderr,"Error With Close Write Pipe From Slave %s\n",strerror(err));
            exit(EXIT_FAILURE);
        }

        char message[1024];
        sprintf(message,"Hi, I am the child %d\r",getpid());
        // lets now write the message, one char per second, to our (new) STDOUT_FILENO.
        // Note: stdout is now a pipe, not a terminal, so printf is fully buffered.
        // These lines sit in the buffer until it is flushed, they are not sent every second
        int i = 0;
        while(i < strlen(message)){
            sleep(1);
            printf("Progress %d on char %c\n",i,message[i]);
            i++;
        }

        //Let me the flush to accomodate the write after 
        fflush(stdout);
        // After writing my message,
        // let me also test write() directly on STDOUT_FILENO, this one skips the printf buffer
        // and goes straight into the pipe, so the master gets it before the progress lines.
        // Note: sizeof(done) is 31, but with a 6 digit pid the text is 30 chars, so the '\0' is sent too.
        // A longer pid would not fit in done[31], strlen(done) is the safer size here
        char done[31];
        sprintf(done,"Child %d, is done sending\n",getpid());
        write(STDOUT_FILENO,&done,strlen(done));
        // close the STDOUT_FILENO, this closes the last write end of the pipe,
        // so the master's read() returns 0 (end of file) and its loop ends.
        // Note: the progress lines still in the printf buffer are lost here, because when the
        // slave returns, exit() tries to flush them to a closed fd. fflush(stdout) before the
        // write() above would fix it
        close(STDOUT_FILENO);
    }
    else{
        // Master: I only read
        printf("I want to read from the slave %d\n",getpid());

        // close the write end in the master, otherwise read() never sees end of file,
        // since a write end would still be open even after the slave closes its own
        if (close(pipe_fd[1])!=0){
            int err = errno;
            fprintf(stderr,"Error With Close Write Pipe From master %s\n",strerror(err));
            exit(EXIT_FAILURE);
        }
        char buf;
        // so now lets read the message, one byte at a time, and print it on the real terminal.
        // read() blocks until the slave writes something, and returns 0 when all write ends are closed
        while(read(pipe_fd[0],&buf,1) > 0){
            putchar(buf);
        }
        
        if (close(pipe_fd[0])!=0){
            int err = errno;
            fprintf(stderr,"Error With Close Read Pipe From master %s\n",strerror(err));
            exit(EXIT_FAILURE);
        }
        // wait for the slave to finish, so it does not become a zombie
        waitpid(child,NULL,0);
        printf("Finished Receiving by Master %d\n",getpid());
    }
    return 0;
}
