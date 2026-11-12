/* bank_dl.c - loads the encryption library only when needed (dlopen/dlsym) */
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
int main(int argc, char **argv) {
    const char *lib = argc > 1 ? argv[1] : "./libcrypto_demo.so";
    char msg[] = "ACCOUNT=1001;AMOUNT=2500";
    void *h = dlopen(lib, RTLD_LAZY);
    if (!h) { fprintf(stderr, "WARNING: %s\n  -> falling back to safe mode (no encryption)\n", dlerror()); return 2; }
    const char *(*ver)(void) = (const char *(*)(void))dlsym(h, "crypto_version");
    void (*enc)(char *, size_t) = (void (*)(char *, size_t))dlsym(h, "encrypt");
    if (!ver || !enc) { fprintf(stderr, "dlsym failed: %s\n", dlerror()); dlclose(h); return 3; }
    printf("Library : %s\n", ver());
    enc(msg, strlen(msg));
    printf("Cipher  : "); for (size_t i = 0; i < strlen(msg); i++) printf("%02x", (unsigned char)msg[i]); printf("\n");
    dlclose(h); return 0;
}
