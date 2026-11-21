## Sample Input

```
4
0 5 2
1 3 1
2 8 4
3 6 3
```

## Sample Output

```
~/Operating-System-/Lab-05$ ./priority
Enter number of processes: 4
Enter arrival time, burst time and priority (lower number = higher priority):
P1: 0 5 2
P2: 1 3 1
P3: 2 8 4
P4: 3 6 3

Process  Arrival  Burst  Priority  Completion  Turnaround  Waiting
P1       0        5      2         5           5           0
P2       1        3      1         8           7           4
P3       2        8      4         22          20          12
P4       3        6      3         14          11          5

Order of execution: P1 -> P2 -> P4 -> P3
Average Waiting Time    = 5.25
Average Turnaround Time = 10.75
```
