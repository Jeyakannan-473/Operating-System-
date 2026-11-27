## Sample Input

```
Directory demo contains: a.c, data.csv, notes.txt, a sub-directory docs and a hidden file .hidden

demo
n

demo
y
```

## Sample Output

```
~/Operating-System-/Lab-27$ ./ls_simulation
Enter directory name: demo
Long listing like ls -l? (y/n): n

a.c  data.csv  docs  notes.txt
~/Operating-System-/Lab-27$ ./ls_simulation
Enter directory name: demo
Long listing like ls -l? (y/n): y

-rw-r--r--  1     13 a.c
-rw-r--r--  1     12 data.csv
drwxr-xr-x  2   4096 docs
-rw-r--r--  1      6 notes.txt
```
