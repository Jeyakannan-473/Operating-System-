/* Lab-28: Simulating the UNIX grep command - usage: grep_simulation [-i] [-n] pattern filename */
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINE 1024

/* Returns 1 if pattern occurs in line (optionally ignoring case) */
static int contains(const char *line, const char *pattern, int ignore_case)
{
    size_t plen = strlen(pattern), llen = strlen(line), i, j;

    if (plen == 0)
        return 1;
    for (i = 0; i + plen <= llen; i++) {
        for (j = 0; j < plen; j++) {
            char a = line[i + j], b = pattern[j];
            if (ignore_case) {
                a = (char)tolower((unsigned char)a);
                b = (char)tolower((unsigned char)b);
            }
            if (a != b)
                break;
        }
        if (j == plen)
            return 1;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    int ignore_case = 0, show_numbers = 0;
    int arg = 1, lineno = 0, matches = 0;
    char line[MAX_LINE];
    const char *pattern, *filename;
    FILE *fp;

    /* Options: -i (ignore case) and -n (show line numbers) */
    while (arg < argc && argv[arg][0] == '-' && argv[arg][1] != '\0') {
        const char *p;
        for (p = argv[arg] + 1; *p != '\0'; p++) {
            if (*p == 'i')
                ignore_case = 1;
            else if (*p == 'n')
                show_numbers = 1;
            else {
                fprintf(stderr, "Unknown option -%c\n", *p);
                return 2;
            }
        }
        arg++;
    }

    if (argc - arg != 2) {
        fprintf(stderr, "Usage: %s [-i] [-n] pattern filename\n", argv[0]);
        return 2;
    }
    pattern = argv[arg];
    filename = argv[arg + 1];

    fp = fopen(filename, "r");
    if (fp == NULL) {
        perror(filename);
        return 2;
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        lineno++;
        if (contains(line, pattern, ignore_case)) {
            if (show_numbers)
                printf("%d:", lineno);
            printf("%s", line);
            if (line[strlen(line) - 1] != '\n')
                printf("\n");
            matches++;
        }
    }
    fclose(fp);

    return matches > 0 ? 0 : 1;        /* like grep: 0 if something matched, 1 if not */
}
