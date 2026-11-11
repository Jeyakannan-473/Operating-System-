#define _POSIX_C_SOURCE 200809L
#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define CHUNK 65536

static double now(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec+t.tv_nsec*1e-9; }

struct shm { sem_t empty, full; size_t len; char data[CHUNK]; };

static double via_pipe(size_t total) {
    int fd[2]; char *buf = malloc(CHUNK); size_t got = 0; ssize_t n;
    if (pipe(fd)) { perror("pipe"); exit(1); }
    memset(buf, 7, CHUNK);
    double s = now(); pid_t p = fork();
    
    if (p == 0) {
        close(fd[1]); char *rb = malloc(CHUNK); unsigned long sum = 0;
        while ((n = read(fd[0], rb, CHUNK)) > 0) { got += n; sum += rb[0]; }
        exit(got == total ? 0 : 1);
    }
    close(fd[0]);
    for (size_t sent = 0; sent < total;) {
        size_t c = total - sent < CHUNK ? total - sent : CHUNK;
        ssize_t w = write(fd[1], buf, c); if (w <= 0) { perror("write"); exit(1); }
        sent += w;
    }
    close(fd[1]); int st; waitpid(p, &st, 0); free(buf);
    return WIFEXITED(st) && WEXITSTATUS(st) == 0 ? now() - s : -1;
}

static double via_shm(size_t total) {
    struct shm *m = mmap(NULL, sizeof *m, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sem_init(&m->empty, 1, 1); sem_init(&m->full, 1, 0); m->len = 1;
    char *buf = malloc(CHUNK); memset(buf, 7, CHUNK);
    double s = now(); pid_t p = fork();
    
    if (p == 0) {
        size_t got = 0; unsigned long sum = 0;
        for (;;) {
            sem_wait(&m->full); if (m->len == 0) break;
            got += m->len; sum += m->data[0];
            sem_post(&m->empty);
        }
        exit(got == total ? 0 : 1);
    }
    for (size_t sent = 0; sent < total;) {
        size_t c = total - sent < CHUNK ? total - sent : CHUNK;
        sem_wait(&m->empty); memcpy(m->data, buf, c); m->len = c; sem_post(&m->full);
        sent += c;
    }
    sem_wait(&m->empty); m->len = 0; sem_post(&m->full);
    int st; waitpid(p, &st, 0); free(buf);
    return WIFEXITED(st) && WEXITSTATUS(st) == 0 ? now() - s : -1;
}

int main(void) {
    size_t mb[] = {1, 10, 100};
    printf("%8s %14s %14s %s\n", "size", "pipe (s)", "shm+sem (s)", "ratio");
    for (int i = 0; i < 3; i++) {
        size_t t = mb[i] << 20; double a = 1e9, b = 1e9;
        for (int k = 0; k < 5; k++) {
            double x = via_pipe(t), y = via_shm(t);
            if (x < a) a = x; if (y < b) b = y;
        }
        printf("%5zu MB %14.4f %14.4f %7.2fx\n", mb[i], a, b, a/b);
    }
    return 0;
}
