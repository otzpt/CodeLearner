"""
TITLE: A complete mini fastfetch
GROUP: Putting it together
USES: everything on the System information and Terminal pages
SUMMARY: One program that gathers the fields, builds a "Label: value" list,
  and prints it next to a logo. Each field is a small function that returns a
  string, or None when the information does not exist on this machine; adding
  a field means adding one function and one entry in the FIELDS list.
NOTES:
  - A field that returns None is left out of the output. Never print "None" or
    a half-filled line.
  - Every read is wrapped so one failing source cannot stop the others: a
    broken field becomes None, not a traceback.
  - Colour comes from ANSI_COLOR in os-release, with a default.
  - Run it on its own to try it: python3 minifetch.py
  - fastfetch itself does the same job with many more sources, optional
    libraries (libpci, libdrm, wayland, xcb) for what sysfs cannot say, and a
    configuration file for which fields to show.
SEE: the pages above, and https://github.com/fastfetch-cli/fastfetch
"""

import getpass
import os
import platform
import shutil
import socket
import struct
import time
from itertools import zip_longest
from pathlib import Path


def read(path):
    try:
        return Path(path).read_text(encoding="utf-8", errors="replace").strip("\0\n ")
    except OSError:
        return ""


def key_value(path, key):
    """Value of the first 'key<spaces>: value' line in a /proc style file."""
    for line in read(path).splitlines():
        name, _, value = line.partition(":")
        if name.strip() == key:
            return value.strip()
    return None


def os_release():
    try:
        return platform.freedesktop_os_release()
    except OSError:
        return {}


# ---- fields -------------------------------------------------------------

def field_os():
    name = os_release().get("PRETTY_NAME")
    return f"{name} {os.uname().machine}" if name else None


def field_host():
    return read("/sys/devices/virtual/dmi/id/product_name") or read("/proc/device-tree/model") or None


def field_kernel():
    name = os.uname()
    return f"{name.sysname} {name.release}"


def field_uptime():
    days, rest = divmod(int(time.clock_gettime(time.CLOCK_BOOTTIME)), 86400)
    hours, rest = divmod(rest, 3600)
    minutes = rest // 60
    parts = []
    if days:
        parts.append(f"{days} day{'s' if days != 1 else ''}")
    if hours:
        parts.append(f"{hours} hour{'s' if hours != 1 else ''}")
    parts.append(f"{minutes} min{'s' if minutes != 1 else ''}")
    return ", ".join(parts)


def field_packages():
    try:
        with open("/var/lib/dpkg/status", encoding="utf-8", errors="replace") as file:
            return f"{sum(1 for line in file if line.rstrip() == 'Status: install ok installed')} (dpkg)"
    except OSError:
        return None


def field_shell():
    shell = os.environ.get("SHELL")
    return os.path.basename(shell) if shell else None


def field_desktop():
    desktop = os.environ.get("XDG_CURRENT_DESKTOP")
    return f"{desktop} ({os.environ.get('XDG_SESSION_TYPE', 'unknown session')})" if desktop else None


def field_display():
    modes = []
    for connector in sorted(Path("/sys/class/drm").glob("card*-*")):
        if read(connector / "status") == "connected":
            lines = read(connector / "modes").splitlines()
            if lines:
                modes.append(lines[0])
    return ", ".join(modes) or None


def field_cpu():
    model = key_value("/proc/cpuinfo", "model name") or key_value("/proc/cpuinfo", "Model")
    if not model:
        return None
    text = f"{model} ({os.cpu_count()})"
    khz = read("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq")
    return f"{text} @ {int(khz) / 1_000_000:.2f} GHz" if khz.isdigit() else text


def field_gpu():
    for card in sorted(Path("/sys/class/drm").glob("card[0-9]*")):
        if "-" in card.name:
            continue
        try:
            vendor = int(read(card / "device/vendor"), 16)
            device = int(read(card / "device/device"), 16)
        except ValueError:
            continue
        for path in ("/usr/share/misc/pci.ids", "/usr/share/hwdata/pci.ids"):
            in_vendor = False
            for line in read(path).splitlines():
                if not line or line.startswith("#"):
                    continue
                if not line.startswith("\t"):
                    if in_vendor:
                        break
                    in_vendor = line[:4].lower() == f"{vendor:04x}" and line[4:6] == "  "
                elif in_vendor and not line.startswith("\t\t") and line[1:5].lower() == f"{device:04x}":
                    return line[7:]
        return f"PCI {vendor:04x}:{device:04x}"
    return None


def meminfo():
    result = {}
    for line in read("/proc/meminfo").splitlines():
        key, _, rest = line.partition(":")
        if rest.split()[:1] and rest.split()[0].isdigit():
            result[key] = int(rest.split()[0])
    return result


def usage_text(used_kib, total_kib):
    return f"{used_kib / 2**20:.2f} GiB / {total_kib / 2**20:.2f} GiB ({used_kib * 100 // total_kib}%)"


def field_memory():
    info = meminfo()
    if info.get("MemTotal", 0) <= 0 or "MemAvailable" not in info:
        return None
    return usage_text(info["MemTotal"] - info["MemAvailable"], info["MemTotal"])


def field_swap():
    info = meminfo()
    if info.get("SwapTotal", 0) <= 0:
        return None
    return usage_text(info["SwapTotal"] - info["SwapFree"], info["SwapTotal"])


def field_disk():
    usage = shutil.disk_usage("/")
    shown_total = usage.used + usage.free
    return f"{usage.used / 2**30:.1f} GiB / {shown_total / 2**30:.1f} GiB ({usage.used / shown_total * 100:.0f}%) /"


def field_ip():
    import fcntl

    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:
        for _index, name in socket.if_nameindex():
            if name == "lo":
                continue
            try:
                packed = struct.pack("256s", name.encode()[:15])
                address = socket.inet_ntoa(fcntl.ioctl(sock.fileno(), 0x8915, packed)[20:24])
                mask = socket.inet_ntoa(fcntl.ioctl(sock.fileno(), 0x891B, packed)[20:24])
            except OSError:
                continue
            return f"{address}/{bin(int.from_bytes(socket.inet_aton(mask), 'big')).count('1')} ({name})"
    return None


FIELDS = [
    ("OS", field_os), ("Host", field_host), ("Kernel", field_kernel), ("Uptime", field_uptime),
    ("Packages", field_packages), ("Shell", field_shell), ("Desktop", field_desktop),
    ("Display", field_display), ("CPU", field_cpu), ("GPU", field_gpu), ("Memory", field_memory),
    ("Swap", field_swap), ("Disk", field_disk), ("Local IP", field_ip),
]

LOGO = [
    "     .-------.     ",
    "    /  o   o  \\    ",
    "   |     >     |   ",
    "   |   \\___/   |   ",
    "    \\         /    ",
    "     '-------'     ",
]


def main():
    accent = os_release().get("ANSI_COLOR", "1;36")
    title = f"{getpass.getuser()}@{socket.gethostname()}"
    lines = [f"\033[{accent}m{title}\033[0m", "-" * len(title)]

    for label, get in FIELDS:
        try:
            value = get()
        except Exception:                      # one broken source must not stop the rest
            value = None
        if value:
            lines.append(f"\033[{accent}m{label}\033[0m: {value}")

    width = max(len(row) for row in LOGO)
    for left, right in zip_longest(LOGO, lines, fillvalue=""):
        print(f"\033[{accent}m{left:<{width}}\033[0m {right}")


if __name__ == "__main__":
    main()
