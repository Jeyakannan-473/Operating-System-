## Sample Input

```
5
3
0 1 0
2 0 0
3 0 2
2 1 1
0 0 2
7 5 3
3 2 2
9 0 2
2 2 2
4 3 3
3 3 2
1
1 0 2
y
4
3 3 0
y
0
0 2 0
n
```

## Sample Output

```
~/Operating-System-/Lab-17$ ./bankers_algorithm
Enter number of processes: 5
Enter number of resource types: 3
Enter Allocation matrix:
P0: 0 1 0
P1: 2 0 0
P2: 3 0 2
P3: 2 1 1
P4: 0 0 2
Enter Maximum matrix:
P0: 7 5 3
P1: 3 2 2
P2: 9 0 2
P3: 2 2 2
P4: 4 3 3
Enter Available resources: 3 3 2

Process  Allocation   Max          Need
P0       0 1 0        7 5 3        7 4 3
P1       2 0 0        3 2 2        1 2 2
P2       3 0 2        9 0 2        6 0 0
P3       2 1 1        2 2 2        0 1 1
P4       0 0 2        4 3 3        4 3 1
Available: 3 3 2

The system is in a safe state. Safe sequence: P1 -> P3 -> P4 -> P0 -> P2

Enter process number making the request: 1
Enter the request for 3 resource types: 1 0 2
Request granted. Safe sequence: P1 -> P3 -> P4 -> P0 -> P2
Another request? (y/n): y

Enter process number making the request: 4
Enter the request for 3 resource types: 3 3 0
Request denied: resources not available, P4 must wait
Another request? (y/n): y

Enter process number making the request: 0
Enter the request for 3 resource types: 0 2 0
Request denied: it would leave the system in an unsafe state
Another request? (y/n): n
```
