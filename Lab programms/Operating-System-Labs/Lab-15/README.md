## Sample Input

```
1
user1
1
user2
2
user1
a.txt
2
user1
b.txt
2
user2
a.txt
5
4
user2
a.txt
3
user1
a.txt
5
6
```

## Sample Output

```
~/Operating-System-/Lab-15$ ./two_level_directory

1.Create directory 2.Create file 3.Delete file 4.Search file 5.Display 6.Exit
Enter your choice: 1
Enter directory name: user1
Directory user1 created

1.Create directory 2.Create file 3.Delete file 4.Search file 5.Display 6.Exit
Enter your choice: 1
Enter directory name: user2
Directory user2 created

1.Create directory 2.Create file 3.Delete file 4.Search file 5.Display 6.Exit
Enter your choice: 2
Enter directory name: user1
Enter file name: a.txt
File a.txt created in user1

1.Create directory 2.Create file 3.Delete file 4.Search file 5.Display 6.Exit
Enter your choice: 2
Enter directory name: user1
Enter file name: b.txt
File b.txt created in user1

1.Create directory 2.Create file 3.Delete file 4.Search file 5.Display 6.Exit
Enter your choice: 2
Enter directory name: user2
Enter file name: a.txt
File a.txt created in user2

1.Create directory 2.Create file 3.Delete file 4.Search file 5.Display 6.Exit
Enter your choice: 5
Directory   Files
user1       a.txt, b.txt
user2       a.txt

1.Create directory 2.Create file 3.Delete file 4.Search file 5.Display 6.Exit
Enter your choice: 4
Enter directory name: user2
Enter file name: a.txt
File a.txt found in user2

1.Create directory 2.Create file 3.Delete file 4.Search file 5.Display 6.Exit
Enter your choice: 3
Enter directory name: user1
Enter file name: a.txt
File a.txt deleted from user1

1.Create directory 2.Create file 3.Delete file 4.Search file 5.Display 6.Exit
Enter your choice: 5
Directory   Files
user1       b.txt
user2       a.txt

1.Create directory 2.Create file 3.Delete file 4.Search file 5.Display 6.Exit
Enter your choice: 6
```
