#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
int main(void){
    /* My plan is to have my child process (the slave) execute a linux command that prints its progress,
    ** then feed my master the progress, but not by waiting.
    ** How do I achieve that?
    ** First, I created a script, progress_count.sh, that simulates a command printing its progress:
    ** it prints "Progress: 0%" up to "Progress: 100%" every half second, then "done".
    ** Then the slave marries the pipe write end to its STDOUT_FILENO and execs the script,
    ** so everything the script echoes goes into the pipe.
    ** Then the master reads the pipe without blocking, and should wait for the slave,
    ** but with no hanging (WNOHANG), so it can do other work in between.
    */

    
    // Create the pipe before fork, so both master and slave inherit the two ends
    int pipe_fd[2];
    if (pipe(pipe_fd)!=0){
        int err = errno;
        fprintf(stderr,"Error piping %s\n",strerror(err));
        exit(EXIT_FAILURE);
    }
        printf("Hi i am master, and i am creating a slave später\n");
        // Flush before fork, otherwise if the output is piped (./main | cat), this line is still
        // in the buffer and the slave gets a copy of it
        fflush(stdout);
        // Fork returns -1 on error, 0 in the slave, and the slave's pid in the master
        int child_pid= fork();
        switch(child_pid){
            case -1:
                int err = errno;
                fprintf(stderr,"Error Creating Child Process %s\n",strerror(err));
                close(pipe_fd[0]);
                close(pipe_fd[1]);
                exit(EXIT_FAILURE);
            case 0://child process
                // Slave: I only write, so close the read end
                close(pipe_fd[0]);
                // Marry the pipe write end to STDOUT_FILENO, so the script's echo goes into the pipe
                
                if (dup2(pipe_fd[1],STDOUT_FILENO)==-1){_exit(EXIT_FAILURE);}
                // STDOUT_FILENO now holds the write end open, so pipe_fd[1] can be closed
                close(pipe_fd[1]);
                // Replace the slave with the script. If execlp works, it never returns,
                // the script keeps the same pid and the same file descriptors (so stdout is still the pipe).
                // The path is relative, ./ means the folder I run the program from, not where the binary is.
                // So run it from the project folder (./build/PROCESS_COM_NO_WAIT), from build/ it is not found.
                // The second argument is argv[0] of the script, the script name, and NULL ends the list
                if (execlp("./progress_count.sh","progress_count.sh",NULL) == -1){
                    int err = errno;
                    // stderr was not redirected, so this still goes to the terminal
                    fprintf(stderr,"Couldnt execute Script %s\n",strerror(err));

                }
                // We only get here if execlp failed, so the slave exits with a failure.
                // so just learned that  _exit(EXIT_FAILURE) is the safer call in a forked child, but why?
                // the stdio buffers the slave copied from the master. Here they are empty, because the
                // master flushed before fork, so exit works too
                _exit(EXIT_FAILURE);
                
            default: //Master
                char buf[1024];
                // Master: I only read, so close the write end,
                // otherwise read() never sees end of file, even after the slave is done
                close(pipe_fd[1]);
        
                // But now we have a problem, read is a blocking function,
                // so it is best that I make it non blocking.
                // Either I use fcntl:
                    // get flags:  int flags = fcntl(fd,F_GETFL);
                    // then fcntl(fd,F_SETFL, flags | O_NONBLOCK)
                // OR
                    // I use pipe2, i.e. pipe2(pipe_fd,O_NONBLOCK)
                        // but this has a disadvantage, it puts O_NONBLOCK on both ends, also the write end.
                        // In a scenario of the master being busy, and the pipe buffer is full,
                        // the slave's write returns -1 with EAGAIN instead of waiting, so in my case I go for fcntl,
                        // only on the read end
                int flags = fcntl(pipe_fd[0],F_GETFL);
                if (flags == -1){
                    perror("fcntl get flag error");
                    exit(EXIT_FAILURE);
                }
                if (fcntl(pipe_fd[0],F_SETFL,flags | O_NONBLOCK) == -1){
                    perror("fnctl set flag error");
                    exit(EXIT_FAILURE);
                }
                // Calling waitpid once here, right after fork, does not work: the slave is still running,
                // so it returns 0, and when the slave exits later nobody collects it, so it stays a zombie.
                // So to avoid that, I put it inside the loop
                //waitpid(child_pid,NULL,WNOHANG);
                // Keep checking the pipe for progress, without blocking,
                // until the script is done (end of file) or a real error happens
                int n = 0;
                while(1){
                    // I use WNOHANG, so waitpid returns immediately (with 0) if the slave has not exited yet,
                    // and collects the slave if it has
                    waitpid(child_pid,NULL,WNOHANG);
                    // read returns the number of bytes it got, -1 if there is nothing yet, 0 at end of file
                    n = read(pipe_fd[0],buf,sizeof(buf));
                    if (n>0){
                        printf("Got %s\n",buf);
                    }
                    else if(n==0) {// finished, the script closed its stdout, no more data will arrive

                        break;
                    }
                    else {
                        // Here I can take care of some errors,
                        // I am interested in when no data is there yet, that is EAGAIN, then I keep looping.
                        // Any other error, I break.
                        // Save errno straight away, a later call can overwrite it
                        int err = errno;
                        if(err!= EAGAIN){    
                         break;
                        }
                    }
                    // Sleep 90 ms, so the loop does not spin and take 100% of a CPU core.
                    // sleep() only takes whole seconds, sleep(0.09) would be sleep(0)
                    usleep(90000);

                    // I could also be doing some other things here, lets say, since read does not block

                }

                // Just to make sure we dont have any zombies: the WNOHANG in the loop can miss the slave,
                // if it closed its stdout but has not fully exited yet. This one blocks until it has,
                // which is short, since the pipe is already finished
                waitpid(child_pid,NULL, 0);
                // Done reading, close the read end
                close(pipe_fd[0]);
                
        }
        
    


    return 0;
}
