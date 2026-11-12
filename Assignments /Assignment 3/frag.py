import random, math
random.seed(42)
sizes=[random.randint(10*1024, 50*1024*1024) for _ in range(100)]   # 10 KB .. 50 MB
print("100 processes, sizes 10 KB - 50 MB, seed 42, total = %.1f MB" % (sum(sizes)/2**20))
print("%-8s %14s %16s %14s %14s" % ("Page","Total pages","Internal frag","Avg/process","Page-table(4B)"))
for name,ps in (("1 KB",1024),("4 KB",4096),("2 MB",2*2**20)):
    pages=sum(math.ceil(s/ps) for s in sizes)
    waste=sum((-s)%ps for s in sizes)
    print("%-8s %14d %13.2f MB %11.1f KB %11.1f KB" % (name,pages,waste/2**20,waste/100/1024,pages*4/1024))
print()
print("Page table size (single level, 4-byte entry for 32-bit, 8-byte entry for 48-bit), 4 KB pages")
e32=2**32//4096; e48=2**48//4096
print("32-bit: 2^20 = %d entries x 4 B = %d MB per process" % (e32, e32*4//2**20))
print("48-bit: 2^36 = %d entries x 8 B = %d GB per process" % (e48, e48*8//2**30))
print("48-bit 4-level (9 bits/level): minimum tables for 1 MB code+1 MB data+1 MB stack ~ 4 levels x 3 regions x 4 KB = 48 KB; one level-4 table (512 entries) covers 2 MB")
print()
print("Effective access time (memory 100 ns, TLB 10 ns, single-level page table)")
m,t=100,10
print("Without TLB: EAT = 2 x 100 = %d ns" % (2*m))
for h in (0.80,0.95,0.99):
    eat=h*(t+m)+(1-h)*(t+2*m)
    print("TLB hit ratio %2d%%: EAT = %.2f x 110 + %.2f x 210 = %.1f ns  (slow-down vs 100 ns: %.1f%%)" % (h*100,h,1-h,eat,(eat-100)))
print("4-level page table, 99%% hit: EAT = %.1f ns; no TLB = %d ns" % (0.99*110+0.01*(10+5*100), 5*m))
