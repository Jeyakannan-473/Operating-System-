## Sample Input

```
8
98 183 37 122 14 124 65 67
53
200
```

## Sample Output

```
~/Operating-System-/Lab-39$ ./cscan_disk
Enter number of disk requests: 8
Enter the disk request queue: 98 183 37 122 14 124 65 67
Enter the initial head position: 53
Enter total number of tracks (0 to N-1): 200

From    To      Seek    Note
53      65      12
65      67      2
67      98      31
98      122     24
122     124     2
124     183     59
183     199     16      end of disk
199     0       199     return jump
0       14      14
14      37      23

Total seek time   = 382 (the return jump is included)
Average seek time = 47.75
```
