## Sample Input

```
text.txt:
Operating System manages hardware
UNIX is a multiuser operating system
Linux is a UNIX like system
Windows is a proprietary system
unix commands: ls, grep, cat

Command-line arguments:
UNIX text.txt
-i -n unix text.txt
Android text.txt
```

## Sample Output

```
~/Operating-System-/Lab-28$ cat text.txt
Operating System manages hardware
UNIX is a multiuser operating system
Linux is a UNIX like system
Windows is a proprietary system
unix commands: ls, grep, cat
~/Operating-System-/Lab-28$ ./grep_simulation UNIX text.txt
UNIX is a multiuser operating system
Linux is a UNIX like system
~/Operating-System-/Lab-28$ ./grep_simulation -i -n unix text.txt
2:UNIX is a multiuser operating system
3:Linux is a UNIX like system
5:unix commands: ls, grep, cat
~/Operating-System-/Lab-28$ ./grep_simulation Android text.txt; echo "Exit status: $?"
Exit status: 1
```
