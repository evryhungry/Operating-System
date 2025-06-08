#include <stdio.h>
#include <unistd.h>
#include <signal.h>

// Signal handler for timeout
void timeout(int sig)
{
    if (sig == SIGALRM)
        puts("Time out!");
    
    // generate SIGALRM after 2 seconds
    alarm(2);	
}

int main(int argc, char *argv[])
{
    int i;
    struct sigaction act;
    act.sa_handler = timeout;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
   
    // specify signal type and signal handler
    sigaction(SIGALRM, &act, 0);	

    alarm(2);

    for (i = 0; i < 3; i++)
    {
        puts("wait...");
        sleep(10);
    }
    return 0;
}
