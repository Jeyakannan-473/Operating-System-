#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int g_init = 7; int g_bss[1000]; const char *ro = "read-only string";
int main(void) { int local = 1; char *h = malloc(1<<20); h[0] = local; g_bss[0] = g_init; sleep(3); return 0; }
