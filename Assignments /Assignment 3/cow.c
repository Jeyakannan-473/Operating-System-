/* cow.c - copy-on-write demo: parent has 256 MB, fork, child writes to half of it */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/resource.h>
static double now(void){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);return t.tv_sec*1e3+t.tv_nsec/1e6;}
static void pss(const char*tag){FILE*f=fopen("/proc/self/smaps_rollup","r");char l[256];long rss=0,ps=0,sh=0,pv=0;
 while(fgets(l,256,f)){sscanf(l,"Rss: %ld",&rss);sscanf(l,"Pss: %ld",&ps);sscanf(l,"Shared_Dirty: %ld",&sh);sscanf(l,"Private_Dirty: %ld",&pv);}
 fclose(f);printf("%-34s Rss=%7ld kB  Pss=%7ld kB  Private_Dirty=%7ld kB\n",tag,rss/1,ps,pv);}
int main(void){
  size_t n=256UL<<20; char*p=malloc(n); memset(p,1,n);
  pss("parent after allocating 256 MB");
  fflush(stdout); double t0=now(); pid_t c=fork();
  if(c==0){ pss("child right after fork (shared)"); fflush(stdout);
    struct rusage r0,r1; getrusage(RUSAGE_SELF,&r0); double t1=now();
    for(size_t i=0;i<n/2;i+=4096) ((volatile char*)p)[i]=2; double t2=now(); getrusage(RUSAGE_SELF,&r1);
    printf("child wrote 128 MB: %.1f ms, COW page faults = %ld\n",t2-t1,r1.ru_minflt-r0.ru_minflt);
    pss("child after writing 128 MB"); fflush(stdout); _exit(0);}
  double t3=now(); waitpid(c,0,0);
  printf("fork() itself took %.3f ms for a 256 MB parent (no data copied)\n",t3-t0);
  pss("parent after child exit");
  return 0;}
