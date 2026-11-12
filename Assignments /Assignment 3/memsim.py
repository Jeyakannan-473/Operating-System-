"""memsim.py - page-replacement simulation of the edge device (1 unit = 1 MB = 256 pages of 4 KB)
Physical memory 2048 MB, no swap. OS+kernel 256 MB pinned. RT task 128 MB pinned (mlock).
ML task: 1536 MB model mapped read-only from flash (file-backed => clean pages can be dropped and re-read).
Logging task: 64 MB buffers.  Optional 4th task: 500 MB.  Replacement policy for evictable pool = LRU."""
import random, collections, json
random.seed(3)
TOTAL=2048
def run(extra, pin_rt, rounds=6):
    rt=[("rt",i) for i in range(128)]
    ml=[("ml",i) for i in range(1536)]
    lg=[("lg",i) for i in range(64)]
    t4=[("t4",i) for i in range(extra)]
    pinned=set(("os",i) for i in range(256))
    if pin_rt: pinned|=set(rt)
    frames=TOTAL-len(pinned)
    cache=collections.OrderedDict()
    faults=collections.Counter(); refs=collections.Counter()
    def touch(pg):
        refs[pg[0]]+=1
        if pg in pinned: return
        if pg in cache: cache.move_to_end(pg); return
        faults[pg[0]]+=1
        if len(cache)>=frames: cache.popitem(last=False)
        cache[pg]=1
    for r in range(rounds):                   # one round = one ML inference + control cycles + logging
        for k,pg in enumerate(ml):            # inference scans the whole model sequentially
            touch(pg)
            if k%12==0:                       # RT control loop runs periodically, touches its state
                touch(random.choice(rt))
            if k%24==0: touch(random.choice(lg))
            if t4 and k%3==0: touch(random.choice(t4))
    return frames,faults,refs
res={}
for extra in (0,500):
    for pin in (True,False):
        fr,f,rf=run(extra,pin)
        res[f"extra{extra}_pin{int(pin)}"]={"frames":fr,"faults":dict(f),"refs":dict(rf)}
        print("4th task %3d MB | RT pinned=%-5s | evictable frames=%4d MB | faults: RT=%5d ML=%6d LOG=%4d T4=%5d | refs RT=%d"%(extra,pin,fr,f["rt"],f["ml"],f["lg"],f["t4"],rf["rt"]))
json.dump(res,open("memsim.json","w"))
print("\nMemory budget (MB): OS 256 + RT 128 + ML 1536 + LOG 64 =",256+128+1536+64,"of 2048 -> free",2048-(256+128+1536+64))
print("With 4th task 500 MB: demand =",256+128+1536+64+500,"of 2048 -> shortfall",256+128+1536+64+500-2048)
