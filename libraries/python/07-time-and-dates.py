"""
TITLE: Time: now, dates, durations, clocks
GROUP: Basics
USES: import time, datetime
SUMMARY: time.time() is seconds since 1970. datetime gives dates you can
  format and subtract. time.clock_gettime reads the precise clocks, including
  the one that counts since boot.
NOTES:
  - datetime.now() is local time without a zone; datetime.now(timezone.utc) is
    aware. Subtracting two datetimes gives a timedelta.
  - time.monotonic() never goes backwards: use it to time things.
    time.time() can jump when the clock is set.
  - time.clock_gettime(time.CLOCK_BOOTTIME) is seconds since boot including
    suspend: the right clock for "up for".
  - strftime("%Y-%m-%d %H:%M") formats; strptime parses.
  - A duration of 93780 seconds is divmod(93780, 86400) -> 1 day, 7380 s.
SEE: pydoc datetime, pydoc time
"""

import time
from datetime import datetime, timedelta, timezone

print("epoch 0 is", datetime.fromtimestamp(0, timezone.utc).strftime("%Y-%m-%d %H:%M:%S"), "UTC")
print("this year :", datetime.now().year)

start = time.monotonic()
elapsed = time.monotonic() - start
print("two monotonic reads", "less" if elapsed < 0.001 else "more", "than a millisecond apart")

seconds = time.clock_gettime(time.CLOCK_BOOTTIME)
print(f"up for     {int(seconds // 86400)} days, {int(seconds % 86400 // 3600)} hours, {int(seconds % 3600 // 60)} minutes")

booted = datetime.now() - timedelta(seconds=seconds)
print("booted at ", booted.strftime("%Y-%m-%d %H:%M"))
print("93780 s    =", timedelta(seconds=93780))
