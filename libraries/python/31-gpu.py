"""
TITLE: Graphics card (vendor, model, driver)
GROUP: System information
USES: pathlib.Path; read the card folders in /sys/class/drm and look the ids up in pci.ids
SUMMARY: Each GPU the kernel drives appears as /sys/class/drm/cardN. Its
  device/ folder has the PCI vendor and device ids as hex numbers, and a
  symlink to the kernel driver. The ids become names through the pci.ids
  database.
NOTES:
  - Only list cardN, not cardN-HDMI-A-1 (those are the connectors, see the
    monitors page): skip names containing a dash.
  - vendor 0x10de is NVIDIA, 0x1002 AMD, 0x8086 Intel. The file holds "0x" and
    hex digits with a newline.
  - The driver is the last part of the device/driver symlink: nvidia, amdgpu,
    i915, xe, nouveau, radeon.
  - pci.ids is a text file of "vendor  name" lines and, below each,
    tab-indented "device  name" lines. It is installed by pciutils-ids or
    hwdata (/usr/share/misc/pci.ids or /usr/share/hwdata/pci.ids). Fall back to
    the raw ids if neither exists.
  - A laptop shows two cards (integrated and discrete). device/boot_vga reads 1
    on the one the firmware used at boot.
  - The libraries that do this for you are libpci (pciutils) and libdrm; from
    Python, run `lspci -nn` with subprocess.
SEE: man 8 lspci, Documentation/gpu/drm-uapi.rst in the kernel
"""

from pathlib import Path


def hex4(text):
    """The leading four hex digits of text as an int, or None."""
    try:
        return int(text[:4], 16) if len(text) > 4 and text[4] == " " else None
    except ValueError:
        return None


def lookup_names(vendor, device):
    """(vendor name, device name) from pci.ids, empty strings if unknown."""
    for path in ("/usr/share/misc/pci.ids", "/usr/share/hwdata/pci.ids"):
        try:
            lines = Path(path).read_text(encoding="utf-8", errors="replace").splitlines()
        except OSError:
            continue
        vendor_name = device_name = ""
        in_vendor = False
        for line in lines:
            if not line or line.startswith("#"):
                continue
            if not line.startswith("\t"):                       # a vendor line
                if in_vendor:
                    break                                       # its block ended
                if hex4(line) == vendor:
                    vendor_name, in_vendor = line[6:], True
            elif in_vendor and not line.startswith("\t\t") and hex4(line[1:]) == device:
                device_name = line[7:]
                break
        if vendor_name:
            return vendor_name, device_name
    return "", ""


def read_id(path):
    try:
        return int(Path(path).read_text().strip(), 16)
    except (OSError, ValueError):
        return None


cards = sorted(p for p in Path("/sys/class/drm").glob("card[0-9]*") if "-" not in p.name)
if not cards:
    print("no graphics devices found")
for card in cards:
    vendor, device = read_id(card / "device/vendor"), read_id(card / "device/device")
    if vendor is None or device is None:
        continue
    vendor_name, device_name = lookup_names(vendor, device)
    driver = (card / "device/driver").resolve().name if (card / "device/driver").exists() else "(none)"
    print(f"{card.name}: {vendor_name or 'unknown vendor'} {device_name or 'unknown device'}")
    print(f"       ids 0x{vendor:04x}:0x{device:04x}, driver {driver}")
