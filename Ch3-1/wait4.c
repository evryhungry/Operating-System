#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    char *argv[2];
    argv[0] = "./hello";
    argv[1] = NULL;

    int pid1, pid2;
    int status, ret;

    printf("A\n");

    pid1 = fork();
    if (pid1 == 0) {
        // child 1
        printf("B\n");

        pid2 = fork();
        if (pid2 == 0) {
            // child 2
            printf("C\n");
            execvp("./hello", argv);
            // execvp 실패 시 fallback
            perror("execvp failed");
            exit(1);
        } else {
            // child 1 (parent of child 2)
            ret = wait(&status);
            printf("D\n");
            exit(0);
        }
    } else {
        // original parent
        printf("E\n");
        ret = wait(&status);
        printf("F\n");
    }

    return 0;
}