#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    int ret;
    char *argv[2];

    argv[0] = "./hello"; // initialize command line arguments for main
    argv[1] = NULL;

    pid = fork();
    if (pid == 0) {    // child process
        ret = execvp("./hello", argv);
        if (ret < 0) {
            perror("Error: execvp failed");
            exit(EXIT_FAILURE);
        }
    } else if (pid > 0) { // parent process
        wait(NULL); // wait for the child process to complete
    } else {
        perror("Error: fork failed");
        exit(EXIT_FAILURE);
    }

    return 0;
}
