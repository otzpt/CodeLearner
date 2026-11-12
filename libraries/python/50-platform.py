"""
TITLE: platform: identifying the platform
GROUP: System information
LIBRARY: Python standard library (platform)
USES: import platform
SUMMARY: The operating system, release, machine type and Python build.
  freedesktop_os_release() reads /etc/os-release for the
  distribution.
ENTRY: platform.system()
   Returns the system/OS name, e.g. 'Linux', 'Windows' or 'Java'.
ENTRY: platform.release()
   Returns the system's release, e.g. '2.2.0' or 'NT'
ENTRY: platform.version()
   Returns the system's release version, e.g. '#3 on degas'
ENTRY: platform.machine()
   Returns the machine type, e.g. 'i386'
ENTRY: platform.node()
   Returns the computer's network name (which may not be fully
   qualified)
ENTRY: platform.processor()
   Returns the (true) processor name, e.g. 'amdk6'
ENTRY: platform.python_version()
   Returns the Python version as string 'major.minor.patchlevel'
ENTRY: platform.uname()
   Fairly portable uname interface. Returns a tuple of strings
   (system, node, release, version, machine, processor) identifying
   the underlying platform.
ENTRY: platform.platform(aliased=False, terse=False)
   Returns a single string identifying the underlying platform with
   as much useful information as possible (but no more :).
ENTRY: platform.freedesktop_os_release()
   Return operation system identification from freedesktop.org os-
   release
NOTES:
  - platform.system() is 'Linux', 'Darwin' or 'Windows'.
    sys.platform is the lowercase form.
  - freedesktop_os_release() needs Python 3.10 or later and raises
    OSError if neither /etc/os-release nor /usr/lib/os-release
    exists.
  - Test features, not names: write `if hasattr(os, 'uname')`
    rather than `if platform.system() == 'Linux'` where you can.
  - Signatures and descriptions are Python's own, read from the
    module's docstrings for the version that runs this page.
    Version notes (added, changed, deprecated) are in the online
    documentation.
SEE: https://docs.python.org/3/library/platform.html, pydoc platform, man os-release
"""

import platform

print("system / release / machine:", platform.system(), platform.release(), platform.machine())
print("node:", platform.node(), " python:", platform.python_version())
try:
    release = platform.freedesktop_os_release()
except OSError as error:
    print("no os-release file:", error)
else:
    for key in ("PRETTY_NAME", "NAME", "ID", "ID_LIKE", "VERSION_ID", "ANSI_COLOR"):
        print(f"{key:<12} {release.get(key, '(not set)')}")
