"""
TITLE: time: time access and conversions
GROUP: Numbers and time
LIBRARY: Python standard library (time)
USES: import time
SUMMARY: Clocks and sleeping. time.time() is wall-clock seconds since the
  epoch and can jump; monotonic() and perf_counter() never go back
  and are for measuring durations.
ENTRY: time.time()
   number
ENTRY: time.monotonic()
   (no description in the module's docstring)
ENTRY: time.perf_counter()
   (no description in the module's docstring)
ENTRY: time.sleep(object, /)
   (no description in the module's docstring)
ENTRY: time.strftime(format[, time_tuple]) -> string
   (no description in the module's docstring)
ENTRY: time.localtime([seconds]) -> (tm_year,tm_mon,tm_mday,tm_hour,tm_min,
   tm_sec,tm_wday,tm_yday,tm_isdst)
ENTRY: time.gmtime([seconds]) -> (tm_year, tm_mon, tm_mday, tm_hour, tm_min,
   tm_mon, tm_mday, tm_hour, tm_min, tm_sec, tm_wday, tm_yday,
   tm_isdst)
ENTRY: time.mktime(object, /)
   number
ENTRY: time.clock_gettime(clk_id, /)
   Return the time of the specified clock clk_id as a float.
ENTRY: time.CLOCK_BOOTTIME = 7
   A clock id for time.clock_gettime(): seconds since boot, including
   time the machine was suspended. Linux.
ENTRY: time.CLOCK_MONOTONIC = 1
   A clock id for time.clock_gettime(): a clock that never goes
   backwards, from an unspecified start. It stops while the machine
   is suspended on Linux.
ENTRY: time.struct_time(iterable=(), /)
   The time value as returned by gmtime(), localtime(), and
   strptime(), and accepted by asctime(), mktime() and strftime().
   May be considered as a sequence of 9 integers.
NOTES:
  - Measure durations with perf_counter() or monotonic(), never
    time.time(): the wall clock can be adjusted.
  - CLOCK_BOOTTIME counts time spent suspended; CLOCK_MONOTONIC
    does not (Linux).
  - time.sleep() may sleep slightly longer than asked, and in 3.5+
    it is not cut short by signals.
  - Signatures and descriptions are Python's own, read from the
    module's docstrings for the version that runs this page.
    Version notes (added, changed, deprecated) are in the online
    documentation.
SEE: https://docs.python.org/3/library/time.html, pydoc time
"""

import time


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


print("epoch 0 in UTC:", time.strftime("%Y-%m-%d %H:%M:%S", time.gmtime(0)))
start = time.perf_counter()
time.sleep(0.05)
print("slept about 50 ms:", 0.05 <= time.perf_counter() - start < 0.5)
print("monotonic never decreases:", time.monotonic() <= time.monotonic())
seconds = time.clock_gettime(time.CLOCK_BOOTTIME)
print("uptime:", format_uptime(seconds), " | format_uptime(93780) =", format_uptime(93780))
print("localtime fields:", time.localtime().tm_year >= 2024, " struct_time has", len(time.localtime()), "fields")
