# CSA04 – Operating Systems – Assignment 3 (CO3)
Name: <YOUR NAME>   Register number: <YOUR REGISTER NUMBER>
Compiler: gcc 13.3.0   OS: Ubuntu 24.04 LTS (replace with your own if you re-run)

Compile:  gcc -std=c11 -Wall -Wextra -O2 -pthread file.c -o file
(gcc -std=gnu11 for clock_gettime programs; add -ldl for dlopen)

| Task | Files | Run |
|------|-------|-----|
| Q1 | crypto.c bank.c bank_dl.c bank_maps.c run_q1.sh | bash Q1/run_q1.sh |
| Q2 | page.c page2048.c page_edge.c pf.c frag.py | gcc page.c -o page && ./page ; python3 frag.py |
| Q3 | seg.c segv.c sleeper.c fragseg.py | ./seg ; ./segv ; python3 fragseg.py |
| Q4 | memtouch.c cow.c | ./memtouch 256 ; ./cow |
| Q5 | lru_complete.c sim.c | ./lru_complete ; ./sim |
| Q6 | commands.sh | bash commands.sh |
| Q7 | rt_loop.c memsim.py | ./rt_loop 0 ; ./rt_loop 1 ; python3 memsim.py |

Re-run every program on YOUR machine, replace the outputs/ files and add screenshots.
Result notes: see the report (CSA04_OS_Assignment3_CO3_Report).
