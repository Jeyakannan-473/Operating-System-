/* bank.c - branch application that links the crypto library at start-up */
#include <stdio.h>
#include <string.h>
#include "crypto.h"
int main(void) {
    char msg[] = "ACCOUNT=1001;AMOUNT=2500";
    printf("Library : %s\n", crypto_version());
    encrypt(msg, strlen(msg));
    printf("Cipher  : ");
    for (size_t i = 0; i < strlen(msg); i++) printf("%02x", (unsigned char)msg[i]);
    printf("\n");
    return 0;
}
