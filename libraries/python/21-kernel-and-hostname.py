"""
TITLE: Kernel, architecture and hostname
GROUP: System information
USES: import os, platform, socket
SUMMARY: os.uname() wraps the uname system call: kernel name, release,
  architecture and hostname in one named tuple. platform has the same pieces
  as separate functions.
NOTES:
  - release is what people mean by "kernel version": 6.17.0-5-generic.
  - machine is the architecture: x86_64, aarch64, riscv64.
  - nodename is the hostname. socket.gethostname() returns the same name.
  - os.uname exists only on Unix; platform.uname() works everywhere but reads
    more slowly the first time.
  - platform.processor() is often empty on Linux: read /proc/cpuinfo for the
    CPU model (see the CPU page).
SEE: pydoc os.uname, pydoc platform, man 2 uname
"""

import os
import platform
import socket

name = os.uname()
print("sysname  ", name.sysname)
print("release  ", name.release)
print("machine  ", name.machine)
print("nodename ", name.nodename)
print("socket.gethostname() is the same:", socket.gethostname() == name.nodename)
print("platform.system()/release()/machine():", platform.system(), platform.release(), platform.machine())
