/* crypto.c - toy "encryption library" (XOR cipher, version 1) */
#include <stddef.h>
const char *crypto_version(void) { return "crypto v1 (XOR key 0x5A)"; }
void encrypt(char *buf, size_t n) { for (size_t i = 0; i < n; i++) buf[i] ^= 0x5A; }
