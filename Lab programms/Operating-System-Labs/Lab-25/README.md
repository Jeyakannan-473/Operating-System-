## Sample Input

```
No input required.
```

## Sample Output

```
~/Operating-System-/Lab-25$ ./io_system_calls
open()/write() : sample.txt created, 18 bytes written

fcntl(F_GETFL) : access mode = O_RDWR, O_APPEND = off
fcntl(F_SETFL) : O_APPEND turned on, O_APPEND = on

lseek(0, SEEK_SET)  : offset = 0
write() at offset 0 : 14 bytes written (O_APPEND adds them at the end)
lseek(0, SEEK_END)  : offset = 32 (file size)
lseek(10, SEEK_SET) : offset = 10, read() gave "Systems"

stat() : size = 32 bytes, permissions = 0644, links = 1, type = regular file

opendir()/readdir() : files in demo_dir (sorted by name):
  a.txt
  b.txt
  c.txt

Demo files removed.
```
