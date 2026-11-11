#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>
#include <time.h>
#include <sys/resource.h>

static double now(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec+t.tv_nsec*1e-9; }
static double cpu(void) { struct rusage r; getrusage(RUSAGE_SELF, &r); return r.ru_utime.tv_sec+r.ru_stime.tv_sec+(r.ru_utime.tv_usec+r.ru_stime.tv_usec)*1e-6; }

static long counter; static atomic_long acounter; static pthread_mutex_t mx = PTHREAD_MUTEX_INITIALIZER;
static atomic_flag spin = ATOMIC_FLAG_INIT;
static long N; static int mode; static long cs_iters;

static void cs_work(void) { volatile long x = 0; for (long i = 0; i < cs_iters; i++) x += i; }

static void *w(void *a) {
    (void)a;
    for (long i = 0; i < N; i++) {
        if (mode == 0) { pthread_mutex_lock(&mx); counter++; cs_work(); pthread_mutex_unlock(&mx); }
        else if (mode == 1) { while (atomic_flag_test_and_set_explicit(&spin, memory_order_acquire)); counter++; cs_work(); atomic_flag_clear_explicit(&spin, memory_order_release); }
        else { atomic_fetch_add(&acounter, 1); }
    }
    return NULL;
}

static void run(int T, long n, long cs, int m, const char *name) {
    N = n; mode = m; cs_iters = cs; counter = 0; atomic_store(&acounter, 0);
    pthread_t t[16]; double c0 = cpu(), s = now();
    for (int i = 0; i < T; i++) pthread_create(&t[i], NULL, w, NULL);
    for (int i = 0; i < T; i++) pthread_join(t[i], NULL);
    double el = now() - s, cp = cpu() - c0; long tot = m == 2 ? atomic_load(&acounter) : counter;
    printf("%-14s %2d thr %9.1f ns/update wall %6.3f s cpu %6.3f s count %s\n", name, T, el / (T * n) * 1e9, el, cp, tot == T * n ? "OK" : "WRONG");
}

int main(void) {
    int th[] = {1, 4, 16};
    puts("-- short critical section (one increment), 2,000,000 updates per thread");
    for (int i = 0; i < 3; i++) { run(th[i], 2000000, 0, 0, "mutex"); run(th[i], 2000000, 0, 1, "spin lock"); run(th[i], 2000000, 0, 2, "atomic add"); }
    puts("-- long critical section (about 20 us of work inside), 200 updates per thread");
    for (int i = 0; i < 3; i++) { run(th[i], 200, 20000, 0, "mutex"); run(th[i], 200, 20000, 1, "spin lock"); }
    return 0;
}
