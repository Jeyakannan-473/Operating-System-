jobs=[("FILM", 0,1200)]+[(f"T{i+1}",10*(i+1),2) for i in range(5)]
N=len(jobs); arr=[j[1] for j in jobs]; bur=[j[2] for j in jobs]

def sim(policy,q=3):
    left=bur[:]; fin={}; first={}; t=0; run_q=[]; cur=None; used=0; level=[0]*N; Q=[[],[],[]]
    qs=[2,8,10**9]; added=set()
    
    def admit():
        for i in range(N):
            if i not in added and arr[i]<=t:
                added.add(i); (Q[0] if policy=="MLFQ" else run_q).append(i)
    
    admit()
    while len(fin)<N:
        admit()
        if policy=="FCFS":
            if cur is None: cur=run_q.pop(0) if run_q else None
        elif policy=="SRTF":
            rd=[i for i in added if left[i]>0]; cur=min(rd,key=lambda i: (left[i],arr[i])) if rd else None
        elif policy=="RR":
            if cur is None and run_q: cur=run_q.pop(0); used=0
        elif policy=="MLFQ":
            lv=next((k for k in range(3) if Q[k]), None)
            if cur is not None and lv is not None and lv<level[cur]:
                Q[level[cur]].insert(0,cur); cur=None
            if cur is None and lv is not None:
                cur=Q[lv].pop(0); used=0
        
        if cur is None: t+=1; continue
        
        used+=1; t+=1; admit()
        first.setdefault(cur,t-1); left[cur]-=1
        
        if left[cur]==0: fin[cur]=t; cur=None
        elif policy=="RR" and used>=q: run_q.append(cur); cur=None
        elif policy=="MLFQ" and used>=qs[level[cur]]:
            level[cur]=min(level[cur]+1,2); Q[level[cur]].append(cur); cur=None
            
    wt=[fin[i]-arr[i]-bur[i] for i in range(N)]
    th=[wt[i] for i in range(1,N)]
    return sum(th)/len(th), max(th), fin[0], wt[0]

print(f"{'policy':26} {'thumbnail avg wait (s)':>22} {'thumbnail max wait (s)':>22} {'film finishes at (s)':>21}")
for name, pol,q in [("FCFS","FCFS",0), ("SRTF", "SRTF", 0), ("Round Robin q=3", "RR", 3), ("Round Robin q=20", "RR", 20), ("MLFQ (q=2, 8, FCFS)", "MLFQ",0)]:
    a, m, f, _ = sim(pol,q); print(f"{name:26} {a:22.1f} {m:22.0f} {f:21d}")
