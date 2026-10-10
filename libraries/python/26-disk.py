"""
TITLE: Disk usage and mounted filesystems
GROUP: System information
USES: import shutil, os; read /proc/self/mounts
SUMMARY: shutil.disk_usage(path) returns total, used and free bytes of the
  filesystem holding a path. /proc/self/mounts lists what is mounted where.
NOTES:
  - disk_usage wraps statvfs. Its "free" is what an ordinary user can use
    (f_bavail), and "used" is total minus the free blocks INCLUDING the part
    reserved for root, so used + free is a little less than total. That is the
    same convention as df.
  - os.statvfs(path) gives the raw fields (f_blocks, f_bfree, f_bavail, f_frsize).
  - /proc/self/mounts has virtual filesystems too (proc, tmpfs, cgroup).
    Filter on the type (ext4, btrfs, xfs, vfat) to list real disks.
  - Snap and loop mounts show up as squashfs: usually skipped.
  - A network or removable drive can block for a while: do not call this in a
    tight loop.
SEE: pydoc shutil.disk_usage, man 3 statvfs
"""

import shutil

usage = shutil.disk_usage("/")
shown_total = usage.used + usage.free
print(f"/   {usage.used / 2**30:.1f} GiB / {shown_total / 2**30:.1f} GiB ({usage.used / shown_total * 100:.0f}%)")
print(f"raw total {usage.total / 2**30:.1f} GiB; reserved for root {(usage.total - shown_total) / 2**30:.1f} GiB")

REAL = {"ext4", "btrfs", "xfs", "vfat", "ntfs3", "f2fs"}
print("real filesystems:")
with open("/proc/self/mounts", encoding="utf-8") as mounts:
    for line in mounts:
        fields = line.split()
        if len(fields) >= 3 and fields[2] in REAL:
            print(f"  {fields[0]:<22} {fields[1]:<18} {fields[2]}")
