// dining_philosophers_even_odd.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N_PHILOS    5
#define CYCLES      3 
#define THINK_TIME  1
#define EAT_TIME    1

sem_t forks[N_PHILOS];

void think(int id) {
    printf("Philosopher %d is THINKING\n", id);
    sleep(THINK_TIME);
}

void eat(int id) {
    printf("Philosopher %d is EATING\n", id);
    sleep(EAT_TIME);
}

// void *philosopher(void *arg) {
//     int id    = *(int*)arg;
//     int left  = id;                    // 왼쪽 포크 인덱스
//     int right = (id + 1) % N_PHILOS;   // 오른쪽 포크 인덱스

//     while (1) {
//         think(id);

//         if (id % 2 == 0) {
//             sem_wait(&forks[right]);
//             printf("Philosopher %d picked up RIGHT fork (%d)\n", id, right);
//             sem_wait(&forks[left]);
//             printf("Philosopher %d picked up LEFT fork  (%d)\n", id, left);
//         } else {
//             sem_wait(&forks[left]);
//             printf("Philosopher %d picked up LEFT fork  (%d)\n", id, left);
//             sem_wait(&forks[right]);
//             printf("Philosopher %d picked up RIGHT fork (%d)\n", id, right);
//         }

//         eat(id);

//         sem_post(&forks[left]);
//         sem_post(&forks[right]);
//         printf("Philosopher %d put down both forks\n", id);
//     }
//     return NULL;
// }

// 유한의 경우
void *philosopher(void *arg) {
    int id = *(int*)arg;
    int left = id;
    int right = (id + 1) % N_PHILOS;

    for (int i = 0; i < CYCLES; i++) {
        think(id);

        if (id % 2 == 0) {
            sem_wait(&forks[right]);
            sem_wait(&forks[left]);
        } else {
            sem_wait(&forks[left]);
            sem_wait(&forks[right]);
        }

        eat(id);

        sem_post(&forks[left]);
        sem_post(&forks[right]);
    }

    printf("Philosopher %d is done eating %d times.\n", id, CYCLES);
    return NULL;
}

int main(void) {
    pthread_t thr[N_PHILOS];
    int       ids[N_PHILOS];

    // 모든 포크 세마포어 초기값 1
    for (int i = 0; i < N_PHILOS; i++) {
        sem_init(&forks[i], 0, 1);
    }

    // 철학자 스레드 생성
    for (int i = 0; i < N_PHILOS; i++) {
        ids[i] = i;
        pthread_create(&thr[i], NULL, philosopher, &ids[i]);
    }

    // join (실제로는 무한 루프지만 예제이므로)
    for (int i = 0; i < N_PHILOS; i++) {
        pthread_join(thr[i], NULL);
    }
    return 0;
}