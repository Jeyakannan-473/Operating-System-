## Sample Input

```
2
1
1
1
1
2
2
1
2
2
2
3
```

## Sample Output

```
~/Operating-System-/Lab-29$ ./process_synchronization
1. Producer  2. Consumer  3. Exit

Enter your choice: 2
Buffer is empty!

Enter your choice: 1
Producer produces item 1
Buffer (1/3): 1

Enter your choice: 1
Producer produces item 2
Buffer (2/3): 1 2

Enter your choice: 1
Producer produces item 3
Buffer (3/3): 1 2 3

Enter your choice: 1
Buffer is full!

Enter your choice: 2
Consumer consumes item 1
Buffer (2/3): 2 3

Enter your choice: 2
Consumer consumes item 2
Buffer (1/3): 3

Enter your choice: 1
Producer produces item 4
Buffer (2/3): 3 4

Enter your choice: 2
Consumer consumes item 3
Buffer (1/3): 4

Enter your choice: 2
Consumer consumes item 4
Buffer (0/3):

Enter your choice: 2
Buffer is empty!

Enter your choice: 3
```
