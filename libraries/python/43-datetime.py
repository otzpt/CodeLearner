"""
TITLE: datetime: basic date and time types
GROUP: Numbers and time
LIBRARY: Python standard library (datetime)
USES: import datetime
SUMMARY: Dates, times, time zones and differences between them. A datetime
  without tzinfo is 'naive' and means local time by convention; use
  timezone-aware datetimes for anything stored or exchanged.
ENTRY: datetime.datetime(year, month, day[, hour[, minute[, second[,
    microsecond[,tzinfo]]]]])
   (no description in the module's docstring)
ENTRY: datetime.datetime.now(tz=None)
   Returns new datetime object representing current time local to tz.
ENTRY: datetime.datetime.fromtimestamp
   timestamp[, tz] -> tz's local time from POSIX timestamp.
ENTRY: datetime.datetime.strptime
   string, format -> new datetime parsed from a string (like
   time.strptime()).
ENTRY: datetime.datetime.strftime
   format -> strftime() style string.
ENTRY: datetime.datetime.isoformat
   [sep] -> string in ISO 8601 format, YYYY-MM-
   DDT[HH[:MM[:SS[.mmm[uuu]]]]][+HH:MM]. sep is used to separate the
   year from the time, and defaults to 'T'. The optional argument
   timespec specifies the number of additional terms of the time to
   include. Valid options are 'auto', 'hours', 'minutes', 'seconds',
   'milliseconds' and 'microseconds'.
ENTRY: datetime.date(year, month, day) --> date object
   --> date object
ENTRY: datetime.date.today()
   Current date or datetime: same as
   self.__class__.fromtimestamp(time.time()).
ENTRY: datetime.timedelta
   Difference between two datetime values.
ENTRY: datetime.timedelta.total_seconds(self, /)
   Total seconds in the duration.
ENTRY: datetime.timezone
   Fixed offset from UTC implementation of tzinfo.
ENTRY: datetime.timezone.utc
   Fixed offset from UTC implementation of tzinfo.
NOTES:
  - datetime.now() is naive (local time);
    datetime.now(timezone.utc) is aware. You cannot subtract one
    from the other.
  - datetime.utcnow() is deprecated since 3.12: use
    datetime.now(timezone.utc).
  - strftime directives: %Y %m %d %H %M %S, %A weekday, %B month,
    %z offset.
  - timedelta stores days, seconds and microseconds;
    .total_seconds() gives the whole thing as a float.
  - Signatures and descriptions are Python's own, read from the
    module's docstrings for the version that runs this page.
    Version notes (added, changed, deprecated) are in the online
    documentation.
SEE: https://docs.python.org/3/library/datetime.html, pydoc datetime
"""

from datetime import date, datetime, timedelta, timezone

print("epoch 0 is", datetime.fromtimestamp(0, timezone.utc).strftime("%Y-%m-%d %H:%M:%S"), "UTC")
now = datetime.now(timezone.utc)
print("aware now has tzinfo:", now.tzinfo, " this year >= 2024:", now.year >= 2024)
print("93780 s      =", timedelta(seconds=93780), " =", timedelta(seconds=93780).total_seconds(), "seconds")
print("isoformat    :", datetime(2024, 2, 29, 13, 5, tzinfo=timezone.utc).isoformat())
parsed = datetime.strptime("2024-03-01 09:30", "%Y-%m-%d %H:%M")
print("strptime     :", parsed, " + 1 day:", parsed + timedelta(days=1))
print("difference   :", datetime(2024, 3, 1) - datetime(2024, 2, 1))
print("date.today() is a date:", isinstance(date.today(), date))
try:
    datetime.strptime("1 March 2024", "%Y-%m-%d")
except ValueError as error:
    print("strptime with a wrong format raises ValueError:", error)
try:
    now - datetime.now()
except TypeError as error:
    print("aware minus naive raises TypeError:", error)
