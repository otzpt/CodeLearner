"""
TITLE: Desktop environment, window manager and display server
GROUP: System information
USES: import os, socket, struct
SUMMARY: The desktop and session type are environment variables. The window
  manager (compositor on Wayland) is found by asking who owns the Wayland
  socket.
NOTES:
  - $XDG_CURRENT_DESKTOP is the desktop ("GNOME", "KDE", "ubuntu:GNOME": a
    colon-separated list, most specific first). $DESKTOP_SESSION is the session
    name, which login managers set.
  - $XDG_SESSION_TYPE is "wayland", "x11" or "tty". $WAYLAND_DISPLAY (often
    "wayland-0") is set under Wayland, $DISPLAY (":0") under X11 or XWayland.
  - Wayland: connect to $XDG_RUNTIME_DIR/$WAYLAND_DISPLAY and ask the kernel
    who is on the other end with SO_PEERCRED. That pid is the compositor;
    /proc/<pid>/comm names it (gnome-shell, kwin_wayland, sway, Hyprland).
  - X11: the window manager is named by a property on the root window
    (_NET_SUPPORTING_WM_CHECK, then _NET_WM_NAME). That needs Xlib or xcb
    (python-xlib is a pure-Python option).
  - Variables are missing over SSH and inside cron. Print "(not set)"; do not
    treat it as an error.
SEE: man 7 unix, man 7 credentials
"""

import os
import socket
import struct
from pathlib import Path


def wayland_compositor_pid():
    runtime = os.environ.get("XDG_RUNTIME_DIR")
    if not runtime:
        return None
    path = os.path.join(runtime, os.environ.get("WAYLAND_DISPLAY", "wayland-0"))
    with socket.socket(socket.AF_UNIX, socket.SOCK_STREAM) as sock:
        try:
            sock.connect(path)
            credentials = sock.getsockopt(socket.SOL_SOCKET, socket.SO_PEERCRED, struct.calcsize("3i"))
        except OSError:
            return None
    pid, _uid, _gid = struct.unpack("3i", credentials)
    return pid


for variable in ("XDG_CURRENT_DESKTOP", "DESKTOP_SESSION", "XDG_SESSION_TYPE", "WAYLAND_DISPLAY", "DISPLAY"):
    print(f"{variable:<21}", os.environ.get(variable, "(not set)"))

pid = wayland_compositor_pid()
if pid:
    print("Wayland compositor   ", Path(f"/proc/{pid}/comm").read_text().strip(), f"(pid {pid})")
else:
    print("Wayland compositor    (no Wayland socket reachable)")
