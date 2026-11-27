/* Lab-25: UNIX I/O system calls - fcntl, lseek, stat, opendir and readdir */
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define FILE_NAME "sample.txt"
#define DIR_NAME "demo_dir"

static void die(const char *msg)
{
    perror(msg);
    exit(1);
}

static int compare_names(const void *a, const void *b)
{
    return strcmp((const char *)a, (const char *)b);
}

int main(void)
{
    const char *text = "Operating Systems\n";
    const char *more = "Appended line\n";
    const char *names[] = {"c.txt", "a.txt", "b.txt"};
    char path[100], list[10][256], buf[32];
    struct stat st;
    struct dirent *entry;
    DIR *dp;
    off_t pos;
    ssize_t n;
    int fd, flags, count = 0, i;

    /* open() and write(): create the file and put some data in it */
    fd = open(FILE_NAME, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
        die("open");
    n = write(fd, text, strlen(text));
    printf("open()/write() : %s created, %zd bytes written\n", FILE_NAME, n);

    /* fcntl(): read the file status flags, then turn on O_APPEND */
    flags = fcntl(fd, F_GETFL);
    if (flags < 0)
        die("fcntl");
    printf("\nfcntl(F_GETFL) : access mode = %s, O_APPEND = %s\n",
           (flags & O_ACCMODE) == O_RDWR ? "O_RDWR" : "other", (flags & O_APPEND) ? "on" : "off");
    if (fcntl(fd, F_SETFL, flags | O_APPEND) < 0)
        die("fcntl");
    flags = fcntl(fd, F_GETFL);
    printf("fcntl(F_SETFL) : O_APPEND turned on, O_APPEND = %s\n", (flags & O_APPEND) ? "on" : "off");

    /* lseek(): move the file offset; O_APPEND still sends this write to the end */
    pos = lseek(fd, 0, SEEK_SET);
    printf("\nlseek(0, SEEK_SET)  : offset = %ld\n", (long)pos);
    n = write(fd, more, strlen(more));
    printf("write() at offset 0 : %zd bytes written (O_APPEND adds them at the end)\n", n);
    pos = lseek(fd, 0, SEEK_END);
    printf("lseek(0, SEEK_END)  : offset = %ld (file size)\n", (long)pos);
    pos = lseek(fd, 10, SEEK_SET);
    n = read(fd, buf, 7);
    printf("lseek(10, SEEK_SET) : offset = %ld, read() gave \"%.*s\"\n", (long)pos, (int)n, buf);
    close(fd);

    /* stat(): file information */
    if (stat(FILE_NAME, &st) < 0)
        die("stat");
    printf("\nstat() : size = %ld bytes, permissions = 0%o, links = %ld, type = %s\n",
           (long)st.st_size, (unsigned)(st.st_mode & 0777), (long)st.st_nlink,
           S_ISREG(st.st_mode) ? "regular file" : "other");

    /* mkdir(), opendir() and readdir(): create a directory and list its entries */
    if (mkdir(DIR_NAME, 0755) < 0)
        die("mkdir");
    for (i = 0; i < 3; i++) {
        snprintf(path, sizeof(path), "%s/%s", DIR_NAME, names[i]);
        fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0)
            die("open");
        close(fd);
    }

    dp = opendir(DIR_NAME);
    if (dp == NULL)
        die("opendir");
    while ((entry = readdir(dp)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        if (count < 10)
            strcpy(list[count++], entry->d_name);
    }
    closedir(dp);

    qsort(list, count, sizeof(list[0]), compare_names);
    printf("\nopendir()/readdir() : files in %s (sorted by name):\n", DIR_NAME);
    for (i = 0; i < count; i++)
        printf("  %s\n", list[i]);

    /* Remove the demo files and directory */
    for (i = 0; i < 3; i++) {
        snprintf(path, sizeof(path), "%s/%s", DIR_NAME, names[i]);
        unlink(path);
    }
    rmdir(DIR_NAME);
    unlink(FILE_NAME);
    printf("\nDemo files removed.\n");

    return 0;
}
