names=["P1", "P2", "P3", "P4"]; arr=[0,1,2,3]; bur=[8,4,9,5]; N=4
Q=[2,4,10**9]; left=bur[:]; level=[0]*N; queues=[[],[],[]]; fin={}; first={}; t=0; g=[]; added=set()

def admit():
    for i in range(N):
        if i not in added and arr[i]<=t: queues[0].append(i); added.add(i)

admit(); cur=None
while len(fin)<N:
    admit()
    lv=next((k for k in range(3) if queues[k]), None)
    if lv is None: t+=1; continue
    p=queues[lv].pop(0); first.setdefault(p,t); start=t; run=0
    while run<Q[lv] and left[p]>0:
        t+=1; run+=1; left[p]-=1; admit()
        if lv>0 and queues[0]: break
        if lv==2 and queues[1]: break
    g.append((p,start,t))
    if left[p]==0: fin[p]=t
    elif run>=Q[lv]: level[p]=min(level[p]+1,2); queues[level[p]].append(p)
    else: queues[lv].insert(0,p)

print("Gantt: "+" ".join(f"{names[p]}({a}-{b})" for p,a,b in g))
print("Proc TAT WT RT (burst)")
tw={}
for i in range(N):
    tat=fin[i]-arr[i]; wt=tat-bur[i]; tw[i]=wt
    print(f"{names[i]:4} {tat:4} {wt:3} {first[i]-arr[i]:3} ({bur[i]})")
print(f"Average WT={sum(tw.values())/N:.2f}; short jobs (P2, P4) avg WT={(tw[1]+tw[3])/2:.2f}; long jobs (P1,P3) avg WT={(tw[0]+tw[2])/2:.2f}")
print("time slices=",len(g), "context switches=", len(g)-1)
