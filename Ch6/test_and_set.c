#include <stdio.h>
#include <pthread.h>

int CompareAndSwap(int *ptr, int expected, int newval){
    int actual = *ptr;
    if(actual == expected) {
        *ptr = newval;
    }
    return actual;
}

int FetchAndAdd(int *ptr) {
    int old;
    do {
        old = *ptr;                      
    } while (CompareAndSwap(ptr, old, old + 1) != old);
    return old;                           
}


typedef struct __lock_t {
    int ticket;  // 다음 티켓 번호
    int turn;    
} lock_t;

void lock_init(lock_t *lk) {
    lk->ticket = 0;
    lk->turn   = 0;
}

void lock(lock_t *lk) {
    int myturn = FetchAndAdd(&lk->ticket);
    while (lk->turn != myturn)
        ; 
}

void unlock(lock_t *lk) {
    FetchAndAdd(&lk->turn);
}

#define NTHREADS 4
#define NITER    1000000

static int  counter = 0;
lock_t      tlk;

void *worker(void *arg) {
    for(int i = 0; i < NITER; i++) {
        lock(&tlk);
        counter++;
        unlock(&tlk);
    }
    return NULL;
}

int main(void) {
    pthread_t th[NTHREADS];

    lock_init(&tlk);

    for(int i = 0; i < NTHREADS; i++) {
        pthread_create(&th[i], NULL, worker, NULL);
    }
    for(int i = 0; i < NTHREADS; i++) {
        pthread_join(th[i], NULL);
    }

    printf("counter = %d (expected %d)\n",
           counter, NTHREADS * NITER);
    return 0;
}