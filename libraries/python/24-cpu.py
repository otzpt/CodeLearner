"""
TITLE: CPU model, core count and speed
GROUP: System information
USES: import os; read /proc/cpuinfo and /sys/devices/system/cpu
SUMMARY: The model name is a line in /proc/cpuinfo. os.cpu_count() gives the
  number of logical processors. The top speed is a file under
  /sys/devices/system/cpu.
NOTES:
  - /proc/cpuinfo repeats one block per logical processor. "model name" appears
    in every block; the first is enough.
  - "Logical" counts hyper-threads. For physical cores read "cpu cores" (per
    package) or count unique "core id" values.
  - os.cpu_count() is the machine's total; len(os.sched_getaffinity(0)) is how
    many this process may actually use (it respects containers and taskset).
  - On ARM the key is not "model name": cpuinfo lists "CPU part" numbers and
    "Hardware"/"Model" instead. Try several keys and fall back.
  - cpuinfo_max_freq is in kHz and is missing in many virtual machines.
  - "cpu MHz" in cpuinfo is the CURRENT speed and changes every second.
SEE: pydoc os.cpu_count, man 5 proc
"""

import os
from pathlib import Path


def cpuinfo_first(*keys):
    """The value after the colon in the first line starting with one of keys."""
    try:
        for line in Path("/proc/cpuinfo").read_text(encoding="utf-8", errors="replace").splitlines():
            key, _, value = line.partition(":")
            if key.strip() in keys:
                return value.strip()
    except OSError:
        pass
    return None


print("model          ", cpuinfo_first("model name", "Model", "Hardware") or "(not in /proc/cpuinfo)")
print("cpu cores      ", cpuinfo_first("cpu cores") or "(not reported)", "(per package)")
print("logical        ", os.cpu_count())
print("usable by us   ", len(os.sched_getaffinity(0)))

try:
    khz = int(Path("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq").read_text())
    print(f"max frequency   {khz / 1_000_000:.2f} GHz")
except (OSError, ValueError):
    print("max frequency   (not reported here)")
