/* Lab-38: SCAN (elevator) disk scheduling - the head moves in one direction to the end of the
   disk serving requests on the way, then reverses and serves the remaining requests */
#include <stdio.h>
#include <stdlib.h>

#define MAX 50

static int head_pos;
static int total = 0;

static void move_to(int track, const char *note)
{
    int seek = abs(track - head_pos);

    total += seek;
    printf("%-8d%-8d%-8d%s\n", head_pos, track, seek, note);
    head_pos = track;
}

static int compare(const void *a, const void *b)
{
    return *(const int *)a - *(const int *)b;
}

int main(void)
{
    int queue[MAX];
    int n, tracks, direction, start, i;

    printf("Enter number of disk requests: ");
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX) {
        printf("Number of requests must be between 1 and %d\n", MAX);
        return 1;
    }
    printf("Enter the disk request queue: ");
    for (i = 0; i < n; i++) {
        if (scanf("%d", &queue[i]) != 1) {
            printf("Invalid track number\n");
            return 1;
        }
    }
    printf("Enter the initial head position: ");
    if (scanf("%d", &head_pos) != 1) {
        printf("Invalid head position\n");
        return 1;
    }
    printf("Enter total number of tracks (0 to N-1): ");
    if (scanf("%d", &tracks) != 1 || tracks < 2) {
        printf("Invalid number of tracks\n");
        return 1;
    }
    printf("Enter initial direction (0 = towards track 0, 1 = towards last track): ");
    if (scanf("%d", &direction) != 1 || (direction != 0 && direction != 1)) {
        printf("Direction must be 0 or 1\n");
        return 1;
    }

    for (i = 0; i < n; i++) {
        if (queue[i] < 0 || queue[i] >= tracks) {
            printf("Track %d is outside the disk\n", queue[i]);
            return 1;
        }
    }
    if (head_pos < 0 || head_pos >= tracks) {
        printf("Head position is outside the disk\n");
        return 1;
    }

    start = head_pos;                       /* head position when scheduling begins */
    qsort(queue, n, sizeof(int), compare);

    printf("\n%-8s%-8s%-8s%s\n", "From", "To", "Seek", "Note");
    if (direction == 1) {
        for (i = 0; i < n; i++)             /* requests at or above the head, going up */
            if (queue[i] >= start)
                move_to(queue[i], "");
        if (head_pos != tracks - 1)
            move_to(tracks - 1, "end of disk");
        for (i = n - 1; i >= 0; i--)        /* then the lower requests, going down */
            if (queue[i] < start)
                move_to(queue[i], "");
    } else {
        for (i = n - 1; i >= 0; i--)        /* requests at or below the head, going down */
            if (queue[i] <= start)
                move_to(queue[i], "");
        if (head_pos != 0)
            move_to(0, "end of disk");
        for (i = 0; i < n; i++)             /* then the higher requests, going up */
            if (queue[i] > start)
                move_to(queue[i], "");
    }

    printf("\nTotal seek time   = %d\n", total);
    printf("Average seek time = %.2f\n", (float)total / n);

    return 0;
}
