## Sample Input

```
1
file1.txt
1
file2.txt
1
notes.txt
1
file1.txt
4
3
file2.txt
3
report.doc
2
file1.txt
4
5
```

## Sample Output

```
~/Operating-System-/Lab-14$ ./single_level_directory

1.Create 2.Delete 3.Search 4.List 5.Exit - Enter your choice: 1
Enter file name: file1.txt
File file1.txt created

1.Create 2.Delete 3.Search 4.List 5.Exit - Enter your choice: 1
Enter file name: file2.txt
File file2.txt created

1.Create 2.Delete 3.Search 4.List 5.Exit - Enter your choice: 1
Enter file name: notes.txt
File notes.txt created

1.Create 2.Delete 3.Search 4.List 5.Exit - Enter your choice: 1
Enter file name: file1.txt
File file1.txt already exists

1.Create 2.Delete 3.Search 4.List 5.Exit - Enter your choice: 4
Files in the directory:
1. file1.txt
2. file2.txt
3. notes.txt

1.Create 2.Delete 3.Search 4.List 5.Exit - Enter your choice: 3
Enter file name: file2.txt
File file2.txt found

1.Create 2.Delete 3.Search 4.List 5.Exit - Enter your choice: 3
Enter file name: report.doc
File report.doc not found

1.Create 2.Delete 3.Search 4.List 5.Exit - Enter your choice: 2
Enter file name: file1.txt
File file1.txt deleted

1.Create 2.Delete 3.Search 4.List 5.Exit - Enter your choice: 4
Files in the directory:
1. file2.txt
2. notes.txt

1.Create 2.Delete 3.Search 4.List 5.Exit - Enter your choice: 5
```
