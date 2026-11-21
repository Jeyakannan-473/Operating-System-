/* Lab-07: Non-preemptive Shortest Job First (SJF) scheduling with arrival times */
#include <stdio.h>

#define MAX 20

struct process {
    int pid;
    int arrival;
    int burst;
    int start;
    int completion;
    int turnaround;
    int waiting;
    int done;
};

int main(void)
{
    struct process p[MAX];
    int n, i, best;
    int time = 0, completed = 0;
    int total_wt = 0, total_tat = 0;

    printf("Enter number of processes: ");
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX) {
        printf("Number of processes must be between 1 and %d\n", MAX);
        return 1;
    }

    printf("Enter arrival time and burst time of each process:\n");
    for (i = 0; i < n; i++) {
        printf("P%d: ", i + 1);
        p[i].pid = i + 1;
        p[i].done = 0;
        if (scanf("%d %d", &p[i].arrival, &p[i].burst) != 2 ||
            p[i].arrival < 0 || p[i].burst < 1) {
            printf("Invalid input\n");
            return 1;
        }
    }

    while (completed < n) {
        /* Among the arrived, unfinished processes choose the shortest burst time */
        best = -1;
        for (i = 0; i < n; i++) {
            if (p[i].done || p[i].arrival > time)
                continue;
            if (best == -1 || p[i].burst < p[best].burst ||
                (p[i].burst == p[best].burst && p[i].arrival < p[best].arrival))
                best = i;
        }

        if (best == -1) {          /* nothing has arrived yet: CPU is idle */
            time++;
            continue;
        }

        p[best].start = time;
        time += p[best].burst;     /* runs to completion (non-preemptive) */
        p[best].completion = time;
        p[best].turnaround = p[best].completion - p[best].arrival;
        p[best].waiting = p[best].turnaround - p[best].burst;
        p[best].done = 1;
        completed++;
    }

    printf("\n%-9s%-9s%-7s%-7s%-6s%-9s%s\n", "Process", "Arrival", "Burst",
           "Start", "End", "Waiting", "Turnaround");
    for (i = 0; i < n; i++) {
        total_wt += p[i].waiting;
        total_tat += p[i].turnaround;
        printf("P%-8d%-9d%-7d%-7d%-6d%-9d%d\n", p[i].pid, p[i].arrival, p[i].burst,
               p[i].start, p[i].completion, p[i].waiting, p[i].turnaround);
    }

    printf("\nAverage Waiting Time    = %.2f\n", (float)total_wt / n);
    printf("Average Turnaround Time = %.2f\n", (float)total_tat / n);

    return 0;
}
