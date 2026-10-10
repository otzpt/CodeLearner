"""
TITLE: Network interfaces and local IP address
GROUP: System information
USES: import socket, fcntl, struct; read /proc/net/route
SUMMARY: The standard library has no getifaddrs. The classic way is to list
  the interface names with socket.if_nameindex() and ask the kernel for each
  one's IPv4 address with an ioctl.
NOTES:
  - SIOCGIFADDR (0x8915) returns an interface's IPv4 address, SIOCGIFNETMASK
    (0x891b) its mask. An interface with no IPv4 address raises OSError: skip it.
  - IPv6 is not covered by that ioctl. Read /proc/net/if_inet6, or run
    `ip -j addr` with subprocess: -j prints JSON that json.loads reads.
  - The interface the machine really uses for the internet is the one with the
    default route: the line of /proc/net/route whose Destination is 00000000.
  - A short trick to find the main IPv4 address: connect() a UDP socket to
    any outside address (nothing is sent) and read getsockname().
  - The third-party psutil.net_if_addrs() does all of this in one call, for
    every platform.
  - These are LOCAL addresses. A public IP needs an outside service.
SEE: man 7 netdevice, pydoc socket.if_nameindex
"""

import fcntl
import socket
import struct

SIOCGIFADDR, SIOCGIFNETMASK = 0x8915, 0x891B


def default_interface():
    try:
        with open("/proc/net/route", encoding="utf-8") as file:
            next(file)                                  # header line
            for line in file:
                fields = line.split()
                if len(fields) > 1 and int(fields[1], 16) == 0:
                    return fields[0]
    except OSError:
        pass
    return ""


def ipv4_of(sock, name, request):
    packed = struct.pack("256s", name.encode()[:15])
    return socket.inet_ntoa(fcntl.ioctl(sock.fileno(), request, packed)[20:24])


route = default_interface()
print("default route goes out through:", route or "(none)")

with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:
    for _index, name in socket.if_nameindex():
        if name == "lo":
            continue
        try:
            address = ipv4_of(sock, name, SIOCGIFADDR)
            mask = ipv4_of(sock, name, SIOCGIFNETMASK)
        except OSError:
            continue                                    # no IPv4 address on this one
        prefix = bin(int.from_bytes(socket.inet_aton(mask), "big")).count("1")
        print(f"{name:<12} IPv4 {address}/{prefix}" + ("  <- default route" if name == route else ""))
