## Sample Input

```
demo.txt
640
```

## Sample Output

```
~/Operating-System-/Lab-40$ ./file_permissions
Enter file name: demo.txt
File demo.txt created

Current permissions:
File        : demo.txt
Permissions : -rw-r--r-- (octal 644)
Owner       : root
Group       : root

User type Read    Write   Execute
Owner     yes     yes     no
Group     yes     no      no
Others    yes     no      no

Enter new permissions in octal (for example 640): 640

Updated permissions:
File        : demo.txt
Permissions : -rw-r----- (octal 640)
Owner       : root
Group       : root

User type Read    Write   Execute
Owner     yes     yes     no
Group     yes     no      no
Others    no      no      no

File demo.txt deleted
```
