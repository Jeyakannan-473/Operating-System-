/* Lab-27: Simulating the UNIX ls command (names, and a long listing like ls -l) */
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX_ENTRIES 200
#define NAME_LEN 256

static char names[MAX_ENTRIES][NAME_LEN];

static int compare_names(const void *a, const void *b)
{
    return strcmp((const char *)a, (const char *)b);
}

/* Build a permission string such as drwxr-xr-x */
static void permission_string(mode_t mode, char *out)
{
    const char *rwx = "rwxrwxrwx";
    int i;

    out[0] = S_ISDIR(mode) ? 'd' : (S_ISLNK(mode) ? 'l' : '-');
    for (i = 0; i < 9; i++)
        out[i + 1] = (mode & (1 << (8 - i))) ? rwx[i] : '-';
    out[10] = '\0';
}

int main(void)
{
    char dirname[200], path[500], perm[11], answer;
    struct dirent *entry;
    struct stat st;
    DIR *dp;
    int count = 0, i;

    printf("Enter directory name: ");
    scanf("%199s", dirname);
    printf("Long listing like ls -l? (y/n): ");
    scanf(" %c", &answer);

    dp = opendir(dirname);
    if (dp == NULL) {
        perror(dirname);
        return 1;
    }
    while ((entry = readdir(dp)) != NULL) {
        if (entry->d_name[0] == '.')            /* ls hides names starting with a dot */
            continue;
        if (count < MAX_ENTRIES) {
            strncpy(names[count], entry->d_name, NAME_LEN - 1);
            names[count][NAME_LEN - 1] = '\0';
            count++;
        }
    }
    closedir(dp);

    qsort(names, count, NAME_LEN, compare_names);

    printf("\n");
    for (i = 0; i < count; i++) {
        if (answer == 'y' || answer == 'Y') {
            snprintf(path, sizeof(path), "%s/%s", dirname, names[i]);
            if (lstat(path, &st) < 0) {
                perror(path);
                continue;
            }
            permission_string(st.st_mode, perm);
            printf("%s %2ld %6ld %s\n", perm, (long)st.st_nlink, (long)st.st_size, names[i]);
        } else {
            printf("%s%s", names[i], i < count - 1 ? "  " : "\n");
        }
    }
    if (count == 0)
        printf("(no files)\n");

    return 0;
}
