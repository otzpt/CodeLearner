"""
TITLE: Monitors and resolution
GROUP: System information
USES: pathlib.Path; read the connector folders in /sys/class/drm (files status, modes, enabled)
SUMMARY: Every video output of a GPU is a "connector" folder named like
  card1-HDMI-A-1. status says connected or not, and modes lists the
  resolutions the monitor offers, best first.
NOTES:
  - modes is a list of "WIDTHxHEIGHT" lines, the monitor's preferred mode
    first. It is what the monitor SUPPORTS, not necessarily what is in use.
  - The mode actually in use, and the refresh rate, belong to the display
    server: ask the Wayland compositor (wl_output events, via libwayland-client),
    X11 (XRandR, via python-xlib or xcb), or the kernel directly (libdrm).
    Running `xrandr` or `wlr-randr` with subprocess is the quick way.
  - enabled says whether the connector is currently driving a monitor.
  - Connector names tell the cable: HDMI-A, DP (DisplayPort), eDP (a laptop's
    own panel), DVI-D, VGA.
  - A machine over SSH or in a container has no connectors in /sys/class/drm.
SEE: Documentation/gpu/drm-kms.rst in the kernel
"""

from pathlib import Path


def first_line(path):
    try:
        lines = Path(path).read_text().splitlines()
        return lines[0] if lines else ""
    except OSError:
        return ""


connected = 0
for connector in sorted(Path("/sys/class/drm").glob("card*-*")):
    if first_line(connector / "status") != "connected":
        continue
    name = connector.name.split("-", 1)[1]
    print(f"{name:<18} preferred mode {first_line(connector / 'modes') or 'unknown':<10} ({first_line(connector / 'enabled') or '?'})")
    connected += 1

if not connected:
    print("no connected monitors found")
