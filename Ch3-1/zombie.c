#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() 
{
    if (fork() == 0) {
        /* Child */
	    printf("Terminating Child, PID = %d\n", getpid());
	    exit(0);
    } else {
	    printf("Running Parent, PID = %d\n", getpid());
	    while (1); /* Infinite loop */
    }

    return 0;
}
