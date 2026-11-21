## Sample Input

```
4
0 7
2 4
4 1
5 4
```

## Sample Output

```
~/Operating-System-/Lab-07$ ./sjf_non_preemptive
Enter number of processes: 4
Enter arrival time and burst time of each process:
P1: 0 7
P2: 2 4
P3: 4 1
P4: 5 4

Process  Arrival  Burst  Start  End   Waiting  Turnaround
P1       0        7      0      7     0        7
P2       2        4      8      12    6        10
P3       4        1      7      8     3        4
P4       5        4      12     16    7        11

Average Waiting Time    = 4.00
Average Turnaround Time = 8.00
```
