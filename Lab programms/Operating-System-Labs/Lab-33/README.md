## Sample Input

```
3
13
7 0 1 2 0 3 0 4 2 3 0 3 2
```

## Sample Output

```
~/Operating-System-/Lab-33$ ./optimal_paging
Enter number of frames: 3
Enter number of pages in the reference string: 13
Enter the reference string: 7 0 1 2 0 3 0 4 2 3 0 3 2

Page  Frames      Status
7     7   -   -   Page fault
0     7   0   -   Page fault
1     7   0   1   Page fault
2     2   0   1   Page fault
0     2   0   1   Page hit
3     2   0   3   Page fault
0     2   0   3   Page hit
4     2   4   3   Page fault
2     2   4   3   Page hit
3     2   4   3   Page hit
0     2   0   3   Page fault
3     2   0   3   Page hit
2     2   0   3   Page hit

Total page faults = 7
Total page hits   = 6
```
