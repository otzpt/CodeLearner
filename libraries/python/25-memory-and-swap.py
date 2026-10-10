"""
TITLE: Memory and swap (used, available, total)
GROUP: System information
USES: read /proc/meminfo; os.sysconf for the total
SUMMARY: /proc/meminfo lists memory in kB. "Used" is MemTotal minus
  MemAvailable, which is how free, htop and fastfetch count it.
NOTES:
  - Values are in kB but mean KiB (1024 bytes). 16384000 kB is 15.6 GiB.
  - MemFree is memory nothing uses at all. Linux fills spare memory with cache,
    so MemFree is small on a healthy machine. Use MemAvailable: the kernel's
    estimate of what a new program could get without swapping.
  - Swap: SwapTotal minus SwapFree is the swap in use. Both are 0 without swap.
  - os.sysconf("SC_PHYS_PAGES") * os.sysconf("SC_PAGE_SIZE") gives the total in
    bytes without parsing, but there is no "available" call in the standard
    library. The third-party psutil.virtual_memory() has one, and reads this
    same file.
  - Read the whole file into a dict; do not assume line order.
SEE: man 5 proc (search /proc/meminfo), pydoc os.sysconf
"""

import os
from pathlib import Path


def meminfo():
    """The /proc/meminfo keys as a dict of kB integers."""
    result = {}
    for line in Path("/proc/meminfo").read_text().splitlines():
        key, _, rest = line.partition(":")
        fields = rest.split()
        if fields and fields[0].isdigit():
            result[key] = int(fields[0])
    return result


def gib(kib):
    return kib / 1024 / 1024


info = meminfo()
total, available = info["MemTotal"], info["MemAvailable"]
print(f"Memory  {gib(total - available):.2f} GiB / {gib(total):.2f} GiB ({(total - available) * 100 // total}%)")
print(f"MemFree alone would say {gib(total - info['MemFree']):.2f} GiB used: do not use it")

if info.get("SwapTotal", 0) > 0:
    swap_used = info["SwapTotal"] - info["SwapFree"]
    print(f"Swap    {gib(swap_used):.2f} GiB / {gib(info['SwapTotal']):.2f} GiB")
else:
    print("Swap    none")

pages, page_size = os.sysconf("SC_PHYS_PAGES"), os.sysconf("SC_PAGE_SIZE")
print(f"sysconf total: {pages * page_size / 2**30:.2f} GiB")
