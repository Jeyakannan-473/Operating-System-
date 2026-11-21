/* Lab-14: Organising files using a single level directory (all files in one directory) */
#include <stdio.h>
#include <string.h>

#define MAX_FILES 20
#define NAME_LEN 30

static int find_file(char files[][NAME_LEN], int count, const char *name)
{
    int i;

    for (i = 0; i < count; i++)
        if (strcmp(files[i], name) == 0)
            return i;
    return -1;
}

int main(void)
{
    char files[MAX_FILES][NAME_LEN];
    char name[NAME_LEN];
    int count = 0, choice, i, pos;

    do {
        printf("\n1.Create 2.Delete 3.Search 4.List 5.Exit - Enter your choice: ");
        if (scanf("%d", &choice) != 1)
            return 1;

        switch (choice) {
        case 1:
            printf("Enter file name: ");
            scanf("%29s", name);
            if (count == MAX_FILES)
                printf("Directory is full\n");
            else if (find_file(files, count, name) != -1)
                printf("File %s already exists\n", name);
            else {
                strcpy(files[count++], name);
                printf("File %s created\n", name);
            }
            break;
        case 2:
            printf("Enter file name: ");
            scanf("%29s", name);
            pos = find_file(files, count, name);
            if (pos == -1)
                printf("File %s not found\n", name);
            else {
                for (i = pos; i < count - 1; i++)
                    strcpy(files[i], files[i + 1]);
                count--;
                printf("File %s deleted\n", name);
            }
            break;
        case 3:
            printf("Enter file name: ");
            scanf("%29s", name);
            if (find_file(files, count, name) == -1)
                printf("File %s not found\n", name);
            else
                printf("File %s found\n", name);
            break;
        case 4:
            if (count == 0)
                printf("Directory is empty\n");
            else {
                printf("Files in the directory:\n");
                for (i = 0; i < count; i++)
                    printf("%d. %s\n", i + 1, files[i]);
            }
            break;
        case 5:
            break;
        default:
            printf("Invalid choice\n");
        }
    } while (choice != 5);

    return 0;
}
