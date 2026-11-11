#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>
#include <sched.h>
#define T 4
#define LOOPS 1000000L

volatile long unsafe_count = 0;
volatile long yield_count = 0;
long safe_count = 0; pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
atomic_long atom_count = 0;

void *w(void *a) {
    (void)a;
    for (long i = 0; i < LOOPS; i++) {
        unsafe_count = unsafe_count + 1;
        
        long tmp = yield_count;
        if (i % 50 == 0) sched_yield();
        yield_count = tmp + 1;
        
        pthread_mutex_lock(&m); safe_count++; pthread_mutex_unlock(&m);
        atomic_fetch_add(&atom_count, 1);
    }
    return NULL;
}

int main(void) {
    pthread_t t[T];
    for (int i = 0; i < T; i++) pthread_create(&t[i], NULL, w, NULL);
    for (int i = 0; i < T; i++) pthread_join(t[i], NULL);
    
    printf("expected                                     : %ld\n", (long)T * LOOPS);
    printf("unprotected                                  : %ld (lost %ld updates)\n", unsafe_count, (long)T * LOOPS - unsafe_count);
    printf("unprotected, forced switch inside the update: %ld (lost %ld updates)\n", yield_count, (long)T * LOOPS - yield_count);
    printf("mutex                                        : %ld\n", safe_count);
    printf("atomic                                       : %ld\n", (long)atom_count);
    return 0;
}
