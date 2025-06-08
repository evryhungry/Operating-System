#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// 상태 변수와 동기화 도구 선언
static int done = 0;
static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  c = PTHREAD_COND_INITIALIZER;

void thr_exit(void) {
    pthread_mutex_lock(&m);
    done = 1;
    pthread_cond_signal(&c);
    pthread_mutex_unlock(&m);
}

void thr_join(void) {
    pthread_mutex_lock(&m);
    while (done == 0) {
        // done 이 1이 될 때까지 블록
        pthread_cond_wait(&c, &m);
    }
    pthread_mutex_unlock(&m);
}

void *child(void *arg) {
    printf("child: working...\n");
    // ... 자식 작업 수행 ...
    thr_exit(); 
    return NULL;
}

int main(void) {
    pthread_t tid;

    printf("parent: begin\n");
    if (pthread_create(&tid, NULL, child, NULL) != 0) {
        perror("pthread_create");
        exit(1);
    }

    thr_join();  // 자식 끝날 때까지 대기

    printf("parent: end\n");
    return 0;
}