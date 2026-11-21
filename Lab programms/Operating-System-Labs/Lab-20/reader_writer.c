/* Lab-20: Reader-Writer problem using semaphores (readers have preference) */
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define NUM_READERS 3
#define NUM_WRITERS 2
#define ROUNDS 3

static sem_t mutex;             /* protects readers_count */
static sem_t write_block;       /* gives a writer (or the group of readers) exclusive access */
static int data = 0;
static int readers_count = 0;

static void *reader(void *arg)
{
    int id = *(int *)arg;
    int i;

    for (i = 0; i < ROUNDS; i++) {
        sem_wait(&mutex);
        readers_count++;
        if (readers_count == 1)
            sem_wait(&write_block);         /* first reader locks out writers */
        sem_post(&mutex);

        printf("Reader %d reads data = %d\n", id, data);
        usleep(50000);

        sem_wait(&mutex);
        readers_count--;
        if (readers_count == 0)
            sem_post(&write_block);         /* last reader lets writers in */
        sem_post(&mutex);

        usleep(100000);
    }
    return NULL;
}

static void *writer(void *arg)
{
    int id = *(int *)arg;
    int i;

    for (i = 0; i < ROUNDS; i++) {
        sem_wait(&write_block);
        data++;
        printf("Writer %d writes data = %d\n", id, data);
        usleep(50000);
        sem_post(&write_block);

        usleep(150000);
    }
    return NULL;
}

int main(void)
{
    pthread_t readers[NUM_READERS], writers[NUM_WRITERS];
    int reader_id[NUM_READERS], writer_id[NUM_WRITERS];
    int i;

    sem_init(&mutex, 0, 1);
    sem_init(&write_block, 0, 1);

    for (i = 0; i < NUM_READERS; i++) {
        reader_id[i] = i + 1;
        pthread_create(&readers[i], NULL, reader, &reader_id[i]);
    }
    for (i = 0; i < NUM_WRITERS; i++) {
        writer_id[i] = i + 1;
        pthread_create(&writers[i], NULL, writer, &writer_id[i]);
    }

    for (i = 0; i < NUM_READERS; i++)
        pthread_join(readers[i], NULL);
    for (i = 0; i < NUM_WRITERS; i++)
        pthread_join(writers[i], NULL);

    sem_destroy(&mutex);
    sem_destroy(&write_block);

    printf("Final value of data = %d\n", data);
    return 0;
}
