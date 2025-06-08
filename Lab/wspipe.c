#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> 
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define BUF_SIZE 2048
#define AC_RED "\x1b[31m"
#define AC_GREEN "\x1b[32m"
#define AC_WHITE "\x1b[37m"

// https://github.com/lattera/glibc/blob/master/string/strcmp.c
int my_strcmp(const char *p1, const char *p2){
  const char *s1 = (const char *) p1;
  const char *s2 = (const char *) p2;
  unsigned char c1, c2;
  
  do
    {
      c1 = (unsigned char) *s1++;
      c2 = (unsigned char) *s2++;
      if (c1 == '\0')
	return c1 - c2;
    }
  while (c1 == c2);

  return c1 - c2;
}

// https://github.com/lattera/glibc/blob/master/string/strstr.c
char* my_strstr(const char* haystack, const char* needle) {
    if (!*needle) return (char*) haystack;

    while (*haystack) {
        const char *h = haystack;
        const char *n = needle;

        while (*h && *n && *h == *n) {
            h++;
            n++;
        }
        if (!*n) return (char*) haystack;
        haystack++;
    }
    return NULL;
}

ssize_t read_line(int fd, char *buffer, size_t max_length) {
    ssize_t num_read = 0;
    char c;
    while (read(fd, &c, 1) == 1 && num_read <  (ssize_t)(max_length - 1)) {
        buffer[num_read++] = c;
        if (c == '\n') break;
    }
    buffer[num_read] = '\0';
    return num_read;
}

void check_highlight_word(const char* line, const char* word, int line_num) {
    const char *p = line;
    printf(AC_GREEN "[%d] " AC_WHITE, line_num);

    while (*p) {
        char *match = my_strstr(p, word);
        if (match) {
            fwrite(p, 1, match - p, stdout);
            printf(AC_RED);
            fwrite(match, 1, strlen(word), stdout);
            printf(AC_WHITE);
            p = match + strlen(word);
        } else {
            printf("%s", p);
            break;
        }
    }
}

void write_result_line(int fd, const char *line, int line_num) {
    char outbuf[BUF_SIZE];
    int len = 0;

    int num_buf[16];
    int num = line_num;
    int num_len = 0;

    while (num > 0){
        num_buf[num_len++] = '0' + num % 10;
        num /= 10;
    }

    outbuf[len++] = '[';
    for (int i = num_len - 1; i >= 0; i--) {
        outbuf[len++] = num_buf[i];
    }
    outbuf[len++] = ']';
    outbuf[len++] = ' ';

    int line_len = strlen(line);
    memcpy(outbuf + len, line, line_len);
    len += line_len;

    write(fd, outbuf, len);
}

int main(int argc, char* argv[]) {
    int pipefd[2];
    pid_t pid;
    char *command, *word;
    int save_enabled = (argc == 4 && my_strcmp(argv[3], "--save") == 0);

    if (argc < 3 || argc > 4 || (argc == 4 && !save_enabled)) {
        fprintf(stderr, "Usage: %s <command> <search_word> [--save]\n", argv[0]);
        return 0;
    }

    int out_fd = -1;
    if (save_enabled) {
        out_fd = open("result.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (out_fd == -1) {
            perror("open");
            exit(1);
        }
    }

    command = argv[1];
    word = argv[2];  

    if (pipe(pipefd) == -1) {
        fprintf(stderr, "Pipe failed");
        return 1;
    }

    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "Fork failed");
        return 1;
    }

    if (pid > 0) {
        close(pipefd[1]);

        char buf[BUF_SIZE];
        int line_num = 1;

        while (read_line(pipefd[0], buf, BUF_SIZE) > 0) {
            if (my_strstr(buf, word)) {
                check_highlight_word(buf, word, line_num);

                if (save_enabled){
                    write_result_line(out_fd, buf, line_num);
                }
            }
            line_num++;
        }

        close(pipefd[0]);
        wait(NULL);
    } else {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        execlp("sh", "sh", "-c", command, NULL);
        exit(1);
    }

    return 0;
}