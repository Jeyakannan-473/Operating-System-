#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#define CAMERAS 64

static int io_us = 0;
static long work_iters = 3000000;
static int next_cam = 0, plates = 0;
static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

static double now(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec+t.tv_nsec*1e-9; }

static void process_frame(int cam) {
    if (io_us) { struct timespec ts = {0, io_us * 1000L}; nanosleep(&ts, NULL); }
    volatile unsigned long x = (unsigned long)cam;
    for (long i = 0; i < work_iters; i++) x = x * 1103515245UL + 12345UL;
    pthread_mutex_lock(&m); plates++; pthread_mutex_unlock(&m);
}

static void *worker(void *arg) {
    (void)arg;
    for (;;) {
        pthread_mutex_lock(&m);
        int c = next_cam < CAMERAS ? next_cam++ : -1;
        pthread_mutex_unlock(&m);
        if (c < 0) break;
        process_frame(c);
    }
    return NULL;
}

static void *one(void *arg) { process_frame((int)(long)arg); return NULL; }

static double run_pool(int n) {
    next_cam = 0; plates = 0; pthread_t t[CAMERAS]; double s = now();
    for (int i=0; i<n; i++) pthread_create(&t[i], NULL, worker, NULL);
    for (int i=0; i<n; i++) pthread_join(t[i], NULL);
    return now() - s;
}

int main(int argc, char **argv) {
    if (argc > 1) io_us = atoi(argv[1]);
    printf("frame = %d us I/O wait + CPU work; %d cameras; CPUs online = %ld\n", io_us, CAMERAS, sysconf(_SC_NPROCESSORS_ONLN));
    
    plates = 0; double s = now();
    for (int c=0; c < CAMERAS; c++) process_frame(c);
    double seq = now() - s;
    printf("%-26s %7.3f s speed-up 1.00 (plates=%d)\n", "sequential (1 thread)", seq, plates);
    
    plates = 0; pthread_t t[CAMERAS]; s = now();
    for (long c=0; c < CAMERAS; c++) pthread_create(&t[c], NULL, one, (void*)c);
    for (int c=0; c < CAMERAS; c++) pthread_join(t[c], NULL);
    double all = now() - s;
    printf("%-26s %7.3f s speed-up %5.2f (plates=%d)\n", "64 threads (1 per camera)", all, seq/all, plates);
    
    int sizes[] = {4, 8, 16};
    for (int i = 0; i < 3; i++) {
        double d = run_pool(sizes[i]); char lab[40]; snprintf(lab, sizeof lab, "pool of %d threads", sizes[i]);
        printf("%-26s %7.3f s speed-up %5.2f (plates=%d)\n", lab, d, seq/d, plates);
    }
    return 0;
}
