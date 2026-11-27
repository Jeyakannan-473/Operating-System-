## Sample Input

```
No input required.
```

## Sample Output

```
~/Operating-System-/Lab-30$ ./thread_concepts
Main: thread 1 created
Main: thread 2 created
Main: pthread_equal(thread 1, thread 1) = equal
Main: pthread_equal(thread 1, thread 2) = not equal
Thread 1: pthread_equal(self, main thread) = not equal
Thread 1: calling pthread_exit with value 10
Main: thread 1 joined, exit value = 10
Thread 2: pthread_equal(self, main thread) = not equal
Thread 2: calling pthread_exit with value 20
Main: thread 2 joined, exit value = 20
Main: all threads have finished
```
