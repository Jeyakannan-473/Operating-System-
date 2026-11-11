#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <sched.h>
#include <unistd.h>
#include <stdatomic.h>
#include <signal.h>

#define SIZE 5
#define PER 2000

static int NP = 4, NC = 4;
static int buffer[SIZE], in = 0, out = 0;
static sem_t empty, full;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static _Atomic int *seen;

#ifdef NOMUTEX
#define LOCK()
#define UNLOCK()
#else
#define LOCK() pthread_mutex_lock(&lock)
#define UNLOCK() pthread_mutex_unlock(&lock)
#endif

static void *producer(void *arg) {
    long id = (long)arg;
    for (int i = 0; i < PER; i++) {
        int item = (int)(id * PER + i);
        sem_wait(&empty); LOCK();
        int slot = in;
        if (i % 7 == 0) sched_yield();
        buffer[slot] = item; in = (slot + 1) % SIZE;
        UNLOCK(); sem_post(&full);
    }
    return NULL;
}

static void *consumer(void *arg) {
    (void)arg;
    int mine = NP * PER / NC;
    for (int k = 0; k < mine; k++) {
        sem_wait(&full); LOCK();
        int slot = out;
        if (k % 7 == 0) sched_yield();
        int item = buffer[slot]; out = (slot + 1) % SIZE;
        UNLOCK(); sem_post(&empty);
        atomic_fetch_add(&seen[item], 1);
    }
    return NULL;
}

static void on_alarm(int s) { (void)s; puts("TIMEOUT: threads blocked"); exit(2); }

int main(int argc, char **argv) {
    if (argc > 2) { NP = atoi(argv[1]); NC = atoi(argv[2]); }
    signal(SIGALRM, on_alarm); alarm(20);
    
    int total = NP * PER; seen = calloc(total, sizeof(*seen));
    sem_init(&empty, 0, SIZE); sem_init(&full, 0, 0);
    pthread_t p[16], c[16];
    
    for (long i = 0; i < NP; i++) pthread_create(&p[i], NULL, producer, (void *)i);
    for (long i = 0; i < NC; i++) pthread_create(&c[i], NULL, consumer, NULL);
    
    for (int i = 0; i < NP; i++) pthread_join(p[i], NULL);
    for (int i = 0; i < NC; i++) pthread_join(c[i], NULL);
    
    int once = 0, lost = 0, dup = 0; long sum = 0;
    for (int i = 0; i < total; i++) {
        if (seen[i] == 1) once++;
        else if (seen[i] == 0) lost++;
        else { dup++; sum += (long)seen[i] * i; }
    }
    
    printf("%d producers, %d consumers %s: produced %d, consumed exactly once %d, lost %d, duplicated %d, sum %ld (expected %ld)\n",
           NP, NC,
           #ifdef NOMUTEX
           "[NO MUTEX]",
           #else
           "[mutex]",
           #endif
           total, once, lost, dup, sum, (long)total * (total - 1) / 2);
    return 0;
}
