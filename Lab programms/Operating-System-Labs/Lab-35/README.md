## Sample Input

```
3
Chennai
1
Delhi
4
Mumbai
2
Kolkata
0
2
4
7
0
```

## Sample Output

```
~/Operating-System-/Lab-35$ ./indexed_allocation
Enter blocks (0 as block number to stop):
Block number (1-20): 3
Data: Chennai
Block number (1-20): 1
Data: Delhi
Block number (1-20): 4
Data: Mumbai
Block number (1-20): 2
Data: Kolkata
Block number (1-20): 0

Index entry   Disk block
1             1
2             3
3             0
4             2

Enter the block number to read (0 to stop): 2
Index entry 2 points to disk block 3
Data: Kolkata

Enter the block number to read (0 to stop): 4
Index entry 4 points to disk block 2
Data: Mumbai

Enter the block number to read (0 to stop): 7
Block 7 is not in the index block

Enter the block number to read (0 to stop): 0

Indexed file deleted.
```
