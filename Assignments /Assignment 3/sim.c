/* sim.c - FIFO, LRU, Optimal, Clock simulator on locality / random strings; prints CSV + timings */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define N 100000
static volatile int sink;
#define PAGES 50
static int ref[N], nxt[N], lastpos[PAGES];
static int fnd(const int *f, int n, int p){ for(int i=0;i<n;i++) if(f[i]==p) return i; return -1; }
int fifo(int fr){int f[64],u=0,nx=0,fl=0;for(int i=0;i<N;i++){if(fnd(f,u,ref[i])>=0)continue;fl++;if(u<fr)f[u++]=ref[i];else{f[nx]=ref[i];nx=(nx+1)%fr;}}return fl;}
int lru(int fr){int f[64],l[64],u=0,fl=0;for(int i=0;i<N;i++){int k=fnd(f,u,ref[i]);if(k>=0){l[k]=i;continue;}fl++;if(u<fr){f[u]=ref[i];l[u++]=i;}else{int v=0;for(int j=1;j<fr;j++)if(l[j]<l[v])v=j;f[v]=ref[i];l[v]=i;}}return fl;}
int opt(int fr){int f[64],nu[64],u=0,fl=0;for(int i=0;i<N;i++){int k=fnd(f,u,ref[i]);if(k>=0){nu[k]=nxt[i];continue;}fl++;if(u<fr){f[u]=ref[i];nu[u++]=nxt[i];}else{int v=0;for(int j=1;j<fr;j++)if(nu[j]>nu[v])v=j;f[v]=ref[i];nu[v]=nxt[i];}}return fl;}
int clk(int fr){int f[64],b[64],u=0,h=0,fl=0;for(int i=0;i<N;i++){int k=fnd(f,u,ref[i]);if(k>=0){b[k]=1;continue;}fl++;if(u<fr){f[u]=ref[i];b[u++]=1;}else{while(b[h]){b[h]=0;h=(h+1)%fr;}f[h]=ref[i];b[h]=1;h=(h+1)%fr;}}return fl;}
static void prep(void){for(int p=0;p<PAGES;p++)lastpos[p]=N;for(int i=N-1;i>=0;i--){nxt[i]=lastpos[ref[i]];lastpos[ref[i]]=i;}}
static double now(void){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);return t.tv_sec*1e3+t.tv_nsec/1e6;}
int main(void){
  srand(12345);
  for(int mode=0;mode<2;mode++){
    if(mode==0){ /* strong locality: phases of 400 refs, working set of 6 pages, 95% inside set */
      int ws[6]; for(int i=0;i<N;i++){ if(i%400==0) for(int k=0;k<6;k++) ws[k]=rand()%PAGES; ref[i]=(rand()%100<95)?ws[rand()%6]:rand()%PAGES; }
    } else for(int i=0;i<N;i++) ref[i]=rand()%PAGES;
    prep();
    const char *nm = mode?"random":"locality";
    for(int fr=3;fr<=10;fr++) printf("CSV,%s,%d,%d,%d,%d,%d\n",nm,fr,fifo(fr),lru(fr),opt(fr),clk(fr));
    double t0=now(); for(int r=0;r<5;r++) sink+=fifo(6); double t1=now(); for(int r=0;r<5;r++) sink+=lru(6); double t2=now(); for(int r=0;r<5;r++) sink+=opt(6); double t3=now(); for(int r=0;r<5;r++) sink+=clk(6); double t4=now();
    printf("TIME,%s,%.3f,%.3f,%.3f,%.3f\n",nm,(t1-t0)/5,(t2-t1)/5,(t3-t2)/5,(t4-t3)/5);
  }
  return 0;}
