#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/wait.h>

struct box { sem_t empty, full; int value; int last; };

int main(void) {
    struct box *b = mmap(NULL, sizeof *b, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sem_init(&b->empty, 1, 1); sem_init(&b->full, 1, 0);
    
    pid_t pid = fork();
    if (pid == 0) {
        int count = 0, sum = 0, max = 0;
        for (;;) {
            sem_wait(&b->full);
            int v = b->value, last = b->last;
            sem_post(&b->empty);
            if (last) break;
            count++; sum += v; if (v > max) max = v;
        }
        printf("analyser: %d readings, average %.1f, max %d\n", count, count ? (double)sum / count : 0.0, max);
        fflush(stdout); _exit(0);
    }
    
    int readings[] = {31, 33, 36, 34, 41, 38};
    for (int i = 0; i < 6; i++) {
        sem_wait(&b->empty); b->value = readings[i]; b->last = 0; sem_post(&b->full);
    }
    sem_wait(&b->empty); b->last = 1; sem_post(&b->full);
    waitpid(pid, NULL, 0); printf("reader: done\n");
    return 0;
}
