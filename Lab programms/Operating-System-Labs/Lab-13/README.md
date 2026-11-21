## Sample Input

```
5
100
500
200
300
600
4
212
417
112
426
```

## Sample Output

```
~/Operating-System-/Lab-13$ ./memory_allocation
Enter number of memory blocks: 5
Enter size of each block:
Block 1: 100
Block 2: 500
Block 3: 200
Block 4: 300
Block 5: 600
Enter number of processes: 4
Enter size of each process:
P1: 212
P2: 417
P3: 112
P4: 426

First Fit
Process  Size          Block No
P1       212           2
P2       417           5
P3       112           2
P4       426           Not allocated
Remaining block sizes: 100 176 200 300 183

Best Fit
Process  Size          Block No
P1       212           4
P2       417           2
P3       112           3
P4       426           5
Remaining block sizes: 100 83 88 88 174

Worst Fit
Process  Size          Block No
P1       212           5
P2       417           2
P3       112           5
P4       426           Not allocated
Remaining block sizes: 100 83 200 300 276
```
