/* Lab-17: Deadlock avoidance - Banker's algorithm (safety check and resource request) */
#include <stdio.h>

#define MAX_P 10
#define MAX_R 10

static int n, m;
static int alloc[MAX_P][MAX_R], maximum[MAX_P][MAX_R], need[MAX_P][MAX_R];
static int avail[MAX_R];

/* Safety algorithm: returns 1 and fills seq[] with a safe sequence if one exists */
static int is_safe(int seq[])
{
    int work[MAX_R], finish[MAX_P] = {0};
    int count = 0, found, i, j;

    for (j = 0; j < m; j++)
        work[j] = avail[j];

    while (count < n) {
        found = 0;
        for (i = 0; i < n; i++) {
            if (finish[i])
                continue;
            for (j = 0; j < m && need[i][j] <= work[j]; j++)
                ;
            if (j == m) {                       /* process i can finish */
                for (j = 0; j < m; j++)
                    work[j] += alloc[i][j];
                finish[i] = 1;
                seq[count++] = i;
                found = 1;
            }
        }
        if (!found)
            return 0;
    }
    return 1;
}

static void vector(char *buf, const int *v)
{
    int j, len = 0;

    for (j = 0; j < m; j++)
        len += sprintf(buf + len, "%d%s", v[j], j < m - 1 ? " " : "");
}

static void print_state(void)
{
    char a[40], x[40], d[40];
    int i, j;

    printf("\n%-9s%-13s%-13s%s\n", "Process", "Allocation", "Max", "Need");
    for (i = 0; i < n; i++) {
        vector(a, alloc[i]);
        vector(x, maximum[i]);
        vector(d, need[i]);
        printf("P%-8d%-13s%-13s%s\n", i, a, x, d);
    }
    printf("Available: ");
    for (j = 0; j < m; j++)
        printf("%d ", avail[j]);
    printf("\n");
}

static void print_sequence(const int seq[])
{
    int i;

    printf("Safe sequence: ");
    for (i = 0; i < n; i++)
        printf("P%d%s", seq[i], i < n - 1 ? " -> " : "\n");
}

static void request_resources(int p, const int req[])
{
    int seq[MAX_P], j;

    for (j = 0; j < m; j++) {
        if (req[j] > need[p][j]) {
            printf("Request denied: P%d has exceeded its maximum claim\n", p);
            return;
        }
    }
    for (j = 0; j < m; j++) {
        if (req[j] > avail[j]) {
            printf("Request denied: resources not available, P%d must wait\n", p);
            return;
        }
    }

    /* Pretend to allocate, then test whether the new state is safe */
    for (j = 0; j < m; j++) {
        avail[j] -= req[j];
        alloc[p][j] += req[j];
        need[p][j] -= req[j];
    }

    if (is_safe(seq)) {
        printf("Request granted. ");
        print_sequence(seq);
    } else {
        for (j = 0; j < m; j++) {           /* roll back */
            avail[j] += req[j];
            alloc[p][j] -= req[j];
            need[p][j] += req[j];
        }
        printf("Request denied: it would leave the system in an unsafe state\n");
    }
}

int main(void)
{
    int seq[MAX_P], req[MAX_R];
    int i, j, p;
    char again;

    printf("Enter number of processes: ");
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_P) {
        printf("Number of processes must be between 1 and %d\n", MAX_P);
        return 1;
    }
    printf("Enter number of resource types: ");
    if (scanf("%d", &m) != 1 || m < 1 || m > MAX_R) {
        printf("Number of resource types must be between 1 and %d\n", MAX_R);
        return 1;
    }

    printf("Enter Allocation matrix:\n");
    for (i = 0; i < n; i++) {
        printf("P%d: ", i);
        for (j = 0; j < m; j++)
            scanf("%d", &alloc[i][j]);
    }
    printf("Enter Maximum matrix:\n");
    for (i = 0; i < n; i++) {
        printf("P%d: ", i);
        for (j = 0; j < m; j++)
            scanf("%d", &maximum[i][j]);
    }
    printf("Enter Available resources: ");
    for (j = 0; j < m; j++)
        scanf("%d", &avail[j]);

    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            need[i][j] = maximum[i][j] - alloc[i][j];

    print_state();

    if (!is_safe(seq)) {
        printf("\nThe system is NOT in a safe state\n");
        return 0;
    }
    printf("\nThe system is in a safe state. ");
    print_sequence(seq);

    do {
        printf("\nEnter process number making the request: ");
        if (scanf("%d", &p) != 1 || p < 0 || p >= n) {
            printf("Invalid process number\n");
            return 1;
        }
        printf("Enter the request for %d resource types: ", m);
        for (j = 0; j < m; j++)
            scanf("%d", &req[j]);

        request_resources(p, req);

        printf("Another request? (y/n): ");
        scanf(" %c", &again);
    } while (again == 'y' || again == 'Y');

    return 0;
}
