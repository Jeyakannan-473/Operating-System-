/* memtouch.c - touch N MB sequentially then randomly; report page faults and time */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/resource.h>
static double now(void){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);return t.tv_sec*1e3+t.tv_nsec/1e6;}
static long flt(void){struct rusage r;getrusage(RUSAGE_SELF,&r);return r.ru_minflt+r.ru_majflt;}
int main(int argc,char**argv){
  size_t mb=argc>1?atoi(argv[1]):256, bytes=mb<<20, pg=4096, np=bytes/pg;
  char *a,*b; a=mmap(0,bytes,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
  b=mmap(0,bytes,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
  long f0=flt(); double t0=now();
  for(size_t i=0;i<np;i++) a[i*pg]=1;                      /* sequential first touch */
  double t1=now(); long f1=flt();
  srand(1); size_t *ord=malloc(np*sizeof *ord); for(size_t i=0;i<np;i++)ord[i]=i;
  for(size_t i=np-1;i>0;i--){size_t j=((size_t)rand()*32768+rand())%(i+1);size_t t=ord[i];ord[i]=ord[j];ord[j]=t;}
  long f2=flt(); double t2=now();
  for(size_t i=0;i<np;i++) b[ord[i]*pg]=1;                 /* random first touch */
  double t3=now(); long f3=flt();
  volatile long s=0; double t4=now(); for(size_t i=0;i<np;i++) s+=a[i*pg]; double t5=now();      /* resident, sequential */
  for(size_t i=0;i<np;i++) s+=a[ord[i]*pg]; double t6=now(); long f6=flt();                      /* resident, random */
  printf("%5zu MB (%zu pages): seq-first-touch faults=%ld (%.1f ms) | rand-first-touch faults=%ld (%.1f ms) | re-read seq %.1f ms, rand %.1f ms, extra faults=%ld\n",
     mb,np,f1-f0,t1-t0,f3-f2,t3-t2,t5-t4,t6-t5,f6-f3);
  return 0;}
