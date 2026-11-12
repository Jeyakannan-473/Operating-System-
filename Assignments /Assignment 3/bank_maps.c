/* bank_maps.c - same as bank.c but keeps running N seconds so /proc can be inspected */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "crypto.h"
int main(int argc, char **argv) {
    char msg[] = "HELLO"; encrypt(msg, 5);
    if (argc > 1) sleep(atoi(argv[1]));
    return 0;
}
