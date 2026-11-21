/* Lab-11: Multithreading - four threads add up different parts of the range 1..N */
#include <pthread.h>
#include <stdio.h>

#define NUM_THREADS 4

struct task {
    int start;
    int end;
    long sum;
};

static void *partial_sum(void *arg)
{
    struct task *t = (struct task *)arg;
    int i;

    t->sum = 0;
    for (i = t->start; i <= t->end; i++)
        t->sum += i;

    return NULL;
}

int main(void)
{
    pthread_t tid[NUM_THREADS];
    struct task tasks[NUM_THREADS];
    int n, i, chunk;
    long total = 0;

    printf("Enter N (threads will add the numbers 1 to N): ");
    if (scanf("%d", &n) != 1 || n < NUM_THREADS) {
        printf("N must be at least %d\n", NUM_THREADS);
        return 1;
    }

    chunk = n / NUM_THREADS;
    for (i = 0; i < NUM_THREADS; i++) {
        tasks[i].start = i * chunk + 1;
        tasks[i].end = (i == NUM_THREADS - 1) ? n : (i + 1) * chunk;
        if (pthread_create(&tid[i], NULL, partial_sum, &tasks[i]) != 0) {
            perror("pthread_create");
            return 1;
        }
    }

    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(tid[i], NULL);
        printf("Thread %d: sum of %d to %d = %ld\n", i + 1, tasks[i].start, tasks[i].end,
               tasks[i].sum);
        total += tasks[i].sum;
    }

    printf("Total sum of 1 to %d = %ld\n", n, total);
    return 0;
}
