/* Lab-09: Inter-process communication using shared memory (writer and reader processes) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define SHM_SIZE 1024

int main(void)
{
    char message[100];
    char *shm_ptr;
    int shmid;
    pid_t pid;

    printf("Enter a message to write to shared memory: ");
    if (fgets(message, sizeof(message), stdin) == NULL)
        return 1;
    message[strcspn(message, "\n")] = '\0';

    /* Create a shared memory segment */
    shmid = shmget(IPC_PRIVATE, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        return 1;
    }

    pid = fork();
    if (pid < 0) {
        perror("fork");
        shmctl(shmid, IPC_RMID, NULL);
        return 1;
    }

    if (pid == 0) {
        /* Child: writer process */
        shm_ptr = (char *)shmat(shmid, NULL, 0);
        if (shm_ptr == (char *)-1) {
            perror("shmat");
            exit(1);
        }
        strcpy(shm_ptr, message);
        printf("Writer process (PID %d) wrote to shared memory: %s\n", getpid(), shm_ptr);
        shmdt(shm_ptr);
        exit(0);
    }

    /* Parent: reader process - waits until the writer has finished */
    wait(NULL);

    shm_ptr = (char *)shmat(shmid, NULL, 0);
    if (shm_ptr == (char *)-1) {
        perror("shmat");
        shmctl(shmid, IPC_RMID, NULL);
        return 1;
    }
    printf("Reader process (PID %d) read from shared memory: %s\n", getpid(), shm_ptr);

    shmdt(shm_ptr);
    shmctl(shmid, IPC_RMID, NULL);     /* remove the shared memory segment */

    return 0;
}
