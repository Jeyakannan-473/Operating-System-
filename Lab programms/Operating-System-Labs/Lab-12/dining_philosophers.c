/* Lab-12: Dining-Philosophers problem using threads and mutex locks (deadlock free) */
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5
#define MEALS 2

static pthread_mutex_t chopstick[NUM_PHILOSOPHERS];

static void *philosopher(void *arg)
{
    int id = *(int *)arg;
    int left = id;
    int right = (id + 1) % NUM_PHILOSOPHERS;
    int first = left < right ? left : right;
    int second = left < right ? right : left;
    int meal;

    for (meal = 1; meal <= MEALS; meal++) {
        printf("Philosopher %d is thinking\n", id + 1);
        usleep(100000 * (id + 1));

        /* Always take the lower numbered chopstick first: this prevents circular wait */
        pthread_mutex_lock(&chopstick[first]);
        pthread_mutex_lock(&chopstick[second]);

        printf("Philosopher %d is eating (meal %d)\n", id + 1, meal);
        usleep(150000);

        pthread_mutex_unlock(&chopstick[second]);
        pthread_mutex_unlock(&chopstick[first]);
        printf("Philosopher %d put down the chopsticks\n", id + 1);
    }

    return NULL;
}

int main(void)
{
    pthread_t thread[NUM_PHILOSOPHERS];
    int id[NUM_PHILOSOPHERS];
    int i;

    for (i = 0; i < NUM_PHILOSOPHERS; i++)
        pthread_mutex_init(&chopstick[i], NULL);

    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        id[i] = i;
        pthread_create(&thread[i], NULL, philosopher, &id[i]);
    }

    for (i = 0; i < NUM_PHILOSOPHERS; i++)
        pthread_join(thread[i], NULL);

    for (i = 0; i < NUM_PHILOSOPHERS; i++)
        pthread_mutex_destroy(&chopstick[i]);

    printf("All philosophers have finished eating.\n");
    return 0;
}
