// bounded_buffer.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 5
#define PRODUCE_COUNT 10

int buffer[BUFFER_SIZE];
int in = 0;    // 다음에 생산자가 쓸 인덱스
int out = 0;   // 다음에 소비자가 읽을 인덱스


void custom_put(int value){
    buffer[in] = value;
    in = (in + 1) % BUFFER_SIZE ;
    printf("Consumer: got %d from buffer[%d]\n", value, out);
    out = (out + 1) % BUFFER_SIZE;
}

int custom_get(){
    int item = buffer[out];
    printf("Consumer: got %d from buffer[%d]\n", item, out);
    out = (out + 1) % BUFFER_SIZE;
    return item;
}

sem_t empty;   // 빈 슬롯 개수 세마포어
sem_t full;    // 채워진 슬롯 개수 세마포어
sem_t mutex;  // 버퍼 접근 보호용 뮤텍스

void *producer(void *arg) {
    for (int i = 0; i < PRODUCE_COUNT; i++) {
        // 빈 슬롯이 없으면(block) 대기
        sem_wait(&mutex);
        sem_wait(&empty);
        custom_put(i);
    
        sem_post(&full);
        sem_post(&mutex);
    }
    return NULL;
}

void *consumer(void *arg) {
    for (int i = 0; i < PRODUCE_COUNT; i++) {
        int item;

        // 채워진 슬롯이 없으면(block) 대기
        sem_wait(&mutex);
        sem_wait(&full);
        // 버퍼에 접근하기 전 상호배타 구역
        
        item = custom_get();

        // 빈 슬롯 하나 증가
        sem_post(&empty);
        sem_post(&mutex);
        printf("빈 슬롯 찾기: %d \n", item);
    }

    return NULL;
}

int main(void) {
    pthread_t prod_tid, cons_tid;

    // 세마포어 초기화: empty=BUFFER_SIZE, full=0
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full,  0, 0);
    sem_init(&mutex, 0, 1);

    // 뮤텍스 초기화

    // 생산자·소비자 스레드 생성
    pthread_create(&prod_tid, NULL, producer, NULL);
    pthread_create(&cons_tid, NULL, consumer, NULL);

    // 종료 대기
    pthread_join(prod_tid, NULL);
    pthread_join(cons_tid, NULL);

    // 자원 해제
    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);

    return 0;
}