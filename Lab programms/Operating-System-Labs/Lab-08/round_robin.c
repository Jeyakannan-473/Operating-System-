/* Lab-08: Round Robin scheduling with arrival times and a time quantum */
#include <stdio.h>

#define MAX 20
#define MAX_SEG 1000

static int queue[MAX];
static int front = 0, rear = 0, count = 0;

static void enqueue(int x)
{
    queue[rear] = x;
    rear = (rear + 1) % MAX;
    count++;
}

static int dequeue(void)
{
    int x = queue[front];
    front = (front + 1) % MAX;
    count--;
    return x;
}

int main(void)
{
    int at[MAX], bt[MAX], rem[MAX], ct[MAX], tat[MAX], wt[MAX];
    int order[MAX];                         /* process indexes sorted by arrival time */
    int seg_pid[MAX_SEG], seg_start[MAX_SEG], seg_end[MAX_SEG];
    int n, i, j, key, quantum, run, cur;
    int next = 0, seg_count = 0, time = 0, completed = 0;
    int total_wt = 0, total_tat = 0;

    printf("Enter number of processes: ");
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX) {
        printf("Number of processes must be between 1 and %d\n", MAX);
        return 1;
    }

    printf("Enter arrival time and burst time of each process:\n");
    for (i = 0; i < n; i++) {
        printf("P%d: ", i + 1);
        if (scanf("%d %d", &at[i], &bt[i]) != 2 || at[i] < 0 || bt[i] < 1) {
            printf("Invalid input\n");
            return 1;
        }
        rem[i] = bt[i];
        order[i] = i;
    }

    printf("Enter time quantum: ");
    if (scanf("%d", &quantum) != 1 || quantum < 1) {
        printf("Invalid time quantum\n");
        return 1;
    }

    /* Sort process indexes by arrival time (stable insertion sort) */
    for (i = 1; i < n; i++) {
        key = order[i];
        for (j = i - 1; j >= 0 && at[order[j]] > at[key]; j--)
            order[j + 1] = order[j];
        order[j + 1] = key;
    }

    while (completed < n) {
        while (next < n && at[order[next]] <= time)
            enqueue(order[next++]);

        if (count == 0) {                   /* CPU idle: jump to the next arrival */
            time = at[order[next]];
            continue;
        }

        cur = dequeue();
        run = rem[cur] < quantum ? rem[cur] : quantum;

        /* Record the Gantt chart, merging consecutive slices of the same process */
        if (seg_count > 0 && seg_pid[seg_count - 1] == cur + 1 &&
            seg_end[seg_count - 1] == time) {
            seg_end[seg_count - 1] = time + run;
        } else if (seg_count < MAX_SEG) {
            seg_pid[seg_count] = cur + 1;
            seg_start[seg_count] = time;
            seg_end[seg_count] = time + run;
            seg_count++;
        }

        time += run;
        rem[cur] -= run;

        /* Processes that arrived during this slice join the queue before cur */
        while (next < n && at[order[next]] <= time)
            enqueue(order[next++]);

        if (rem[cur] > 0) {
            enqueue(cur);
        } else {
            ct[cur] = time;
            completed++;
        }
    }

    printf("\n%-9s%-9s%-7s%-12s%-12s%s\n", "Process", "Arrival", "Burst",
           "Completion", "Turnaround", "Waiting");
    for (i = 0; i < n; i++) {
        tat[i] = ct[i] - at[i];
        wt[i] = tat[i] - bt[i];
        total_wt += wt[i];
        total_tat += tat[i];
        printf("P%-8d%-9d%-7d%-12d%-12d%d\n", i + 1, at[i], bt[i], ct[i], tat[i], wt[i]);
    }

    printf("\nGantt Chart:\n");
    for (i = 0; i < seg_count; i++)
        printf("| P%d (%d-%d) ", seg_pid[i], seg_start[i], seg_end[i]);
    printf("|\n");

    printf("\nAverage Waiting Time    = %.2f\n", (float)total_wt / n);
    printf("Average Turnaround Time = %.2f\n", (float)total_tat / n);

    return 0;
}
