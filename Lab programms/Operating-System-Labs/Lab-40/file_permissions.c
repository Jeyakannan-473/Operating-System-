/* Lab-40: Linux file access permissions and the three types of users (owner, group, others) */
#include <fcntl.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

static void show_permissions(const char *name)
{
    const char *who[] = {"Owner", "Group", "Others"};
    const char *rwx = "rwxrwxrwx";
    struct stat st;
    struct passwd *pw;
    struct group *gr;
    char perm[11];
    int i, bits;

    if (stat(name, &st) < 0) {
        perror("stat");
        return;
    }

    perm[0] = S_ISDIR(st.st_mode) ? 'd' : '-';
    for (i = 0; i < 9; i++)
        perm[i + 1] = (st.st_mode & (1 << (8 - i))) ? rwx[i] : '-';
    perm[10] = '\0';

    pw = getpwuid(st.st_uid);
    gr = getgrgid(st.st_gid);
    printf("File        : %s\n", name);
    printf("Permissions : %s (octal %03o)\n", perm, (unsigned)(st.st_mode & 0777));
    printf("Owner       : %s\n", pw ? pw->pw_name : "unknown");
    printf("Group       : %s\n", gr ? gr->gr_name : "unknown");

    printf("\n%-10s%-8s%-8s%s\n", "User type", "Read", "Write", "Execute");
    for (i = 0; i < 3; i++) {
        bits = (st.st_mode >> (6 - 3 * i)) & 7;
        printf("%-10s%-8s%-8s%s\n", who[i], (bits & 4) ? "yes" : "no",
               (bits & 2) ? "yes" : "no", (bits & 1) ? "yes" : "no");
    }
}

int main(void)
{
    char name[100];
    unsigned int mode;
    int fd, created = 0;

    printf("Enter file name: ");
    scanf("%99s", name);

    if (access(name, F_OK) != 0) {          /* create the file if it does not exist */
        fd = open(name, O_WRONLY | O_CREAT, 0644);
        if (fd < 0) {
            perror("open");
            return 1;
        }
        close(fd);
        created = 1;
        printf("File %s created\n", name);
    }

    printf("\nCurrent permissions:\n");
    show_permissions(name);

    printf("\nEnter new permissions in octal (for example 640): ");
    if (scanf("%o", &mode) != 1 || mode > 0777) {
        printf("Invalid permissions\n");
        return 1;
    }
    if (chmod(name, (mode_t)mode) != 0) {
        perror("chmod");
        return 1;
    }

    printf("\nUpdated permissions:\n");
    show_permissions(name);

    if (created) {                          /* only remove the file this program created */
        unlink(name);
        printf("\nFile %s deleted\n", name);
    }

    return 0;
}
