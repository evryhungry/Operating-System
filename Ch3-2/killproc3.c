#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define N 5

void main() 
{
    pid_t pid[N];
    int i;
    int child_status;

    for (i = 0; i < N; i++)
	    if ((pid[i] = fork()) == 0) 
	        while(1);   /* Child: Infinite Loop */
	    
    /* Parent terminates the child processes */
    for (i = 0; i < N; i++) {
        printf("Killing process %d\n", pid[i]);
        kill(pid[i], SIGINT);
    }

    /* Parent reaps terminated children */
    for (i = 0; i < N; i++) {
        pid_t wpid = wait(&child_status);
        if (WIFEXITED(child_status))
            printf("Child %d terminated with exit status %d\n",
               wpid, WEXITSTATUS(child_status));
        else
            printf("Child %d terminated abnormally\n", wpid);
    }
}