/* Lab-26: File management operations - create, write, read, append, rename and delete */
#include <stdio.h>

#define RENAMED_FILE "renamed.txt"

static int show_file(const char *name)
{
    FILE *fp = fopen(name, "r");
    char line[200];

    if (fp == NULL) {
        printf("Error opening %s for reading\n", name);
        return 0;
    }
    printf("Contents of %s:\n", name);
    while (fgets(line, sizeof(line), fp) != NULL)
        printf("  %s", line);
    fclose(fp);
    return 1;
}

int main(void)
{
    char name[100], text[200], more[200];
    FILE *fp;

    printf("Enter file name: ");
    scanf("%99s", name);
    printf("Enter text to write: ");
    scanf(" %199[^\n]", text);
    printf("Enter text to append: ");
    scanf(" %199[^\n]", more);

    /* Create the file and write to it */
    fp = fopen(name, "w");
    if (fp == NULL) {
        printf("Error opening %s for writing\n", name);
        return 1;
    }
    fprintf(fp, "%s\n", text);
    fclose(fp);
    printf("\nFile %s created and text written\n", name);

    /* Read the file */
    if (!show_file(name))
        return 1;

    /* Append more text */
    fp = fopen(name, "a");
    if (fp == NULL) {
        printf("Error opening %s for appending\n", name);
        return 1;
    }
    fprintf(fp, "%s\n", more);
    fclose(fp);
    printf("\nText appended to %s\n", name);
    if (!show_file(name))
        return 1;

    /* Rename and then delete the file */
    if (rename(name, RENAMED_FILE) != 0) {
        perror("rename");
        return 1;
    }
    printf("\nFile %s renamed to %s\n", name, RENAMED_FILE);

    if (remove(RENAMED_FILE) != 0) {
        perror("remove");
        return 1;
    }
    printf("File %s deleted\n", RENAMED_FILE);

    return 0;
}
