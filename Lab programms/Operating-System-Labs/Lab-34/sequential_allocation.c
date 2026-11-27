/* Lab-34: Sequential file allocation - records are stored one after another and
   a record can only be reached by reading all the records before it */
#include <stdio.h>

#define FILE_NAME "sequential_file.dat"
#define MAX_RECORDS 50

struct record {
    int number;
    char data[100];
};

int main(void)
{
    FILE *fp;
    struct record rec;
    int keys[MAX_RECORDS], count = 0, i, duplicate;
    int wanted, records_read, found;

    fp = fopen(FILE_NAME, "wb");
    if (fp == NULL) {
        printf("Error creating the file\n");
        return 1;
    }

    /* Write the records one after another */
    printf("Enter records (0 as record number to stop):\n");
    while (count < MAX_RECORDS) {
        printf("Record number: ");
        if (scanf("%d", &rec.number) != 1 || rec.number == 0)
            break;
        if (rec.number < 0) {
            printf("Record number must be positive\n");
            continue;
        }
        duplicate = 0;
        for (i = 0; i < count; i++)
            if (keys[i] == rec.number)
                duplicate = 1;
        if (duplicate) {
            printf("Record %d already exists\n", rec.number);
            continue;
        }
        printf("Data: ");
        scanf(" %99[^\n]", rec.data);
        fwrite(&rec, sizeof(rec), 1, fp);
        keys[count++] = rec.number;
    }
    fclose(fp);
    printf("\n%d record(s) stored one after another in the file.\n", count);

    /* Read records: the file is always scanned from the beginning */
    fp = fopen(FILE_NAME, "rb");
    if (fp == NULL) {
        printf("Error opening the file\n");
        return 1;
    }
    while (count > 0) {
        printf("\nEnter the record number to read (0 to stop): ");
        if (scanf("%d", &wanted) != 1 || wanted == 0)
            break;

        rewind(fp);
        records_read = 0;
        found = 0;
        printf("Reading the file from the beginning:\n");
        while (fread(&rec, sizeof(rec), 1, fp) == 1) {
            records_read++;
            printf("  Record %d: %s\n", rec.number, rec.data);
            if (rec.number == wanted) {
                found = 1;
                break;
            }
        }
        if (found)
            printf("Record %d found after reading %d record(s)\n", wanted, records_read);
        else
            printf("Record %d not found (all %d record(s) were read)\n", wanted, records_read);
    }
    fclose(fp);

    remove(FILE_NAME);
    printf("\nSequential file deleted.\n");
    return 0;
}
