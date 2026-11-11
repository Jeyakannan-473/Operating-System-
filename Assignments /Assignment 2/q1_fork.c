#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    int n = argc > 1 ? atoi(argv[1]) : 3;
    for (int i=0; i<n; i++) {
        pid_t pid = fork();
        if (pid < 0) { perror("fork"); return 1; }
        if (pid == 0) {
            printf("session %d: pid=%d ppid=%d\n", i + 1, getpid(), getppid());
            fflush(stdout);
            sleep(4);
            _exit(i + 1);
        }
    }
    printf("server pid=%d created %d sessions\n", getpid(), n);
    fflush(stdout);
    for (int i=0; i<n; i++) {
        int st; pid_t w = wait(&st);
        printf("session pid=%d terminated, exit status %d\n", w, WEXITSTATUS(st));
    }
    return 0;
}
