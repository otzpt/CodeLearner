"""
TITLE: Operating system name and version (os-release)
GROUP: System information
USES: import platform  (platform.freedesktop_os_release, Python 3.10+)
SUMMARY: platform.freedesktop_os_release() reads /etc/os-release (falling back
  to /usr/lib/os-release) and returns its KEY=value lines as a dict. This is
  where "Ubuntu 26.04 LTS" comes from.
NOTES:
  - Keys you will want: PRETTY_NAME (display name), NAME, ID (short, lower-case,
    like ubuntu), ID_LIKE (what it derives from), VERSION_ID, VERSION_CODENAME
    and ANSI_COLOR (the colour its logo uses).
  - It raises OSError if neither file exists (some containers, non-Linux).
  - Not every key is present on every distribution: use dict.get().
  - Before Python 3.10 there is no such function: parse the file yourself
    (the lines are KEY=value, optionally quoted).
  - platform.system() is the kernel name ("Linux"), not the distribution.
SEE: pydoc platform.freedesktop_os_release, man 5 os-release
"""

import platform

try:
    release = platform.freedesktop_os_release()
except OSError as error:
    print("no os-release file:", error)
else:
    for key in ("PRETTY_NAME", "NAME", "ID", "ID_LIKE", "VERSION_ID", "VERSION_CODENAME", "ANSI_COLOR"):
        print(f"{key:<17} {release.get(key, '(not set)')}")
