/* rt_loop.c - time-critical loop; each iteration touches a page of a work buffer.
   mode 0: no locking (pages first-touched inside the loop => page faults)
   mode 1: mlockall(MCL_CURRENT|MCL_FUTURE) + pre-fault the buffer BEFORE the loop            */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/resource.h>
#define ITER 50000
static long long ns(void){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);return t.tv_sec*1000000000LL+t.tv_nsec;}
static int cmp(const void*a,const void*b){long long x=*(long long*)a,y=*(long long*)b;return x<y?-1:x>y;}
int main(int argc,char**argv){
  int lock=argc>1&&atoi(argv[1]); size_t bytes=(size_t)ITER*4096;
  char*buf=mmap(0,bytes,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
  if(lock){ if(mlockall(MCL_CURRENT|MCL_FUTURE)) perror("mlockall"); for(size_t i=0;i<bytes;i+=4096) buf[i]=0; }
  static long long lat[ITER]; struct rusage a,b; getrusage(RUSAGE_SELF,&a);
  for(int i=0;i<ITER;i++){ long long t0=ns(); buf[(size_t)i*4096]=(char)i; volatile int x=0; for(int k=0;k<200;k++) x+=k; lat[i]=ns()-t0; }
  getrusage(RUSAGE_SELF,&b);
  long long mx=0,sum=0; for(int i=0;i<ITER;i++){sum+=lat[i]; if(lat[i]>mx)mx=lat[i];}
  qsort(lat,ITER,sizeof(long long),cmp);
  printf("%-10s page faults in loop=%6ld | mean=%6lld ns | median=%6lld ns | p99.9=%7lld ns | WORST=%8lld ns (%.1f us)\n",lock?"mlockall":"no lock",b.ru_minflt-a.ru_minflt,sum/ITER,lat[ITER/2],lat[(int)(ITER*0.999)],mx,mx/1000.0);
  return 0;}
