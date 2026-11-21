/* Lab-19: Process synchronization using mutex locks - threads updating a shared counter */
#include <pthread.h>
#include <stdio.h>

#define NUM_THREADS 4
#define INCREMENTS 100000

static long counter = 0;
static pthread_mutex_t lock;

static void *increment(void *arg)
{
    int i;

    (void)arg;
    for (i = 0; i < INCREMENTS; i++) {
        pthread_mutex_lock(&lock);          /* enter critical section */
        counter++;
        pthread_mutex_unlock(&lock);        /* leave critical section */
    }
    return NULL;
}

int main(void)
{
    pthread_t thread[NUM_THREADS];
    int i;

    pthread_mutex_init(&lock, NULL);

    for (i = 0; i < NUM_THREADS; i++)
        pthread_create(&thread[i], NULL, increment, NULL);
    for (i = 0; i < NUM_THREADS; i++)
        pthread_join(thread[i], NULL);

    pthread_mutex_destroy(&lock);

    printf("Number of threads      : %d\n", NUM_THREADS);
    printf("Increments per thread  : %d\n", INCREMENTS);
    printf("Expected counter value : %d\n", NUM_THREADS * INCREMENTS);
    printf("Final counter value    : %ld\n", counter);

    return 0;
}
