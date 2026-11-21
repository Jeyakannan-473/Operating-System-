/* Lab-10: Inter-process communication using a message queue (sender and receiver processes) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

struct message {
    long msg_type;
    char msg_text[100];
};

int main(void)
{
    struct message msg;
    char text[100];
    int msgid;
    pid_t pid;

    printf("Enter a message to send: ");
    if (fgets(text, sizeof(text), stdin) == NULL)
        return 1;
    text[strcspn(text, "\n")] = '\0';

    /* Create a message queue */
    msgid = msgget(IPC_PRIVATE, IPC_CREAT | 0666);
    if (msgid == -1) {
        perror("msgget");
        return 1;
    }

    pid = fork();
    if (pid < 0) {
        perror("fork");
        msgctl(msgid, IPC_RMID, NULL);
        return 1;
    }

    if (pid == 0) {
        /* Child: sender process */
        memset(&msg, 0, sizeof(msg));
        msg.msg_type = 1;
        strcpy(msg.msg_text, text);
        printf("Sender   (PID %d): sending message: %s\n", getpid(), msg.msg_text);
        if (msgsnd(msgid, &msg, sizeof(msg.msg_text), 0) == -1) {
            perror("msgsnd");
            exit(1);
        }
        exit(0);
    }

    /* Parent: receiver process - blocks until a message of type 1 arrives */
    if (msgrcv(msgid, &msg, sizeof(msg.msg_text), 1, 0) == -1) {
        perror("msgrcv");
        msgctl(msgid, IPC_RMID, NULL);
        return 1;
    }
    printf("Receiver (PID %d): received message: %s\n", getpid(), msg.msg_text);

    wait(NULL);
    msgctl(msgid, IPC_RMID, NULL);     /* remove the message queue */

    return 0;
}
