/* Lab-22: Best fit memory management (contiguous allocation of 1000 memory units) */
#include <stdio.h>

#define MEMORY_SIZE 1000

static int memory[MEMORY_SIZE];     /* 0 = free, otherwise the id of the owning process */

static int in_use(int pid)
{
    int i;

    for (i = 0; i < MEMORY_SIZE; i++)
        if (memory[i] == pid)
            return 1;
    return 0;
}

static void display(void)
{
    int i = 0, j;

    printf("Memory map:\n");
    while (i < MEMORY_SIZE) {
        j = i;
        while (j < MEMORY_SIZE && memory[j] == memory[i])
            j++;
        if (memory[i] == 0)
            printf("%4d-%-5d%-8s(%d)\n", i, j - 1, "Free", j - i);
        else {
            char label[12];
            snprintf(label, sizeof(label), "P%d", memory[i]);
            printf("%4d-%-5d%-8s(%d)\n", i, j - 1, label, j - i);
        }
        i = j;
    }
}

/* Best fit: use the smallest free hole that is big enough */
static int allocate(int pid, int size, int *start)
{
    int i = 0, j, len, chosen = -1, chosen_len = 0;

    while (i < MEMORY_SIZE) {
        if (memory[i] != 0) {
            i++;
            continue;
        }
        j = i;
        while (j < MEMORY_SIZE && memory[j] == 0)
            j++;
        len = j - i;                    /* free hole: memory[i .. j-1] */
        if (len >= size && (chosen == -1 || len < chosen_len)) {
            chosen = i;
            chosen_len = len;
        }
        i = j;
    }

    if (chosen == -1)
        return 0;

    for (i = chosen; i < chosen + size; i++)
        memory[i] = pid;
    *start = chosen;
    return 1;
}

static void release(int pid)
{
    int i;

    for (i = 0; i < MEMORY_SIZE; i++)
        if (memory[i] == pid)
            memory[i] = 0;
}

int main(void)
{
    int choice, pid, size, start;

    do {
        printf("\n1.Allocate 2.Release 3.Display 4.Exit - Enter your choice: ");
        if (scanf("%d", &choice) != 1)
            return 1;

        switch (choice) {
        case 1:
            printf("Enter process id: ");
            scanf("%d", &pid);
            printf("Enter size: ");
            scanf("%d", &size);
            if (pid < 1 || size < 1 || size > MEMORY_SIZE)
                printf("Invalid process id or size\n");
            else if (in_use(pid))
                printf("Process P%d already has memory allocated\n", pid);
            else if (allocate(pid, size, &start))
                printf("Process P%d allocated memory %d-%d\n", pid, start, start + size - 1);
            else
                printf("Allocation failed for P%d: no free hole of %d units\n", pid, size);
            break;
        case 2:
            printf("Enter process id: ");
            scanf("%d", &pid);
            if (pid < 1 || !in_use(pid))
                printf("Process P%d not found\n", pid);
            else {
                release(pid);
                printf("Process P%d released its memory\n", pid);
            }
            break;
        case 3:
            display();
            break;
        case 4:
            break;
        default:
            printf("Invalid choice\n");
        }
    } while (choice != 4);

    return 0;
}
