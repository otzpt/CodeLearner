#!/usr/bin/env python3
"""Run the AVR code the Arduino course quotes on a simulated ATmega328P.

check-avr.py proves the code COMPILES to the instructions the lessons show.
This runs it: avr-gcc builds execute.cpp (which includes snippets.cpp) for the
Uno's chip, simavr executes it, and this script talks to simavr's gdb server
to stop at finished() and read the results out of the chip's memory.

    python3 run-avr.py

Needs avr-gcc and simavr (Ubuntu: sudo apt install gcc-avr avr-libc simavr).
Uses simavr's fixed gdb port, 1234. Exits 0 when every result is what the
lessons claim.
"""

import os
import shutil
import socket
import subprocess
import sys
import tempfile
import time

HERE = os.path.dirname(os.path.abspath(__file__))
SRAM = 0x800000  # how avr gdb stubs address data memory


def send(sock, body):
    sock.sendall("${}#{:02x}".format(body, sum(body.encode()) & 0xFF).encode())


def receive(sock):
    data = b""
    while True:
        char = sock.recv(1)
        if not char:
            raise ConnectionError("simavr closed the connection")
        if char == b"$":
            data = b""
        elif char == b"#":
            sock.recv(2)  # checksum
            sock.sendall(b"+")
            return data.decode()
        elif char not in (b"+", b"-"):
            data += char


def main():
    for tool in ("avr-gcc", "avr-nm", "simavr"):
        if shutil.which(tool) is None:
            sys.exit("{} not found (Ubuntu: sudo apt install gcc-avr avr-libc simavr)".format(tool))

    with tempfile.TemporaryDirectory() as scratch:
        elf = os.path.join(scratch, "execute.elf")
        build = subprocess.run(
            ["avr-gcc", "-mmcu=atmega328p", "-Os", "-std=gnu++11", "-o", elf,
             os.path.join(HERE, "execute.cpp")], capture_output=True, text=True)
        if build.returncode != 0:
            sys.exit("build failed:\n" + build.stderr)

        symbols = {}
        for line in subprocess.check_output(["avr-nm", elf], text=True).splitlines():
            parts = line.split()
            if len(parts) == 3:
                symbols[parts[2]] = int(parts[0], 16)

        try:
            socket.create_connection(("127.0.0.1", 1234), timeout=1).close()
            sys.exit("something is already listening on port 1234 (an old simavr?)")
        except OSError:
            pass  # nothing there: the port is free

        simulator = subprocess.Popen(["simavr", "-m", "atmega328p", "-f", "16000000", "-g", elf],
                                     stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        try:
            sock = None
            for _ in range(50):
                try:
                    sock = socket.create_connection(("127.0.0.1", 1234), timeout=10)
                    break
                except OSError:
                    time.sleep(0.1)
            if sock is None:
                sys.exit("could not reach simavr's gdb server on port 1234")

            send(sock, "Z0,{:x},2".format(symbols["finished"]))
            if receive(sock) != "OK":
                sys.exit("simavr refused the breakpoint")
            send(sock, "c")
            stop = receive(sock)
            if not stop.startswith("T"):
                sys.exit("unexpected stop reply: " + stop)

            def read(name, size=1):
                address = symbols[name]
                address = address - SRAM if address >= SRAM else address
                send(sock, "m{:x},{:x}".format(SRAM + address, size))
                return int.from_bytes(bytes.fromhex(receive(sock)), "little")

            checks = [
                ("swap_asm(0xA5) is 0x5A", read("result_swap") == 0x5A),
                ("16-bit way: 1023 * 5000 / 1023 gives 3 (the wrap lesson 7 shows)", read("result_wrapped_16bit", 2) == 3),
                ("widened to 32 bits gives 5000", read("result_widened", 2) == 5000),
                ("interrupts are on before the critical section", read("result_read_sreg_interrupts_on") & 0x80 != 0),
                ("interrupts are OFF inside it", read("result_inside_critical") & 0x80 == 0),
                ("interrupts are back ON after it", read("result_after_critical") & 0x80 != 0),
                ("already off: still off inside it", read("result_inside_critical_when_off") & 0x80 == 0),
                ("already off: still off after it (sei() would have turned them on)", read("result_after_critical_when_off") & 0x80 == 0),
                ("PORTB |= (1 << 5) sets exactly bit 5", read("result_portb") == 0x20),
            ]
        finally:
            simulator.kill()
            simulator.wait()

    failed = 0
    for label, ok in checks:
        print("{}  {}".format("ok  " if ok else "FAIL", label))
        failed += 0 if ok else 1
    sys.exit(1 if failed else 0)


if __name__ == "__main__":
    main()
