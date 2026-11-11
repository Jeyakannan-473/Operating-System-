# CSA04 Operating Systems
Name: Jeyakannan L
Compiler: gcc 13.3.0
Assignment 2 (CO2)
Register number: 192525376
OS: Ubuntu 24.04 LTS (Linux 6.x), 1 virtual CPU
Compile: gcc -std=c11 -Wall -Wextra -pthread -o <name> <name>.c
(add -O1 for the benchmarks)

Task | Run | Result |
|---|---|---|
Q1 | ./q1_fork 3 & then ps -o pid,ppid,stat,wchan:14,cmd --ppid <pid>; ./q1_bench | 3 children in state S, fork 37 us, thread 7 us |
Q2 | ./q2_rr gcc -DQUANTUM=1... python3 algos.py | matches expected: avg TAT 20.00, avg WT 13.50 |
Q3 | ./q3_cameras 0 / ./q3_cameras 4000 / ./q3_race | speed-up 1.0 (CPU-bound, 1 CPU) / 2.3 (with I/O) |
Q4 | ./q4_pipe / ./q4_shm_readings / ./q4_shm | analyser: 6 readings, average 35.5, max 41 shm about 2x faster |
Q5 | ./q5_bb (x100); ./q5_multi 4 4 ./q5_counter ./q5_locks | sum 210 in 100/100 runs per-coach lock 13x faster |
Q6 | ./q6_banker / ./q6_bench | safe sequence P1 P3 P4 P0 P2 time n^2 |
