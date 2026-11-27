## Sample Input

```
8
98 183 37 122 14 124 65 67
53
```

## Sample Output

```
~/Operating-System-/Lab-37$ ./fcfs_disk
Enter number of disk requests: 8
Enter the disk request queue: 98 183 37 122 14 124 65 67
Enter the initial head position: 53

From    To      Seek
53      98      45
98      183     85
183     37      146
37      122     85
122     14      108
14      124     110
124     65      59
65      67      2

Total seek time   = 640
Average seek time = 80.00
```
