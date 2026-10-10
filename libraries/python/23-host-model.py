"""
TITLE: Machine model (vendor and product name)
GROUP: System information
USES: pathlib.Path  (read /sys/devices/virtual/dmi/id, or /proc/device-tree/model)
SUMMARY: On PCs the firmware describes the machine in DMI tables, and the
  kernel exposes them as small text files. This is the "Host" line.
NOTES:
  - Readable by everyone: sys_vendor, product_name, product_version,
    board_vendor, board_name, bios_vendor, bios_version.
  - Root only: product_serial, product_uuid, board_serial. Do not print serial
    numbers in a tool people paste into forums.
  - Virtual machines and cheap boards put placeholders here, such as
    "To Be Filled By O.E.M.". Show or skip them, but expect them.
  - On ARM boards (Raspberry Pi) there is no DMI: read /proc/device-tree/model,
    a string ended by a zero byte.
  - Each file ends with a newline: strip it.
SEE: man 5 sysfs
"""

from pathlib import Path

DMI = Path("/sys/devices/virtual/dmi/id")


def read_small(path):
    try:
        return Path(path).read_text(encoding="utf-8", errors="replace").strip("\0\n ")
    except OSError:
        return ""


found = False
for name in ("sys_vendor", "product_name", "product_version", "board_vendor", "board_name", "bios_version"):
    value = read_small(DMI / name)
    if value:
        print(f"{name:<16} {value}")
        found = True

if not found:
    model = read_small("/proc/device-tree/model")
    print(f"device-tree model {model}" if model else "no machine model available here")
