/* seg.c - segment table translation with base/limit check */
#include <stdio.h>
struct seg { const char *name; unsigned base, limit; };
struct seg table[] = { {"code", 1400, 1000}, {"data", 6300, 400}, {"heap", 4300, 400}, {"stack", 3200, 1100}, {"lib", 4700, 1000} };
int main(void) {
    unsigned q[][2] = { {0,53},{0,999},{0,1000},{1,0},{1,399},{1,400},{2,100},{3,1099},{3,1100},{4,500},{5,10},{4,1000} };
    for (int i = 0; i < 12; i++) {
        unsigned s = q[i][0], d = q[i][1];
        if (s >= sizeof table / sizeof table[0]) printf("(%u,%4u) -> TRAP: invalid segment number\n", s, d);
        else if (d >= table[s].limit) printf("(%u,%4u) -> TRAP: offset %u >= limit %u of %s (segmentation fault)\n", s, d, d, table[s].limit, table[s].name);
        else printf("(%u,%4u) -> %-5s physical = %u + %u = %u\n", s, d, table[s].name, table[s].base, d, table[s].base + d);
    }
    return 0;
}
