/* Lab-15: Organising files using a two level directory (one directory per user) */
#include <stdio.h>
#include <string.h>

#define MAX_USERS 10
#define MAX_FILES 10
#define NAME_LEN 30

struct user_dir {
    char name[NAME_LEN];
    char files[MAX_FILES][NAME_LEN];
    int count;
};

static struct user_dir dir[MAX_USERS];
static int users = 0;

static int find_user(const char *name)
{
    int i;

    for (i = 0; i < users; i++)
        if (strcmp(dir[i].name, name) == 0)
            return i;
    return -1;
}

static int find_file(const struct user_dir *d, const char *name)
{
    int i;

    for (i = 0; i < d->count; i++)
        if (strcmp(d->files[i], name) == 0)
            return i;
    return -1;
}

int main(void)
{
    char user[NAME_LEN], name[NAME_LEN];
    int choice, u, f, i, j;

    do {
        printf("\n1.Create directory 2.Create file 3.Delete file 4.Search file 5.Display 6.Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1)
            return 1;

        if (choice >= 2 && choice <= 4) {
            printf("Enter directory name: ");
            scanf("%29s", user);
            u = find_user(user);
            if (u == -1) {
                printf("Directory %s does not exist\n", user);
                continue;
            }
            printf("Enter file name: ");
            scanf("%29s", name);
            f = find_file(&dir[u], name);
        }

        switch (choice) {
        case 1:
            printf("Enter directory name: ");
            scanf("%29s", user);
            if (users == MAX_USERS)
                printf("Master directory is full\n");
            else if (find_user(user) != -1)
                printf("Directory %s already exists\n", user);
            else {
                strcpy(dir[users].name, user);
                dir[users++].count = 0;
                printf("Directory %s created\n", user);
            }
            break;
        case 2:
            if (f != -1)
                printf("File %s already exists in %s\n", name, user);
            else if (dir[u].count == MAX_FILES)
                printf("Directory %s is full\n", user);
            else {
                strcpy(dir[u].files[dir[u].count++], name);
                printf("File %s created in %s\n", name, user);
            }
            break;
        case 3:
            if (f == -1)
                printf("File %s not found in %s\n", name, user);
            else {
                for (j = f; j < dir[u].count - 1; j++)
                    strcpy(dir[u].files[j], dir[u].files[j + 1]);
                dir[u].count--;
                printf("File %s deleted from %s\n", name, user);
            }
            break;
        case 4:
            if (f == -1)
                printf("File %s not found in %s\n", name, user);
            else
                printf("File %s found in %s\n", name, user);
            break;
        case 5:
            if (users == 0)
                printf("No directories created\n");
            else {
                printf("%-12s%s\n", "Directory", "Files");
                for (i = 0; i < users; i++) {
                    printf("%-12s", dir[i].name);
                    if (dir[i].count == 0)
                        printf("(empty)");
                    for (j = 0; j < dir[i].count; j++)
                        printf("%s%s", dir[i].files[j], j < dir[i].count - 1 ? ", " : "");
                    printf("\n");
                }
            }
            break;
        case 6:
            break;
        default:
            printf("Invalid choice\n");
        }
    } while (choice != 6);

    return 0;
}
