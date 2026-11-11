#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define M 3
static double now(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec+t.tv_nsec*1e-9; }

static long ops;
static int safe(int n, int avail[M], int (*maxm)[M], int (*alloc)[M]) {
    int work[M], count = 0; char *finish = calloc(n, 1); memcpy(work, avail, sizeof work);
    while (count < n) {
        int found = 0;
        for (int p = 0; p < n; p++) {
            if (finish[p]) continue;
            int ok = 1;
            for (int r = 0; r < M; r++) { ops++; if (maxm[p][r] - alloc[p][r] > work[r]) { ok = 0; break; } }
            if (ok) { for (int r = 0; r < M; r++) work[r] += alloc[p][r]; finish[p] = 1; count++; found = 1; }
        }
        if (!found) { free(finish); return 0; }
    }
    free(finish); return 1;
}

int main(void) {
    int sizes[] = {10, 100, 1000, 2000, 4000};
    printf("%6s %12s %14s %12s\n", "n", "time (us)", "comparisons", "time/n^2 (ns)");
    for (int s = 0; s < 5; s++) {
        int n = sizes[s];
        int (*maxm)[M] = malloc(sizeof(int[M]) * n), (*alloc)[M] = malloc(sizeof(int[M]) * n);
        int avail[M] = {1, 1, 1};
        for (int j = 0; j < n; j++) for (int r = 0; r < M; r++) {
            alloc[j][r] = 1; maxm[j][r] = 1 + (r == 0 ? (n - 1 - j) : 0);
        }
        
        int reps = n < 100 ? 2000 : 5; double best = 1e9;
        for (int k = 0; k < 3; k++) {
            double t = now();
            for (int i = 0; i < reps; i++) {
                ops = 0; if (!safe(n, avail, maxm, alloc)) { puts("unsafe?!"); return 1; }
            }
            double e = (now() - t) / reps; if (e < best) best = e;
        }
        
        printf("%d %12.2f %14ld %12.2f\n", n, best * 1e6, ops, best * 1e9 / ((double)n * n));
        free(maxm); free(alloc);
    }
    return 0;
}
