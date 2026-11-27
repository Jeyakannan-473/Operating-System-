## Sample Input

```
101
Ravi
102
Anita
103
Karthik
104
Meena
0
103
101
110
0
```

## Sample Output

```
~/Operating-System-/Lab-34$ ./sequential_allocation
Enter records (0 as record number to stop):
Record number: 101
Data: Ravi
Record number: 102
Data: Anita
Record number: 103
Data: Karthik
Record number: 104
Data: Meena
Record number: 0

4 record(s) stored one after another in the file.

Enter the record number to read (0 to stop): 103
Reading the file from the beginning:
  Record 101: Ravi
  Record 102: Anita
  Record 103: Karthik
Record 103 found after reading 3 record(s)

Enter the record number to read (0 to stop): 101
Reading the file from the beginning:
  Record 101: Ravi
Record 101 found after reading 1 record(s)

Enter the record number to read (0 to stop): 110
Reading the file from the beginning:
  Record 101: Ravi
  Record 102: Anita
  Record 103: Karthik
  Record 104: Meena
Record 110 not found (all 4 record(s) were read)

Enter the record number to read (0 to stop): 0

Sequential file deleted.
```
