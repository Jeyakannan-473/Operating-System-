#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define TRAINS 10
#define COACHES 8
#define SEATS 72
#define THREADS 16
#define PER_THREAD 600

static char seat[TRAINS][COACHES][SEATS];
static pthread_mutex_t gl = PTHREAD_MUTEX_INITIALIZER, tl[TRAINS], cl[TRAINS][COACHES];
static int gran; static long booked[THREADS], refused[THREADS], bad;

static double now(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec+t.tv_nsec*1e-9; }

static void *agent(void *a) {
    long id = (long)a; unsigned s = 12345u + (unsigned)id * 977u;
    for (int i = 0; i < PER_THREAD; i++) {
        int t = rand_r(&s) % TRAINS, c = rand_r(&s) % COACHES, k = rand_r(&s) % SEATS;
        pthread_mutex_t *m = gran == 0 ? &gl : gran == 1 ? &tl[t] : &cl[t][c];
        pthread_mutex_lock(m);
        if (seat[t][c][k] == 0) {
            struct timespec ts = {0, 50000}; nanosleep(&ts, NULL);
            seat[t][c][k]++; booked[id]++;
        } else refused[id]++;
        pthread_mutex_unlock(m);
    }
    return NULL;
}

int main(void) {
    for (int i = 0; i < TRAINS; i++) {
        pthread_mutex_init(&tl[i], NULL);
        for (int j = 0; j < COACHES; j++) pthread_mutex_init(&cl[i][j], NULL);
    }
    const char *name[] = {"one global lock", "one lock per train", "one lock per coach"};
    
    for (gran = 0; gran < 3; gran++) {
        for (int t = 0; t < TRAINS; t++) for (int c = 0; c < COACHES; c++) for (int k = 0; k < SEATS; k++) seat[t][c][k] = 0;
        for (int i = 0; i < THREADS; i++) booked[i] = refused[i] = 0;
        
        pthread_t th[THREADS]; double s = now();
        for (long i = 0; i < THREADS; i++) pthread_create(&th[i], NULL, agent, (void *)i);
        for (int i = 0; i < THREADS; i++) pthread_join(th[i], NULL);
        double el = now() - s;
        
        long b = 0, r = 0, sold = 0; bad = 0;
        for (int i = 0; i < THREADS; i++) { b += booked[i]; r += refused[i]; }
        for (int t = 0; t < TRAINS; t++) for (int c = 0; c < COACHES; c++) for (int k = 0; k < SEATS; k++) {
            sold += seat[t][c][k]; if (seat[t][c][k] > 1) bad++;
        }
        
        printf("%-20s %6.3f s %8.0f requests/s booked %ld, refused %ld, seats sold %ld, double-sold %ld\n",
               name[gran], el, THREADS * PER_THREAD / el, b, r, sold, bad);
    }
    return 0;
}
