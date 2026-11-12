"""
TITLE: socket: low-level networking interface
GROUP: System information
LIBRARY: Python standard library (socket)
USES: import socket
SUMMARY: BSD sockets: connect to servers, accept connections, look up host
  names, list network interfaces. Most programs use higher-level
  modules (http, urllib), but this is what they are built on.
ENTRY: socket.gethostname()
   (no description in the module's docstring)
ENTRY: socket.gethostbyname(host) -> address
   (no description in the module's docstring)
ENTRY: socket.getaddrinfo(host, port, family=0, type=0, proto=0, flags=0)
   Resolve host and port into list of address info entries.
ENTRY: socket.socket(family=-1, type=-1, proto=-1, fileno=None)
   A subclass of _socket.socket adding the makefile() method.
ENTRY: socket.create_connection(address, timeout=<object object at
    0x7fbe847809a0>, source_address=None, *, all_errors=False)
   Connect to *address* and return the socket object.
ENTRY: socket.if_nameindex()
   (no description in the module's docstring)
ENTRY: socket.inet_aton(ip_addr, /)
   Convert an IP address in string format (123.45.67.89) to the
   32-bit packed binary format used in low-level network functions.
ENTRY: socket.inet_ntoa(packed_ip, /)
   Convert an IP address from 32-bit packed binary format to string
   format.
ENTRY: socket.AF_INET = <AddressFamily.AF_INET: 2>
   The IPv4 address family, for socket.socket().
ENTRY: socket.SOCK_STREAM = <SocketKind.SOCK_STREAM: 1>
   A stream (TCP) socket type, for socket.socket().
ENTRY: socket.SOCK_DGRAM = <SocketKind.SOCK_DGRAM: 2>
   A datagram (UDP) socket type, for socket.socket().
ENTRY: socket.timeout
   Timeout expired.
NOTES:
  - Always give network calls a timeout (settimeout, or timeout=
    in create_connection); the default is to wait forever.
  - send() may send less than you asked: use sendall().
  - A socket is a context manager: `with socket.socket(...) as s:`
    closes it for you.
  - The interface address lookup above uses fcntl.ioctl with Linux
    request numbers (SIOCGIFADDR 0x8915); the socket module itself
    has no portable way to list addresses.
  - Signatures and descriptions are Python's own, read from the
    module's docstrings for the version that runs this page.
    Version notes (added, changed, deprecated) are in the online
    documentation.
SEE: https://docs.python.org/3/library/socket.html, pydoc socket
"""

import fcntl
import os
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


print("gethostname() == os.uname().nodename:", socket.gethostname() == os.uname().nodename)
print("inet_aton / inet_ntoa:", socket.inet_aton("192.168.1.10"), socket.inet_ntoa(b"\xc0\xa8\x01\x0a"))
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
