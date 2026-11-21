/* Lab-01: Create a new process with fork() and display process IDs */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    pid_t pid;

    printf("Before fork : PID = %d, PPID = %d\n", getpid(), getppid());

    pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    } else if (pid == 0) {
        printf("Child process : PID = %d, PPID = %d\n", getpid(), getppid());
    } else {
        wait(NULL);
        printf("Parent process: PID = %d, PPID = %d, Child PID = %d\n",
               getpid(), getppid(), pid);
    }

    return 0;
}
