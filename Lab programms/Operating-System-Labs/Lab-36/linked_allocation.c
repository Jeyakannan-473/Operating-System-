/* Lab-36: Linked file allocation - a file is a linked list of disk blocks scattered on the disk;
   the directory stores the first and last block and every block points to the next one */
#include <stdio.h>
#include <string.h>

#define DISK_BLOCKS 30
#define MAX_FILES 10
#define MAX_FILE_BLOCKS 10
#define NAME_LEN 20

struct disk_block {
    int used;
    int next;               /* disk block number of the next block of the file, -1 = last block */
    char data[50];
};

struct dir_entry {
    char name[NAME_LEN];
    int first;
    int last;
    int length;
};

static struct disk_block disk[DISK_BLOCKS];
static struct dir_entry dir[MAX_FILES];
static int files = 0;

static int find_file(const char *name)
{
    int i;

    for (i = 0; i < files; i++)
        if (strcmp(dir[i].name, name) == 0)
            return i;
    return -1;
}

static void create_file(void)
{
    char name[NAME_LEN];
    int n, i, b, prev = -1;

    printf("Enter file name: ");
    scanf("%19s", name);
    if (files == MAX_FILES) {
        printf("Directory is full\n");
        return;
    }
    if (find_file(name) != -1) {
        printf("File %s already exists\n", name);
        return;
    }
    printf("Enter number of blocks (1-%d): ", MAX_FILE_BLOCKS);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_FILE_BLOCKS) {
        printf("Invalid number of blocks\n");
        return;
    }

    strcpy(dir[files].name, name);
    dir[files].length = n;
    for (i = 1; i <= n; i++) {
        for (;;) {                                  /* ask until a free disk block is given */
            printf("Block %d - disk block number (0-%d): ", i, DISK_BLOCKS - 1);
            if (scanf("%d", &b) != 1)
                return;
            if (b < 0 || b >= DISK_BLOCKS)
                printf("Disk block %d does not exist\n", b);
            else if (disk[b].used)
                printf("Disk block %d is already allocated\n", b);
            else
                break;
        }
        printf("Data: ");
        scanf(" %49[^\n]", disk[b].data);
        disk[b].used = 1;
        disk[b].next = -1;                          /* the newest block is the last one */
        if (prev == -1)
            dir[files].first = b;
        else
            disk[prev].next = b;                    /* link the previous block to this one */
        dir[files].last = b;
        prev = b;
    }
    files++;
    printf("File %s created\n", name);
}

static void read_block(void)
{
    char name[NAME_LEN];
    int f, n, i, b;

    printf("Enter file name: ");
    scanf("%19s", name);
    f = find_file(name);
    if (f == -1) {
        printf("File %s not found\n", name);
        return;
    }
    printf("Enter block number to read (1-%d): ", dir[f].length);
    if (scanf("%d", &n) != 1 || n < 1 || n > dir[f].length) {
        printf("Invalid block number\n");
        return;
    }

    /* Blocks can only be reached by following the pointers from the first block */
    printf("Following the pointers from the first block: ");
    b = dir[f].first;
    for (i = 1; i <= n; i++) {
        printf("%d%s", b, i < n ? " -> " : "\n");
        if (i < n)
            b = disk[b].next;
    }
    printf("Block %d of %s is disk block %d, data: %s\n", n, name, b, disk[b].data);
}

static void display_directory(void)
{
    int i, b;

    if (files == 0) {
        printf("Directory is empty\n");
        return;
    }
    printf("%-10s%-8s%-8s%-8s%s\n", "File", "First", "Last", "Length", "Chain of disk blocks");
    for (i = 0; i < files; i++) {
        printf("%-10s%-8d%-8d%-8d", dir[i].name, dir[i].first, dir[i].last, dir[i].length);
        for (b = dir[i].first; b != -1; b = disk[b].next)
            printf("%d -> ", b);
        printf("end\n");
    }
}

int main(void)
{
    int choice;

    do {
        printf("\n1.Create file 2.Read block 3.Display directory 4.Exit - Enter your choice: ");
        if (scanf("%d", &choice) != 1)
            return 1;

        switch (choice) {
        case 1:
            create_file();
            break;
        case 2:
            read_block();
            break;
        case 3:
            display_directory();
            break;
        case 4:
            break;
        default:
            printf("Invalid choice\n");
        }
    } while (choice != 4);

    return 0;
}
