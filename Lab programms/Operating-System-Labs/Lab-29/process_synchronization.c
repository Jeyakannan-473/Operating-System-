/* Lab-29: Classical process synchronization problem - bounded-buffer producer/consumer */
#include <stdio.h>

#define BUFFER_SIZE 3

static int mutex = 1;                   /* 1 = buffer free, 0 = in use */
static int full = 0;                    /* number of filled slots */
static int empty = BUFFER_SIZE;         /* number of empty slots */
static int buffer[BUFFER_SIZE];
static int in = 0, out = 0;
static int next_item = 1;

static void show_buffer(void)
{
    int i;

    printf("Buffer (%d/%d):", full, BUFFER_SIZE);
    for (i = 0; i < full; i++)
        printf(" %d", buffer[(out + i) % BUFFER_SIZE]);
    printf("\n");
}

static void producer(void)
{
    --mutex;                            /* wait(mutex) */
    buffer[in] = next_item;
    printf("Producer produces item %d\n", next_item++);
    in = (in + 1) % BUFFER_SIZE;
    ++full;
    --empty;
    ++mutex;                            /* signal(mutex) */
}

static void consumer(void)
{
    --mutex;                            /* wait(mutex) */
    printf("Consumer consumes item %d\n", buffer[out]);
    out = (out + 1) % BUFFER_SIZE;
    --full;
    ++empty;
    ++mutex;                            /* signal(mutex) */
}

int main(void)
{
    int choice;

    printf("1. Producer  2. Consumer  3. Exit\n");
    do {
        printf("\nEnter your choice: ");
        if (scanf("%d", &choice) != 1)
            return 1;

        switch (choice) {
        case 1:
            if (mutex == 1 && empty != 0) {
                producer();
                show_buffer();
            } else {
                printf("Buffer is full!\n");
            }
            break;
        case 2:
            if (mutex == 1 && full != 0) {
                consumer();
                show_buffer();
            } else {
                printf("Buffer is empty!\n");
            }
            break;
        case 3:
            break;
        default:
            printf("Invalid choice\n");
        }
    } while (choice != 3);

    return 0;
}
