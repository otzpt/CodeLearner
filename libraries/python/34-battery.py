"""
TITLE: Battery level and charging state
GROUP: System information
USES: pathlib.Path; read the folders in /sys/class/power_supply
SUMMARY: Each power source is a folder in /sys/class/power_supply. A battery
  has type "Battery", a capacity from 0 to 100, and a status such as
  Charging, Discharging, Full or Not charging.
NOTES:
  - Desktops have no battery: the folder is empty, or holds only mains adapters
    (type "Mains"). That is the normal case, not an error.
  - Names vary: BAT0, BAT1, macsmc-battery. Look at the type file instead of
    guessing the name.
  - Some batteries report energy_now / energy_full (or charge_*) instead of
    capacity. Compute 100 * now / full.
  - A laptop with two batteries has two entries. Show both or add them up.
  - The third-party psutil.sensors_battery() reads the same files.
SEE: Documentation/ABI/testing/sysfs-class-power in the kernel
"""

from pathlib import Path


def read(path):
    try:
        return Path(path).read_text().strip()
    except OSError:
        return ""


found = False
for supply in sorted(Path("/sys/class/power_supply").glob("*")):
    if read(supply / "type") != "Battery":
        continue
    print(f"{supply.name}: {read(supply / 'capacity') or '?'}% ({read(supply / 'status') or 'unknown'})")
    found = True

if not found:
    print("no battery on this machine")
