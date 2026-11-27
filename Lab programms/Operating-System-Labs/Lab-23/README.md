## Sample Input

```
1
1
200
1
2
100
1
3
300
1
4
100
1
5
150
2
1
2
3
3
1
6
120
3
4
```

## Sample Output

```
~/Operating-System-/Lab-23$ ./first_fit

1.Allocate 2.Release 3.Display 4.Exit - Enter your choice: 1
Enter process id: 1
Enter size: 200
Process P1 allocated memory 0-199

1.Allocate 2.Release 3.Display 4.Exit - Enter your choice: 1
Enter process id: 2
Enter size: 100
Process P2 allocated memory 200-299

1.Allocate 2.Release 3.Display 4.Exit - Enter your choice: 1
Enter process id: 3
Enter size: 300
Process P3 allocated memory 300-599

1.Allocate 2.Release 3.Display 4.Exit - Enter your choice: 1
Enter process id: 4
Enter size: 100
Process P4 allocated memory 600-699

1.Allocate 2.Release 3.Display 4.Exit - Enter your choice: 1
Enter process id: 5
Enter size: 150
Process P5 allocated memory 700-849

1.Allocate 2.Release 3.Display 4.Exit - Enter your choice: 2
Enter process id: 1
Process P1 released its memory

1.Allocate 2.Release 3.Display 4.Exit - Enter your choice: 2
Enter process id: 3
Process P3 released its memory

1.Allocate 2.Release 3.Display 4.Exit - Enter your choice: 3
Memory map:
   0-199  Free    (200)
 200-299  P2      (100)
 300-599  Free    (300)
 600-699  P4      (100)
 700-849  P5      (150)
 850-999  Free    (150)

1.Allocate 2.Release 3.Display 4.Exit - Enter your choice: 1
Enter process id: 6
Enter size: 120
Process P6 allocated memory 0-119

1.Allocate 2.Release 3.Display 4.Exit - Enter your choice: 3
Memory map:
   0-119  P6      (120)
 120-199  Free    (80)
 200-299  P2      (100)
 300-599  Free    (300)
 600-699  P4      (100)
 700-849  P5      (150)
 850-999  Free    (150)

1.Allocate 2.Release 3.Display 4.Exit - Enter your choice: 4
```
