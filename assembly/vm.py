#!/usr/bin/env python3
"""A small helper that boots a disk image in QEMU and reads the result back.

The OS modules (10 onward) claim things about what the machine does: what is
on the screen, what came out of the serial port, which registers hold what.
Those claims are checked by running the image and reading the answer out of
the emulator, not by assuming it. Used by check-os.py, and importable on its
own:

    from vm import Vm
    with Vm("os/boot1.bin") as vm:
        vm.wait_for_screen("Hello")
        print(vm.screen_text())

Needs qemu-system-x86_64. Talks to it through QMP on a loopback TCP port, so
nothing is written outside a temporary folder.
"""

import json
import os
import re
import shutil
import socket
import subprocess
import tempfile
import time


def qemu_available():
    return shutil.which("qemu-system-x86_64") is not None


def free_port():
    with socket.socket() as s:
        s.bind(("127.0.0.1", 0))
        return s.getsockname()[1]


class Vm:
    def __init__(self, image, drive_args=None, extra=None, serial=True):
        self.image = image
        self.tmp = tempfile.mkdtemp(prefix="asmcourse-")
        self.serial_path = os.path.join(self.tmp, "serial.txt")
        self.log_path = os.path.join(self.tmp, "qemu.log")
        port = free_port()
        args = ["qemu-system-x86_64", "-display", "none", "-no-reboot",
                "-qmp", "tcp:127.0.0.1:%d,server,nowait" % port]
        args += drive_args or ["-drive", "format=raw,file=" + image]
        if serial:
            args += ["-serial", "file:" + self.serial_path]
        args += extra or []
        self.proc = subprocess.Popen(args, stdout=subprocess.DEVNULL,
                                     stderr=open(self.log_path, "w"))
        self.sock = None
        for _ in range(100):
            try:
                self.sock = socket.create_connection(("127.0.0.1", port), timeout=5)
                break
            except OSError:
                time.sleep(0.05)
        if self.sock is None:
            raise RuntimeError("could not reach QEMU's monitor")
        self.buf = b""
        self._read_json()                       # the greeting
        self.qmp("qmp_capabilities")

    # --- plumbing -------------------------------------------------------
    def _read_json(self):
        while True:
            line, sep, rest = self.buf.partition(b"\n")
            if sep:
                self.buf = rest
                msg = json.loads(line)
                if "event" in msg:
                    continue
                return msg
            data = self.sock.recv(65536)
            if not data:
                raise RuntimeError("QEMU closed the monitor")
            self.buf += data

    def qmp(self, command, **arguments):
        self.sock.sendall((json.dumps({"execute": command, "arguments": arguments}) + "\n").encode())
        msg = self._read_json()
        if "error" in msg:
            raise RuntimeError(msg["error"]["desc"])
        return msg.get("return")

    def hmp(self, line):
        return self.qmp("human-monitor-command", **{"command-line": line})

    # --- reading the machine -------------------------------------------
    def mem(self, address, size):
        """size bytes of physical memory at address."""
        path = os.path.join(self.tmp, "mem.bin")
        self.qmp("pmemsave", val=address, size=size, filename=path)
        for _ in range(100):                     # pmemsave returns before the file is whole
            if os.path.exists(path) and os.path.getsize(path) == size:
                break
            time.sleep(0.01)
        with open(path, "rb") as f:
            data = f.read()
        os.remove(path)
        return data

    def screen_text(self, rows=25, cols=80):
        """The VGA text screen as a list of lines (characters only, no colour)."""
        raw = self.mem(0xB8000, rows * cols * 2)
        return [raw[r * cols * 2:(r + 1) * cols * 2:2].decode("latin1").rstrip() for r in range(rows)]

    def screen_attrs(self, row, col, count=1, cols=80):
        raw = self.mem(0xB8000 + (row * cols + col) * 2, count * 2)
        return list(raw[1::2])

    def serial_text(self):
        try:
            with open(self.serial_path, "rb") as f:
                return f.read().decode("latin1").replace("\r\n", "\n")
        except FileNotFoundError:
            return ""

    def registers(self):
        """`info registers` parsed to {name: int}."""
        text = self.hmp("info registers")
        regs = {}
        for name, value in re.findall(r"\b([A-Za-z0-9]+)\s*=\s*([0-9a-fA-F]{2,})", text):
            regs.setdefault(name, int(value, 16))
        return regs

    def sendkey(self, key):
        self.hmp("sendkey " + key)

    def log(self):
        try:
            with open(self.log_path) as f:
                return f.read()
        except FileNotFoundError:
            return ""

    # --- waiting --------------------------------------------------------
    def wait_for_screen(self, text, timeout=8.0):
        end = time.time() + timeout
        while time.time() < end:
            if any(text in line for line in self.screen_text()):
                return True
            time.sleep(0.1)
        return False

    def wait_for_serial(self, text, timeout=8.0):
        end = time.time() + timeout
        while time.time() < end:
            if text in self.serial_text():
                return True
            time.sleep(0.1)
        return False

    def wait_exit(self, timeout=8.0):
        """True if QEMU quit by itself within the timeout (e.g. the guest reset)."""
        try:
            self.proc.wait(timeout=timeout)
            return True
        except subprocess.TimeoutExpired:
            return False

    # --- cleanup --------------------------------------------------------
    def close(self):
        try:
            self.qmp("quit")
        except Exception:
            pass
        try:
            self.proc.wait(timeout=5)
        except subprocess.TimeoutExpired:
            self.proc.kill()
        shutil.rmtree(self.tmp, ignore_errors=True)

    def __enter__(self):
        return self

    def __exit__(self, *exc):
        self.close()
