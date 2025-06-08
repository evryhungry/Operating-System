#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#define N 5

int ccount = 0;

// SIGCHLD handler that reaps all terminated children
void child_handler2(int sig)
{
    int child_status;
    pid_t pid;
    while ((pid = wait(&child_status)) > 0) {
        ccount--;
        printf("Received signal %d from process %d\n", sig, pid);
    }
}


// Using a handler that reaps multiple children
void main()
{
    pid_t pid[N];
    int i;
    ccount = N;

    signal(SIGCHLD, child_handler2);
    for (i = 0; i < N; i++)
        if ((pid[i] = fork()) == 0) {
            sleep(1);
            exit(0); /* Child exits */
        }
    while (ccount > 0);  /* Parent spins */
}