/* Lab-13: Memory allocation strategies - First Fit, Best Fit and Worst Fit */
#include <stdio.h>

#define MAX 20

enum strategy { FIRST_FIT, BEST_FIT, WORST_FIT };

static void allocate(const char *title, enum strategy s, int nb, const int block[],
                     int np, const int process[])
{
    int remaining[MAX];
    int i, j, chosen;

    for (j = 0; j < nb; j++)
        remaining[j] = block[j];

    printf("\n%s\n", title);
    printf("%-9s%-14s%s\n", "Process", "Size", "Block No");

    for (i = 0; i < np; i++) {
        chosen = -1;
        for (j = 0; j < nb; j++) {
            if (remaining[j] < process[i])
                continue;
            if (s == FIRST_FIT) {
                chosen = j;
                break;
            }
            if (chosen == -1 ||
                (s == BEST_FIT && remaining[j] < remaining[chosen]) ||
                (s == WORST_FIT && remaining[j] > remaining[chosen]))
                chosen = j;
        }

        if (chosen == -1) {
            printf("P%-8d%-14dNot allocated\n", i + 1, process[i]);
        } else {
            remaining[chosen] -= process[i];
            printf("P%-8d%-14d%d\n", i + 1, process[i], chosen + 1);
        }
    }

    printf("Remaining block sizes:");
    for (j = 0; j < nb; j++)
        printf(" %d", remaining[j]);
    printf("\n");
}

int main(void)
{
    int block[MAX], process[MAX];
    int nb, np, i;

    printf("Enter number of memory blocks: ");
    if (scanf("%d", &nb) != 1 || nb < 1 || nb > MAX) {
        printf("Number of blocks must be between 1 and %d\n", MAX);
        return 1;
    }
    printf("Enter size of each block:\n");
    for (i = 0; i < nb; i++) {
        printf("Block %d: ", i + 1);
        if (scanf("%d", &block[i]) != 1 || block[i] < 1) {
            printf("Invalid block size\n");
            return 1;
        }
    }

    printf("Enter number of processes: ");
    if (scanf("%d", &np) != 1 || np < 1 || np > MAX) {
        printf("Number of processes must be between 1 and %d\n", MAX);
        return 1;
    }
    printf("Enter size of each process:\n");
    for (i = 0; i < np; i++) {
        printf("P%d: ", i + 1);
        if (scanf("%d", &process[i]) != 1 || process[i] < 1) {
            printf("Invalid process size\n");
            return 1;
        }
    }

    allocate("First Fit", FIRST_FIT, nb, block, np, process);
    allocate("Best Fit", BEST_FIT, nb, block, np, process);
    allocate("Worst Fit", WORST_FIT, nb, block, np, process);

    return 0;
}
