## Sample Input

```
8
98 183 37 122 14 124 65 67
53
200
0

8
98 183 37 122 14 124 65 67
53
200
1
```

## Sample Output

```
~/Operating-System-/Lab-38$ ./scan_disk
Enter number of disk requests: 8
Enter the disk request queue: 98 183 37 122 14 124 65 67
Enter the initial head position: 53
Enter total number of tracks (0 to N-1): 200
Enter initial direction (0 = towards track 0, 1 = towards last track): 0

From    To      Seek    Note
53      37      16
37      14      23
14      0       14      end of disk
0       65      65
65      67      2
67      98      31
98      122     24
122     124     2
124     183     59

Total seek time   = 236
Average seek time = 29.50
~/Operating-System-/Lab-38$ ./scan_disk
Enter number of disk requests: 8
Enter the disk request queue: 98 183 37 122 14 124 65 67
Enter the initial head position: 53
Enter total number of tracks (0 to N-1): 200
Enter initial direction (0 = towards track 0, 1 = towards last track): 1

From    To      Seek    Note
53      65      12
65      67      2
67      98      31
98      122     24
122     124     2
124     183     59
183     199     16      end of disk
199     37      162
37      14      23

Total seek time   = 331
Average seek time = 41.38
```
