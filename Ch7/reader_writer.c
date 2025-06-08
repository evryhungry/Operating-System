// rwlock.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

typedef struct {
    sem_t lock;        // readers 카운터 보호용
    sem_t writelock;   // read/write 배타 제어용
    int   readers;     // 현재 대기 중인(실행 중인) reader 개수
} rwlock_t;

// rwlock 초기화
void rwlock_init(rwlock_t *rw) {
    sem_init(&rw->lock,      0, 1);   // binary semaphore, 초기값 1
    sem_init(&rw->writelock, 0, 1);   // binary semaphore, 초기값 1
    rw->readers = 0;
}

// reader 진입 (read-lock 획득)
void rwlock_acquire_readlock(rwlock_t *rw) {
    sem_wait(&rw->lock);            // readers 카운터 보호
    rw->readers++;
    if (rw->readers == 1) {
        // 첫 번째 리더라면 writers 배제
        sem_wait(&rw->writelock);
    }
    sem_post(&rw->lock);
}

// reader 나감 (read-lock 해제)
void rwlock_release_readlock(rwlock_t *rw) {
    sem_wait(&rw->lock);
    rw->readers--;
    if (rw->readers == 0) {
        // 마지막 리더가 나가면 writers 허용
        sem_post(&rw->writelock);
    }
    sem_post(&rw->lock);
}

// writer 진입 (write-lock 획득)
void rwlock_acquire_writelock(rwlock_t *rw) {
    sem_wait(&rw->writelock);
}

// writer 나감 (write-lock 해제)
void rwlock_release_writelock(rwlock_t *rw) {
    sem_post(&rw->writelock);
}

// -------------------------------
// 간단한 데모: reader 3명, writer 2명
// -------------------------------
#define N_READERS 3
#define N_WRITERS 2

rwlock_t lock;

void *reader(void *arg) {
    int id = *(int*)arg;
    sleep(1); // staggered start
    printf("Reader %d wants to read\n", id);
    rwlock_acquire_readlock(&lock);
      printf("Reader %d START reading\n", id);
      sleep(2);
      printf("Reader %d END   reading\n", id);
    rwlock_release_readlock(&lock);
    return NULL;
}

void *writer(void *arg) {
    int id = *(int*)arg;
    sleep(1);
    printf("Writer %d wants to write\n", id);
    rwlock_acquire_writelock(&lock);
      printf("Writer %d START writing\n", id);
      sleep(3);
      printf("Writer %d END   writing\n", id);
    rwlock_release_writelock(&lock);
    return NULL;
}

int main(void) {
    pthread_t rth[N_READERS], wth[N_WRITERS];
    int      rids[N_READERS],   wids[N_WRITERS];

    rwlock_init(&lock);

    for (int i = 0; i < N_READERS; i++) {
        rids[i] = i+1;
        pthread_create(&rth[i], NULL, reader, &rids[i]);
    }
    for (int i = 0; i < N_WRITERS; i++) {
        wids[i] = i+1;
        pthread_create(&wth[i], NULL, writer, &wids[i]);
    }

    for (int i = 0; i < N_READERS; i++)
        pthread_join(rth[i], NULL);
    for (int i = 0; i < N_WRITERS; i++)
        pthread_join(wth[i], NULL);

    return 0;
}