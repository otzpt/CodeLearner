"""
TITLE: Uptime, boot time and load average
GROUP: System information
USES: import time, os, datetime
SUMMARY: time.clock_gettime(time.CLOCK_BOOTTIME) is seconds since boot, the
  same number as the first value in /proc/uptime. os.getloadavg() gives the
  1, 5 and 15 minute load.
NOTES:
  - CLOCK_BOOTTIME keeps counting while the machine is suspended;
    CLOCK_MONOTONIC does not. Use BOOTTIME for "up for".
  - Format the number yourself: days, hours, minutes. Print "1 day" and
    "2 days" correctly, and leave out zero parts.
  - boot time = now - uptime. /proc/stat also has it as "btime", in seconds
    since 1970.
  - Load average is the number of processes wanting a CPU, averaged. A value
    above the number of logical CPUs means the machine is busy.
  - time.clock_gettime and CLOCK_BOOTTIME are Linux (Python 3.7+).
SEE: pydoc time.clock_gettime, pydoc os.getloadavg
"""

import os
import time
from datetime import datetime, timedelta


def format_uptime(seconds):
    """'1 day, 2 hours, 3 mins': leave out zero days and hours, always show minutes."""
    days, rest = divmod(int(seconds), 86400)
    hours, rest = divmod(rest, 3600)
    minutes = rest // 60
    parts = []
    if days:
        parts.append(f"{days} day{'s' if days != 1 else ''}")
    if hours:
        parts.append(f"{hours} hour{'s' if hours != 1 else ''}")
    parts.append(f"{minutes} min{'s' if minutes != 1 else ''}")
    return ", ".join(parts)


boot_seconds = time.clock_gettime(time.CLOCK_BOOTTIME)
print("Uptime:", format_uptime(boot_seconds))

with open("/proc/uptime", encoding="utf-8") as file:
    print(f"/proc/uptime says {float(file.read().split()[0]):.2f} seconds")

print("Booted at", (datetime.now() - timedelta(seconds=boot_seconds)).strftime("%Y-%m-%d %H:%M"))
print("Load: %.2f %.2f %.2f" % os.getloadavg())
print("format_uptime(93780) =", format_uptime(93780))
