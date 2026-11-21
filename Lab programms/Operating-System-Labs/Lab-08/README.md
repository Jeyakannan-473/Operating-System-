## Sample Input

```
4
0 5
1 3
2 1
3 2
2
```

## Sample Output

```
~/Operating-System-/Lab-08$ ./round_robin
Enter number of processes: 4
Enter arrival time and burst time of each process:
P1: 0 5
P2: 1 3
P3: 2 1
P4: 3 2
Enter time quantum: 2

Process  Arrival  Burst  Completion  Turnaround  Waiting
P1       0        5      11          11          6
P2       1        3      10          9           6
P3       2        1      5           3           2
P4       3        2      9           6           4

Gantt Chart:
| P1 (0-2) | P2 (2-4) | P3 (4-5) | P1 (5-7) | P4 (7-9) | P2 (9-10) | P1 (10-11) |

Average Waiting Time    = 4.50
Average Turnaround Time = 7.25
```
