## Sample Input

```
1
report
4
9
Hello
16
Operating
1
Systems
10
Lab
1
notes
2
5
Linked
16
20
allocation
3
2
report
3
4
```

## Sample Output

```
~/Operating-System-/Lab-36$ ./linked_allocation

1.Create file 2.Read block 3.Display directory 4.Exit - Enter your choice: 1
Enter file name: report
Enter number of blocks (1-10): 4
Block 1 - disk block number (0-29): 9
Data: Hello
Block 2 - disk block number (0-29): 16
Data: Operating
Block 3 - disk block number (0-29): 1
Data: Systems
Block 4 - disk block number (0-29): 10
Data: Lab
File report created

1.Create file 2.Read block 3.Display directory 4.Exit - Enter your choice: 1
Enter file name: notes
Enter number of blocks (1-10): 2
Block 1 - disk block number (0-29): 5
Data: Linked
Block 2 - disk block number (0-29): 16
Disk block 16 is already allocated
Block 2 - disk block number (0-29): 20
Data: allocation
File notes created

1.Create file 2.Read block 3.Display directory 4.Exit - Enter your choice: 3
File      First   Last    Length  Chain of disk blocks
report    9       10      4       9 -> 16 -> 1 -> 10 -> end
notes     5       20      2       5 -> 20 -> end

1.Create file 2.Read block 3.Display directory 4.Exit - Enter your choice: 2
Enter file name: report
Enter block number to read (1-4): 3
Following the pointers from the first block: 9 -> 16 -> 1
Block 3 of report is disk block 1, data: Systems

1.Create file 2.Read block 3.Display directory 4.Exit - Enter your choice: 4
```
