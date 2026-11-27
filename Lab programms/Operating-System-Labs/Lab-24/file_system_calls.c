/* Lab-24: UNIX system calls for file management - creat, open, write, read, close, unlink */
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
    char name[100], text[200], line[210], buf[256];
    int fd;
    ssize_t n;

    printf("Enter file name: ");
    scanf("%99s", name);
    printf("Enter text to write: ");
    scanf(" %199[^\n]", text);

    /* creat(): create a new (empty) file */
    fd = creat(name, 0644);
    if (fd < 0) {
        perror("creat");
        return 1;
    }
    printf("\ncreat()  : file %s created, file descriptor = %d\n", name, fd);
    close(fd);

    /* open() + write(): append the text to the file */
    fd = open(name, O_WRONLY | O_APPEND);
    if (fd < 0) {
        perror("open");
        return 1;
    }
    printf("open()   : %s opened for writing, file descriptor = %d\n", name, fd);

    snprintf(line, sizeof(line), "%s\n", text);
    n = write(fd, line, strlen(line));
    if (n < 0) {
        perror("write");
        close(fd);
        return 1;
    }
    printf("write()  : %zd bytes written\n", n);
    close(fd);
    printf("close()  : file closed\n");

    /* open() + read(): read the data back */
    fd = open(name, O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }
    printf("open()   : %s opened for reading, file descriptor = %d\n", name, fd);

    n = read(fd, buf, sizeof(buf) - 1);
    if (n < 0) {
        perror("read");
        close(fd);
        return 1;
    }
    buf[n] = '\0';
    printf("read()   : %zd bytes read\n", n);
    printf("Data read from the file: %s", buf);
    close(fd);

    /* unlink(): delete the file */
    if (unlink(name) < 0) {
        perror("unlink");
        return 1;
    }
    printf("unlink() : file %s deleted\n", name);

    return 0;
}
