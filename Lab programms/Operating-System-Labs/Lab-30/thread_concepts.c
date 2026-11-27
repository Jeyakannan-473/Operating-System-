/* Lab-30: Thread concepts - pthread_create, pthread_join, pthread_equal and pthread_exit */
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

static pthread_t main_thread;

static void *worker(void *arg)
{
    int id = (int)(intptr_t)arg;

    usleep(100000 * id);                /* stagger the threads so the output order is stable */
    printf("Thread %d: pthread_equal(self, main thread) = %s\n", id,
           pthread_equal(pthread_self(), main_thread) ? "equal" : "not equal");
    printf("Thread %d: calling pthread_exit with value %d\n", id, id * 10);

    pthread_exit((void *)(intptr_t)(id * 10));      /* value is collected by pthread_join */
}

int main(void)
{
    pthread_t tid[2];
    void *value;
    int i;

    main_thread = pthread_self();

    /* (i) create */
    for (i = 0; i < 2; i++) {
        if (pthread_create(&tid[i], NULL, worker, (void *)(intptr_t)(i + 1)) != 0) {
            perror("pthread_create");
            return 1;
        }
        printf("Main: thread %d created\n", i + 1);
    }

    /* (iii) equal */
    printf("Main: pthread_equal(thread 1, thread 1) = %s\n",
           pthread_equal(tid[0], tid[0]) ? "equal" : "not equal");
    printf("Main: pthread_equal(thread 1, thread 2) = %s\n",
           pthread_equal(tid[0], tid[1]) ? "equal" : "not equal");

    /* (ii) join - wait for each thread and collect the value given to pthread_exit */
    for (i = 0; i < 2; i++) {
        pthread_join(tid[i], &value);
        printf("Main: thread %d joined, exit value = %d\n", i + 1, (int)(intptr_t)value);
    }

    printf("Main: all threads have finished\n");
    return 0;
}
