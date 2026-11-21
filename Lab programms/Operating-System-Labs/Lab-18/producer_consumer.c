/* Lab-18: Producer-Consumer problem using semaphores and a bounded buffer */
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define BUFFER_SIZE 3
#define ITEMS 8

static int buffer[BUFFER_SIZE];
static int in = 0, out = 0;
static sem_t empty_slots;       /* counts empty slots, starts at BUFFER_SIZE */
static sem_t full_slots;        /* counts filled slots, starts at 0 */
static sem_t mutex;             /* protects the buffer */

static void *producer(void *arg)
{
    int item;

    (void)arg;
    for (item = 1; item <= ITEMS; item++) {
        sem_wait(&empty_slots);             /* wait for a free slot */
        sem_wait(&mutex);
        buffer[in] = item;
        printf("Producer produced item %d (slot %d)\n", item, in);
        in = (in + 1) % BUFFER_SIZE;
        sem_post(&mutex);
        sem_post(&full_slots);              /* one more item available */
        usleep(100000);
    }
    return NULL;
}

static void *consumer(void *arg)
{
    int i, item;

    (void)arg;
    for (i = 0; i < ITEMS; i++) {
        sem_wait(&full_slots);              /* wait for an item */
        sem_wait(&mutex);
        item = buffer[out];
        printf("Consumer consumed item %d (slot %d)\n", item, out);
        out = (out + 1) % BUFFER_SIZE;
        sem_post(&mutex);
        sem_post(&empty_slots);             /* one more free slot */
        usleep(250000);
    }
    return NULL;
}

int main(void)
{
    pthread_t prod, cons;

    sem_init(&empty_slots, 0, BUFFER_SIZE);
    sem_init(&full_slots, 0, 0);
    sem_init(&mutex, 0, 1);

    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    sem_destroy(&empty_slots);
    sem_destroy(&full_slots);
    sem_destroy(&mutex);

    printf("All %d items were produced and consumed.\n", ITEMS);
    return 0;
}
