/* Lab-02: Copy one file to another using open, read, write and close system calls */
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#define BUF_SIZE 1024

int main(void)
{
    char src[100], dest[100], buf[BUF_SIZE];
    int fd_src, fd_dest;
    ssize_t n;

    printf("Enter the source file name: ");
    scanf("%99s", src);
    printf("Enter the destination file name: ");
    scanf("%99s", dest);

    fd_src = open(src, O_RDONLY);
    if (fd_src < 0) {
        perror("open source");
        return 1;
    }

    fd_dest = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_dest < 0) {
        perror("open destination");
        close(fd_src);
        return 1;
    }

    while ((n = read(fd_src, buf, BUF_SIZE)) > 0) {
        if (write(fd_dest, buf, n) != n) {
            perror("write");
            close(fd_src);
            close(fd_dest);
            return 1;
        }
    }
    if (n < 0)
        perror("read");

    close(fd_src);
    close(fd_dest);

    printf("Contents copied from %s to %s\n", src, dest);
    return 0;
}
