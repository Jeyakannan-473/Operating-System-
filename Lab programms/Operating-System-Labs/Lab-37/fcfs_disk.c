/* Lab-37: First Come First Served (FCFS) disk scheduling */
#include <stdio.h>
#include <stdlib.h>

#define MAX 50

int main(void)
{
    int queue[MAX];
    int n, head, i, from, seek, total = 0;

    printf("Enter number of disk requests: ");
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX) {
        printf("Number of requests must be between 1 and %d\n", MAX);
        return 1;
    }
    printf("Enter the disk request queue: ");
    for (i = 0; i < n; i++) {
        if (scanf("%d", &queue[i]) != 1 || queue[i] < 0) {
            printf("Invalid track number\n");
            return 1;
        }
    }
    printf("Enter the initial head position: ");
    if (scanf("%d", &head) != 1 || head < 0) {
        printf("Invalid head position\n");
        return 1;
    }

    /* Requests are served in the order in which they arrive */
    printf("\n%-8s%-8s%s\n", "From", "To", "Seek");
    from = head;
    for (i = 0; i < n; i++) {
        seek = abs(queue[i] - from);
        total += seek;
        printf("%-8d%-8d%d\n", from, queue[i], seek);
        from = queue[i];
    }

    printf("\nTotal seek time   = %d\n", total);
    printf("Average seek time = %.2f\n", (float)total / n);

    return 0;
}
