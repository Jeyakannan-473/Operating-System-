/* Lab-04: Select the waiting process with the smallest execution time (SJF) - all arrive at time 0 */
#include <stdio.h>

#define MAX 20

int main(void)
{
    int n, i, j, key_bt, key_pid;
    int pid[MAX], bt[MAX], wt[MAX], tat[MAX];
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
        pid[i] = i + 1;
    }

    /* Sort by burst time (shortest first); equal burst times keep input order */
    for (i = 1; i < n; i++) {
        key_bt = bt[i];
        key_pid = pid[i];
        for (j = i - 1; j >= 0 && bt[j] > key_bt; j--) {
            bt[j + 1] = bt[j];
            pid[j + 1] = pid[j];
        }
        bt[j + 1] = key_bt;
        pid[j + 1] = key_pid;
    }

    wt[0] = 0;
    for (i = 1; i < n; i++)
        wt[i] = wt[i - 1] + bt[i - 1];

    printf("\n%-9s%-12s%-14s%s\n", "Process", "Burst Time", "Waiting Time", "Turnaround Time");
    for (i = 0; i < n; i++) {
        tat[i] = wt[i] + bt[i];
        total_wt += wt[i];
        total_tat += tat[i];
        printf("P%-8d%-12d%-14d%d\n", pid[i], bt[i], wt[i], tat[i]);
    }

    printf("\nAverage Waiting Time    = %.2f\n", (float)total_wt / n);
    printf("Average Turnaround Time = %.2f\n", (float)total_tat / n);

    return 0;
}
