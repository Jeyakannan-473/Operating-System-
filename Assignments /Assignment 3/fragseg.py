import random
random.seed(7)
MEM=1024  # KB of physical memory, first-fit contiguous segments
segs=[random.randint(20,120) for _ in range(20)]
mem=[]  # list of (start,size,id)
def holes():
    h=[];pos=0
    for s,z,i in sorted(mem):
        if s>pos:h.append((pos,s-pos))
        pos=s+z
    if pos<MEM:h.append((pos,MEM-pos))
    return h
def alloc(i,z):
    for s,hz in holes():
        if hz>=z: mem.append((s,z,i)); return True
    return False
loaded=[]
for i,z in enumerate(segs):
    if alloc(i,z): loaded.append(i)
used=sum(z for _,z,_ in mem); print("Loaded %d/20 segments, sizes(KB)=%s"%(len(loaded),segs))
print("Memory used %d KB of %d KB (%.1f%%)"%(used,MEM,100*used/MEM))
removed=random.sample(loaded,10)
mem[:]=[m for m in mem if m[2] not in removed]
print("Removed 10 random segments:",sorted(removed))
h=holes(); free=sum(z for _,z in h); big=max(z for _,z in h)
print("Holes (start,size KB):",h)
print("Free = %d KB in %d holes, largest hole = %d KB"%(free,len(h),big))
print("External fragmentation = 1 - largest/free = %.1f%%"%(100*(1-big/free)))
need=big+1
print("A new segment of %d KB cannot be loaded although %d KB is free"%(need,free))
