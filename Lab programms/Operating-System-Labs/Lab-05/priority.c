/* Lab-05: Priority scheduling (non-preemptive) - highest priority waiting process runs next */
#include <stdio.h>

#define MAX 20

struct process {
    int pid;
    int arrival;
    int burst;
    int priority;      /* lower number = higher priority */
    int completion;
    int turnaround;
    int waiting;
    int done;
};

int main(void)
{
    struct process p[MAX];
    int order[MAX];
    int n, i, k = 0, best;
    int time = 0, completed = 0;
    int total_wt = 0, total_tat = 0;

    printf("Enter number of processes: ");
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX) {
        printf("Number of processes must be between 1 and %d\n", MAX);
        return 1;
    }

    printf("Enter arrival time, burst time and priority (lower number = higher priority):\n");
    for (i = 0; i < n; i++) {
        printf("P%d: ", i + 1);
        p[i].pid = i + 1;
        p[i].done = 0;
        if (scanf("%d %d %d", &p[i].arrival, &p[i].burst, &p[i].priority) != 3 ||
            p[i].arrival < 0 || p[i].burst < 1) {
            printf("Invalid input\n");
            return 1;
        }
    }

    while (completed < n) {
        /* Pick the arrived, unfinished process with the highest priority */
        best = -1;
        for (i = 0; i < n; i++) {
            if (p[i].done || p[i].arrival > time)
                continue;
            if (best == -1 || p[i].priority < p[best].priority ||
                (p[i].priority == p[best].priority && p[i].arrival < p[best].arrival))
                best = i;
        }

        if (best == -1) {          /* nothing has arrived yet: CPU is idle */
            time++;
            continue;
        }

        time += p[best].burst;     /* runs to completion (non-preemptive) */
        p[best].completion = time;
        p[best].turnaround = p[best].completion - p[best].arrival;
        p[best].waiting = p[best].turnaround - p[best].burst;
        p[best].done = 1;
        order[k++] = p[best].pid;
        completed++;
    }

    printf("\n%-9s%-9s%-7s%-10s%-12s%-12s%s\n", "Process", "Arrival", "Burst",
           "Priority", "Completion", "Turnaround", "Waiting");
    for (i = 0; i < n; i++) {
        total_wt += p[i].waiting;
        total_tat += p[i].turnaround;
        printf("P%-8d%-9d%-7d%-10d%-12d%-12d%d\n", p[i].pid, p[i].arrival, p[i].burst,
               p[i].priority, p[i].completion, p[i].turnaround, p[i].waiting);
    }

    printf("\nOrder of execution: ");
    for (i = 0; i < n; i++)
        printf("P%d%s", order[i], i < n - 1 ? " -> " : "\n");

    printf("Average Waiting Time    = %.2f\n", (float)total_wt / n);
    printf("Average Turnaround Time = %.2f\n", (float)total_tat / n);

    return 0;
}
