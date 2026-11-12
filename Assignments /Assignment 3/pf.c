/* pf.c - run a command and report page faults, like /usr/bin/time -v */
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>
int main(int argc, char **argv) {
    if (fork() == 0) { execvp(argv[1], argv + 1); perror("exec"); _exit(127); }
    int st; struct rusage r; wait4(-1, &st, 0, &r);
    printf("Minor (reclaiming a frame) page faults: %ld\nMajor (I/O) page faults: %ld\nMaximum resident set size (kbytes): %ld\nPage size (bytes): %ld\n", r.ru_minflt, r.ru_majflt, r.ru_maxrss, sysconf(_SC_PAGESIZE));
    return 0;
}
