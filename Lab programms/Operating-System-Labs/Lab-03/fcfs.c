/* Lab-03: First Come First Served (FCFS) scheduling - all processes arrive at time 0 */
#include <stdio.h>

#define MAX 20

int main(void)
{
    int n, i;
    int bt[MAX], wt[MAX], tat[MAX];
    int total_wt = 0, total_tat = 0;

    printf("Enter number of processes: ");
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX) {
        printf("Number of processes must be between 1 and %d\n", MAX);
        return 1;
    }

    printf("Enter burst time of each process:\n");
    for (i = 0; i < n; i++) {
        printf("P%d: ", i + 1);
        if (scanf("%d", &bt[i]) != 1 || bt[i] < 1) {
            printf("Invalid burst time\n");
            return 1;
        }
    }

    /* Processes run in the order in which they arrive (P1, P2, ...) */
    wt[0] = 0;
    for (i = 1; i < n; i++)
        wt[i] = wt[i - 1] + bt[i - 1];

    printf("\n%-9s%-12s%-14s%s\n", "Process", "Burst Time", "Waiting Time", "Turnaround Time");
    for (i = 0; i < n; i++) {
        tat[i] = wt[i] + bt[i];
        total_wt += wt[i];
        total_tat += tat[i];
        printf("P%-8d%-12d%-14d%d\n", i + 1, bt[i], wt[i], tat[i]);
    }

    printf("\nAverage Waiting Time    = %.2f\n", (float)total_wt / n);
    printf("Average Turnaround Time = %.2f\n", (float)total_tat / n);

    return 0;
}
