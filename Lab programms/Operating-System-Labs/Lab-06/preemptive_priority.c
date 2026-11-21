/* Lab-06: Preemptive priority scheduling - a higher priority arrival preempts the running process */
#include <stdio.h>

#define MAX 20
#define MAX_SEG 1000

struct process {
    int pid;
    int arrival;
    int burst;
    int priority;      /* lower number = higher priority */
    int remaining;
    int completion;
    int turnaround;
    int waiting;
};

int main(void)
{
    struct process p[MAX];
    int seg_pid[MAX_SEG], seg_start[MAX_SEG], seg_end[MAX_SEG];
    int n, i, best, seg_count = 0;
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
        if (scanf("%d %d %d", &p[i].arrival, &p[i].burst, &p[i].priority) != 3 ||
            p[i].arrival < 0 || p[i].burst < 1) {
            printf("Invalid input\n");
            return 1;
        }
        p[i].remaining = p[i].burst;
    }

    while (completed < n) {
        /* At every time unit pick the arrived process with the highest priority */
        best = -1;
        for (i = 0; i < n; i++) {
            if (p[i].remaining == 0 || p[i].arrival > time)
                continue;
            if (best == -1 || p[i].priority < p[best].priority ||
                (p[i].priority == p[best].priority && p[i].arrival < p[best].arrival))
                best = i;
        }

        if (best == -1) {          /* CPU idle */
            time++;
            continue;
        }

        /* Record the Gantt chart, merging consecutive units of the same process */
        if (seg_count > 0 && seg_pid[seg_count - 1] == p[best].pid &&
            seg_end[seg_count - 1] == time) {
            seg_end[seg_count - 1] = time + 1;
        } else if (seg_count < MAX_SEG) {
            seg_pid[seg_count] = p[best].pid;
            seg_start[seg_count] = time;
            seg_end[seg_count] = time + 1;
            seg_count++;
        }

        p[best].remaining--;
        time++;

        if (p[best].remaining == 0) {
            p[best].completion = time;
            p[best].turnaround = p[best].completion - p[best].arrival;
            p[best].waiting = p[best].turnaround - p[best].burst;
            completed++;
        }
    }

    printf("\n%-9s%-9s%-7s%-10s%-12s%-12s%s\n", "Process", "Arrival", "Burst",
           "Priority", "Completion", "Turnaround", "Waiting");
    for (i = 0; i < n; i++) {
        total_wt += p[i].waiting;
        total_tat += p[i].turnaround;
        printf("P%-8d%-9d%-7d%-10d%-12d%-12d%d\n", p[i].pid, p[i].arrival, p[i].burst,
               p[i].priority, p[i].completion, p[i].turnaround, p[i].waiting);
    }

    printf("\nGantt Chart:\n");
    for (i = 0; i < seg_count; i++)
        printf("| P%d (%d-%d) ", seg_pid[i], seg_start[i], seg_end[i]);
    printf("|\n");

    printf("\nAverage Waiting Time    = %.2f\n", (float)total_wt / n);
    printf("Average Turnaround Time = %.2f\n", (float)total_tat / n);

    return 0;
}
