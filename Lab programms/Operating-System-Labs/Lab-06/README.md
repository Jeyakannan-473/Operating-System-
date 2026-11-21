## Sample Input

```
4
0 8 3
1 4 2
2 2 1
3 3 4
```

## Sample Output

```
~/Operating-System-/Lab-06$ ./preemptive_priority
Enter number of processes: 4
Enter arrival time, burst time and priority (lower number = higher priority):
P1: 0 8 3
P2: 1 4 2
P3: 2 2 1
P4: 3 3 4

Process  Arrival  Burst  Priority  Completion  Turnaround  Waiting
P1       0        8      3         14          14          6
P2       1        4      2         7           6           2
P3       2        2      1         4           2           0
P4       3        3      4         17          14          11

Gantt Chart:
| P1 (0-1) | P2 (1-2) | P3 (2-4) | P2 (4-7) | P1 (7-14) | P4 (14-17) |

Average Waiting Time    = 4.75
Average Turnaround Time = 9.00
```
