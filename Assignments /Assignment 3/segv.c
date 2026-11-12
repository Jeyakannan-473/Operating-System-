#include <stdio.h>
int main(void) { int *p = (int *)0; printf("about to write to address 0...\n"); fflush(stdout); *p = 42; printf("not reached\n"); return 0; }
