#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#define N 5

int ccount = 0;

// SIGCHLD handler that reaps one terminated child 
void child_handler(int sig)
{
    int child_status;
    pid_t pid = wait(&child_status);
    ccount--;
    printf("Received signal %d from process %d\n", sig, pid);
}

// Signal funkiness: Pending signals are not queued
void main()
{
    pid_t pid[N];
    int i;
    ccount = N;

    signal(SIGCHLD, child_handler);
    for (i = 0; i < N; i++)
        if ((pid[i] = fork()) == 0) {
            exit(0);    /* Child: Exit */
        }
    while (ccount > 0)
	    pause();    /* Suspend until signal occurs */
}
