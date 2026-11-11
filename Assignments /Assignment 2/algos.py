names=["P1", "P2", "P3", "P4"]; arr=[0,1,2,3]; bur=[8,4,9,5]; pri=[3,1,4,2]
N=4
def report(title, gantt, first, fin):
    print(f"\n== {title} ==")
    print("Gantt: "+" ".join(f"{names[p]}({a}-{b})" for p,a,b in gantt))
    tw=tt=tr=0
    print("Proc TAT WT RT")
    for i in range(N):
        tat=fin[i]-arr[i]; wt=tat-bur[i]; rt=first[i]-arr[i]
        tw+=wt; tt+=tat; tr+=rt
        print(f"{names[i]:4} {tat:4} {wt:3} {rt:3}")
    print(f"Average TAT={tt/N:.2f} WT={tw/N:.2f} RT={tr/N:.2f}")

def merge(g):
    out=[]
    for p,a,b in g:
        if out and out[-1][0]==p and out[-1][2]==a: out[-1]=(p,out[-1][1],b)
        else: out.append((p,a,b))
    return out

def nonpre(key,title):
    t=0; done=set(); g=[]; first={}; fin={}
    while len(done)<N:
        rd=[i for i in range(N) if i not in done and arr[i]<=t]
        if not rd: t=min(arr[i] for i in range(N) if i not in done); continue
        p=min(rd, key=key); first[p]=t; g.append((p,t,t+bur[p])); t+=bur[p]; fin[p]=t; done.add(p)
    report(title,g,first,fin)

def srtf():
    left=bur[:]; t=0; g=[]; first={}; fin={}
    while len(fin)<N:
        rd=[i for i in range(N) if left[i]>0 and arr[i]<=t]
        if not rd: t+=1; continue
        p=min(rd, key=lambda i: (left[i],arr[i])); first.setdefault(p,t)
        g.append((p,t,t+1)); left[p]-=1; t+=1
        if left[p]==0: fin[p]=t
    report("SRTF (preemptive SJF)", merge(g), first, fin)

def rr(q):
    left=bur[:]; t=0; Q=[0]; added={0}; g=[]; first={}; fin={}
    while len(fin)<N:
        p=Q.pop(0); first.setdefault(p,t); r=min(q,left[p]); g.append((p,t,t+r)); t+=r; left[p]-=r
        for i in range(N):
            if i not in added and arr[i]<=t: Q.append(i); added.add(i)
        if left[p]>0: Q.append(p)
        else: fin[p]=t
    report(f"Round Robin q={q}",g, first, fin); return len(g)

nonpre(lambda i: (arr[i],i), "FCFS")
nonpre(lambda i: (bur[i],arr[i]), "SJF (non-preemptive)")
srtf()
nonpre(lambda i: (pri[i],arr[i]), "Priority (non-preemptive; P1=3, P2=1, P3=4, P4=2)")
for q in (1,3,5,20): s=rr(q); print("slices:", s, "context switches:",s-1)
