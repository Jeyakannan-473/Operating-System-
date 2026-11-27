/* Lab-35: Indexed file allocation - an index block holds a pointer to every block of the file */
#include <stdio.h>

#define FILE_NAME "indexed_file.dat"
#define MAX_BLOCKS 20

struct block {
    int number;
    char data[100];
};

int main(void)
{
    FILE *fp;
    struct block blk;
    int index_block[MAX_BLOCKS + 1];    /* index_block[i] = disk block holding block i, -1 = none */
    int disk_blocks = 0, i, wanted;

    for (i = 0; i <= MAX_BLOCKS; i++)
        index_block[i] = -1;

    fp = fopen(FILE_NAME, "wb");
    if (fp == NULL) {
        printf("Error creating the file\n");
        return 1;
    }

    /* Write blocks: each one goes to the next free disk block and the index is updated */
    printf("Enter blocks (0 as block number to stop):\n");
    while (1) {
        printf("Block number (1-%d): ", MAX_BLOCKS);
        if (scanf("%d", &blk.number) != 1 || blk.number == 0)
            break;
        if (blk.number < 1 || blk.number > MAX_BLOCKS) {
            printf("Block number must be between 1 and %d\n", MAX_BLOCKS);
            continue;
        }
        if (index_block[blk.number] != -1) {
            printf("Block %d is already written\n", blk.number);
            continue;
        }
        printf("Data: ");
        scanf(" %99[^\n]", blk.data);
        fwrite(&blk, sizeof(blk), 1, fp);
        index_block[blk.number] = disk_blocks++;
    }
    fclose(fp);

    printf("\n%-14s%s\n", "Index entry", "Disk block");
    for (i = 1; i <= MAX_BLOCKS; i++)
        if (index_block[i] != -1)
            printf("%-14d%d\n", i, index_block[i]);

    /* Read blocks: the index block gives the disk block directly (random access) */
    fp = fopen(FILE_NAME, "rb");
    if (fp == NULL) {
        printf("Error opening the file\n");
        return 1;
    }
    while (disk_blocks > 0) {
        printf("\nEnter the block number to read (0 to stop): ");
        if (scanf("%d", &wanted) != 1 || wanted == 0)
            break;

        if (wanted < 1 || wanted > MAX_BLOCKS || index_block[wanted] == -1) {
            printf("Block %d is not in the index block\n", wanted);
            continue;
        }
        fseek(fp, (long)index_block[wanted] * sizeof(blk), SEEK_SET);
        if (fread(&blk, sizeof(blk), 1, fp) != 1) {
            printf("Error reading the disk block\n");
            break;
        }
        printf("Index entry %d points to disk block %d\n", wanted, index_block[wanted]);
        printf("Data: %s\n", blk.data);
    }
    fclose(fp);

    remove(FILE_NAME);
    printf("\nIndexed file deleted.\n");
    return 0;
}
