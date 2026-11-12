#!/bin/bash
lsblk; cat /proc/partitions; swapon --show; free -h; df -h /
sudo fdisk -l          # run on your own machine and keep a screenshot
dd if=/dev/zero of=/tmp/test.bin bs=1M count=1024 conv=fsync
sync; echo 3 | sudo tee /proc/sys/vm/drop_caches; dd if=/tmp/test.bin of=/dev/null bs=1M
