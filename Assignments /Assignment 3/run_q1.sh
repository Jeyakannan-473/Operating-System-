#!/bin/bash
set -u
cd /home/claude/work/Q1
cat > crypto.c <<'EOF'
/* crypto.c - toy "encryption library" (XOR cipher, version 1) */
#include <stddef.h>
const char *crypto_version(void) { return "crypto v1 (XOR key 0x5A)"; }
void encrypt(char *buf, size_t n) { for (size_t i = 0; i < n; i++) buf[i] ^= 0x5A; }
EOF
sed 's/v1 (XOR key 0x5A)/v2 (XOR key 0x3C, security fix)/; s/0x5A/0x3C/' crypto.c > crypto_v2.c
printf '#include <stddef.h>\nconst char *crypto_version(void);\nvoid encrypt(char *buf, size_t n);\n' > crypto.h
cat > bank.c <<'EOF'
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
EOF
cat > bank_maps.c <<'EOF'
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
EOF
cat > bank_dl.c <<'EOF'
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
EOF
F="-std=c11 -Wall -Wextra -O2"
gcc $F -fPIC -shared -o libcrypto_demo.so crypto.c
gcc $F -fPIC -shared -o libcrypto_demo_v2.so crypto_v2.c
gcc $F -c crypto.c -o crypto.o && ar rcs libcrypto_demo.a crypto.o
gcc $F -static -o bank_static bank.c -L. -l:libcrypto_demo.a
gcc $F -o bank_dynamic bank.c -L. -lcrypto_demo -Wl,-rpath,'$ORIGIN'
gcc $F -o bank_maps bank_maps.c -L. -lcrypto_demo -Wl,-rpath,'$ORIGIN'
gcc $F -static -o bank_maps_static bank_maps.c -L. -l:libcrypto_demo.a
gcc $F -o bank_dl bank_dl.c -ldl
echo '=== Memory (/proc/PID/status) static vs dynamic'
for p in bank_maps_static bank_maps; do ./$p 3 & sleep 1; pid=$(pgrep -f "^./$p 3"); echo "== $p"; grep -E 'VmSize|VmRSS|RssAnon|RssFile' /proc/$pid/status; wait; done
echo; echo '=== Two copies running: smaps of r-xp mappings'
./bank_maps 4 & sleep 0.3; ./bank_maps 4 & sleep 1
for pid in $(pgrep -f "^./bank_maps 4"); do echo "-- pid $pid"; awk '/r-xp .*libcrypto_demo.so|r-xp .*libc.so.6/ {f=1;print "  "$6} f && /^(Rss|Pss|Shared_Clean|Private_Clean):/ {printf "     %-15s %s kB\n",$1,$2; if($1=="Private_Clean:")f=0}' /proc/$pid/smaps; done
wait
echo; echo '=== Replace library WITHOUT recompiling'
echo '$ ./bank_dynamic'; ./bank_dynamic
cp libcrypto_demo.so libcrypto_demo.so.bak; cp libcrypto_demo_v2.so libcrypto_demo.so
echo '$ cp libcrypto_demo_v2.so libcrypto_demo.so ; md5sum bank_dynamic ; ./bank_dynamic'; md5sum bank_dynamic; ./bank_dynamic
echo '$ ./bank_static  (still old: must be rebuilt)'; ./bank_static
echo; echo '=== Missing library'
mv libcrypto_demo.so libcrypto_demo.so.hide
echo '$ ./bank_dynamic'; ./bank_dynamic; echo "exit code $?"
echo '$ ./bank_dl ./libcrypto_demo.so'; ./bank_dl ./libcrypto_demo.so; echo "exit code $?"
mv libcrypto_demo.so.hide libcrypto_demo.so
echo '$ ./bank_dl /lib/x86_64-linux-gnu/libm.so.6   (library exists, symbol missing)'; ./bank_dl /lib/x86_64-linux-gnu/libm.so.6; echo "exit code $?"
cp libcrypto_demo.so.bak libcrypto_demo.so
echo; echo '=== ELF type'; readelf -h bank_dynamic | grep Type; readelf -h bank_static | grep Type
echo '=== PIC'; readelf -d libcrypto_demo.so | grep TEXTREL || echo 'PIC library: no TEXTREL entry'
