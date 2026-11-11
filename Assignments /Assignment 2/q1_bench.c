#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/wait.h>

static double now(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec+t.tv_nsec*1e-9; }
static void *th(void *a) { (void)a; return NULL; }
static long rss_kb(void) { FILE* f=fopen("/proc/self/statm", "r"); long a,b; if(fscanf(f, "%ld %ld", &a, &b)!=2) b=0; fclose(f); return b*4; }

int main(void) {
    int N = 2000;
    double t = now();
    for (int i = 0; i < N; i++) { pid_t p = fork(); if (p == 0) exit(0); waitpid(p, NULL, 0); }
    double tp = now() - t;
    
    t = now();
    for (int i = 0; i < N; i++) { pthread_t x; pthread_create(&x, NULL, th, NULL); pthread_join(x, NULL); }
    double tt = now() - t;
    
    printf("create+join %d processes: %.3f s (%.1f us each)\n", N, tp, tp/N*1e6);
    printf("create+join %d threads : %.3f s (%.1f us each)\n", N, tt, tt/N*1e6);
    printf("resident memory of this process: %ld KB\n", rss_kb());
    
    int a[2], b[2]; if (pipe(a) || pipe(b)) return 1;
    int R = 100000; char c = 'x';
    pid_t p = fork();
    if (p == 0) {
        for (int i = 0; i < R; i++) {
            if (read(a[0], &c, 1) != 1) exit(1);
            if (write(b[1], &c, 1) != 1) exit(1);
        }
        exit(0);
    }
    
    t = now();
    for (int i = 0; i < R; i++) {
        if (write(a[1], &c, 1) != 1) return 1;
        if (read(b[0], &c, 1) != 1) return 1;
    }
    double pp = now() - t; waitpid(p, NULL, 0);
    printf("process ping-pong: %.2f us per round trip (2 switches + 4 syscalls)\n", pp/R*1e6);
    return 0;
}
