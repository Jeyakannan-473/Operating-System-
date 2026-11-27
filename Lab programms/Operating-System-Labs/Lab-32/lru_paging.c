/* Lab-32: Least Recently Used (LRU) page replacement */
#include <stdio.h>

#define MAX_FRAMES 10
#define MAX_PAGES 50

static void print_frames(const int frames[], int nf)
{
    int j;

    for (j = 0; j < nf; j++) {
        if (frames[j] == -1)
            printf("%-4s", "-");
        else
            printf("%-4d", frames[j]);
    }
}

/* Reads the number of frames and the reference string; returns 0 on bad input */
static int read_input(int *nf, int *np, int pages[])
{
    int i;

    printf("Enter number of frames: ");
    if (scanf("%d", nf) != 1 || *nf < 1 || *nf > MAX_FRAMES) {
        printf("Number of frames must be between 1 and %d\n", MAX_FRAMES);
        return 0;
    }
    printf("Enter number of pages in the reference string: ");
    if (scanf("%d", np) != 1 || *np < 1 || *np > MAX_PAGES) {
        printf("Number of pages must be between 1 and %d\n", MAX_PAGES);
        return 0;
    }
    printf("Enter the reference string: ");
    for (i = 0; i < *np; i++) {
        if (scanf("%d", &pages[i]) != 1 || pages[i] < 0) {
            printf("Invalid page number\n");
            return 0;
        }
    }
    return 1;
}

int main(void)
{
    int pages[MAX_PAGES], frames[MAX_FRAMES];
    int last_used[MAX_FRAMES];          /* position in the string of each frame's latest use */
    int nf, np, i, j, slot, found;
    int faults = 0, hits = 0;

    if (!read_input(&nf, &np, pages))
        return 1;

    for (j = 0; j < nf; j++)
        frames[j] = -1;                 /* -1 means the frame is empty */

    printf("\n%-6s%-*s%s\n", "Page", nf * 4, "Frames", "Status");
    for (i = 0; i < np; i++) {
        found = -1;
        for (j = 0; j < nf; j++)
            if (frames[j] == pages[i])
                found = j;

        printf("%-6d", pages[i]);
        if (found != -1) {
            hits++;
            last_used[found] = i;       /* the page was just used */
            print_frames(frames, nf);
            printf("Page hit\n");
        } else {
            faults++;
            slot = -1;                  /* use an empty frame if there is one */
            for (j = 0; j < nf && slot == -1; j++)
                if (frames[j] == -1)
                    slot = j;
            if (slot == -1) {           /* otherwise replace the least recently used page */
                slot = 0;
                for (j = 1; j < nf; j++)
                    if (last_used[j] < last_used[slot])
                        slot = j;
            }
            frames[slot] = pages[i];
            last_used[slot] = i;
            print_frames(frames, nf);
            printf("Page fault\n");
        }
    }

    printf("\nTotal page faults = %d\n", faults);
    printf("Total page hits   = %d\n", hits);

    return 0;
}
