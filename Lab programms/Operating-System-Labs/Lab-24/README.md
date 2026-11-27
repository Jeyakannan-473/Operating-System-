## Sample Input

```
sample.txt
Hello, UNIX system calls!
```

## Sample Output

```
~/Operating-System-/Lab-24$ ./file_system_calls
Enter file name: sample.txt
Enter text to write: Hello, UNIX system calls!

creat()  : file sample.txt created, file descriptor = 3
open()   : sample.txt opened for writing, file descriptor = 3
write()  : 26 bytes written
close()  : file closed
open()   : sample.txt opened for reading, file descriptor = 3
read()   : 26 bytes read
Data read from the file: Hello, UNIX system calls!
unlink() : file sample.txt deleted
```
