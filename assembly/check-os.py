#!/usr/bin/env python3
"""Boot every disk image the operating-system modules (10 onward) show, and
check what the modules say the machine does.

Each module's text quotes things the emulator printed: what is on the screen,
what came out of the serial port, which registers hold what. This builds the
files in os/ the way the module says, boots them in QEMU, reads the answer
back, and fails if it differs from the claim.

    python3 check-os.py [module numbers...]

Needs nasm and qemu-system-x86_64 (and gcc, ld for the kernel modules). With
no qemu it says so and exits 0: the checks that need an emulator are skipped,
not passed.
"""

import os
import shutil
import subprocess
import sys
import tempfile
import time

HERE = os.path.dirname(os.path.abspath(__file__))
OS = os.path.join(HERE, "os")
sys.path.insert(0, HERE)

from vm import Vm, qemu_available   # noqa: E402

CHECKS = []


def check(module, name):
    def register(fn):
        CHECKS.append((module, name, fn))
        return fn
    return register


def nasm_bin(source, out):
    """Assemble a flat binary. Returns its bytes."""
    done = subprocess.run(["nasm", "-f", "bin", "-I", OS + "/", source, "-o", out],
                          capture_output=True, text=True)
    if done.returncode:
        raise AssertionError("nasm failed: " + done.stderr.strip())
    with open(out, "rb") as f:
        return f.read()


def screen_has(vm, text, timeout=8.0):
    return vm.wait_for_screen(text, timeout)


# --------------------------------------------------------------- module 10

@check(10, "boot1.asm prints its line, in real mode, inside its 512 bytes")
def _(work):
    image = nasm_bin(os.path.join(OS, "boot1.asm"), work + "/boot1.bin")
    assert len(image) == 512, "boot1.bin is %d bytes" % len(image)
    assert image[510:512] == b"\x55\xaa", "no boot signature"
    with Vm(work + "/boot1.bin") as vm:
        assert screen_has(vm, "Hello from a boot sector"), "message not on screen"
        screen = vm.screen_text()
        assert any("Booting from Hard Disk" in l for l in screen), "no SeaBIOS boot line"
        assert any(l.startswith("SeaBIOS") for l in screen), "no SeaBIOS banner"
        time.sleep(0.5)
        r = vm.registers()
        assert r["CS"] == 0, "CS = %#x" % r["CS"]
        assert 0x7C00 <= r["EIP"] < 0x7E00, "EIP = %#x" % r["EIP"]
        assert r["CR0"] & 1 == 0, "CR0 = %#x: protected mode is on" % r["CR0"]


@check(10, "without org the message is not printed")
def _(work):
    nasm_bin(os.path.join(OS, "traps", "boot1_noorg.asm"), work + "/noorg.bin")
    with Vm(work + "/noorg.bin") as vm:
        assert screen_has(vm, "Booting from Hard Disk"), "did not reach the boot line"
        time.sleep(1.5)
        screen = vm.screen_text()
        assert not any("Hello" in l for l in screen), "the message printed after all"
        assert not any("Boot failed" in l for l in screen), "the BIOS rejected the sector"
        junk = [l for l in screen[screen.index(next(l for l in screen if "Booting from Hard Disk" in l)) + 1:] if l.strip()]
        assert junk, "nothing at all was printed"


@check(10, "without the signature the BIOS reports 'not a bootable disk'")
def _(work):
    nasm_bin(os.path.join(OS, "traps", "boot1_nosig.asm"), work + "/nosig.bin")
    with Vm(work + "/nosig.bin") as vm:
        assert screen_has(vm, "Boot failed: not a bootable disk"), "message not on screen"


@check(10, "a boot sector that is too big is refused by NASM")
def _(work):
    with open(os.path.join(OS, "boot1.asm")) as f:
        source = f.read().replace("message: db", "pad: times 520 db 0x90\nmessage: db")
    path = work + "/big.asm"
    with open(path, "w") as f:
        f.write(source)
    done = subprocess.run(["nasm", "-f", "bin", path, "-o", work + "/big.bin"], capture_output=True, text=True)
    assert done.returncode != 0 and "TIMES value -69 is negative" in done.stderr, done.stderr


@check(10, "16-bit code reaches a 32-bit register with a 0x66 prefix")
def _(work):
    with open(work + "/b16.asm", "w") as f:
        f.write("bits 16\nmov ax, 1\nmov eax, 1\n")
    data = nasm_bin(work + "/b16.asm", work + "/b16.bin")
    assert data.hex().upper() == "B8010066B801000000", data.hex()


# --------------------------------------------------------------- module 11

def _boot2(work, name="boot2.bin", source=None):
    path = source or os.path.join(OS, "boot2.asm")
    image = nasm_bin(path, work + "/" + name)
    assert len(image) == 512 and image[510:512] == b"\x55\xaa", "not a bootable 512-byte image"
    return work + "/" + name


@check(11, "boot2.asm: serial greeting, cleared screen, banner with attribute 0x1F")
def _(work):
    with Vm(_boot2(work)) as vm:
        assert vm.wait_for_serial("boot2 ready"), "no greeting on the serial port"
        deadline = time.time() + 5
        while time.time() < deadline and "Type something:" not in vm.screen_text()[1]:
            time.sleep(0.1)
        screen = vm.screen_text()
        assert screen[0].strip() == "", "row 0 is not empty: %r" % screen[0]
        assert screen[1][2:17] == "Type something:" and screen[1][:2].strip() == "", repr(screen[1])
        attrs = vm.screen_attrs(1, 2, 15)
        assert attrs == [0x1F] * 15, [hex(a) for a in attrs]


@check(11, "boot2.asm: keys h, i, a give 23, 17, 1E on the serial port and hia on row 3")
def _(work):
    with Vm(_boot2(work)) as vm:
        assert vm.wait_for_serial("boot2 ready"), "no greeting"
        time.sleep(0.5)
        for key in "hia":
            vm.sendkey(key)
            time.sleep(0.4)
        assert vm.wait_for_serial("a:1E"), repr(vm.serial_text())
        serial = vm.serial_text().split("\n")
        assert serial[1:4] == ["h:23", "i:17", "a:1E"], serial
        screen = vm.screen_text()
        assert screen[3][2:5] == "hia", repr(screen[3])
        assert vm.screen_attrs(3, 2, 3) == [0x0A] * 3


@check(11, "attribute 0: the text is in video memory and every cell has attribute 0")
def _(work):
    with Vm(_boot2(work, "noattr.bin", os.path.join(OS, "traps", "boot2_noattr.asm"))) as vm:
        assert vm.wait_for_serial("boot2 ready"), "no greeting"
        time.sleep(0.8)
        assert vm.screen_text()[1][2:17] == "Type something:", "text not in memory"
        assert vm.screen_attrs(1, 2, 15) == [0] * 15, "attributes are not all 0"


@check(11, "mov ax, 0xB8000 is a NASM warning, not an error")
def _(work):
    with open(work + "/seg.asm", "w") as f:
        f.write("bits 16\n    mov ax, 0xB8000\n    mov es, ax\n")
    done = subprocess.run(["nasm", "-f", "bin", work + "/seg.asm", "-o", work + "/seg.bin"],
                          capture_output=True, text=True)
    assert done.returncode == 0, "NASM refused it"
    assert "warning: word exceeds bounds [-w+number-overflow]" in done.stderr, done.stderr


@check(11, "es left at 0: nothing on the screen, the text lands at physical 0xA4")
def _(work):
    with open(os.path.join(OS, "boot2.asm")) as f:
        source = f.read()
    assert "    mov ax, 0xB800              ; video memory" in source
    source = source.replace("    mov ax, 0xB800              ; video memory is the segment 0xB800",
                            "    xor ax, ax                  ; (wrong: es = 0)")
    path = work + "/noes.asm"
    with open(path, "w") as f:
        f.write(source)
    with Vm(_boot2(work, "noes.bin", path)) as vm:
        assert vm.wait_for_serial("boot2 ready"), "no greeting"
        time.sleep(0.8)
        assert not any("Type something" in l for l in vm.screen_text()), "text reached the screen"
        low = vm.mem(0xA4, 30)
        assert low[0::2][:15] == b"Type something:", low


# --------------------------------------------------------------- module 12

def _os12(work, stage1="stage1.asm", stage2="stage2.asm", name="os12.img"):
    s1 = nasm_bin(os.path.join(OS, stage1), work + "/s1.bin")
    s2 = nasm_bin(os.path.join(OS, stage2), work + "/s2.bin")
    assert len(s1) == 512 and s1[510:512] == b"\x55\xaa", "stage 1 is not a boot sector"
    assert len(s2) == 1024, "stage 2 is %d bytes, stage 1 reads 1024" % len(s2)
    with open(work + "/" + name, "wb") as f:
        f.write(s1 + s2)
    return work + "/" + name


E820_QEMU = [
    "base 0x0000000000000000  length 0x000000000009FC00  type 1",
    "base 0x000000000009FC00  length 0x0000000000000400  type 2",
    "base 0x00000000000F0000  length 0x0000000000010000  type 2",
    "base 0x0000000000100000  length 0x0000000007EE0000  type 1",
    "base 0x0000000007FE0000  length 0x0000000000020000  type 2",
    "base 0x00000000FFFC0000  length 0x0000000000040000  type 2",
    "base 0x000000FD00000000  length 0x0000000300000000  type 2",
]


@check(12, "two stages: stage 2 runs, gets drive 0x80, lists the memory map the lesson quotes")
def _(work):
    with Vm(_os12(work)) as vm:
        assert vm.wait_for_serial("usable bytes"), "stage 2 never finished: " + repr(vm.serial_text())
        lines = [l.strip() for l in vm.serial_text().split("\n")]
        assert lines[0] == "stage 2 loaded" and lines[1] == "boot drive: 0x80", lines[:2]
        for entry in E820_QEMU:
            assert entry in lines, "the lesson quotes %r, the machine printed %r" % (entry, lines)
        total = 0
        for l in lines:
            if l.startswith("base 0x") and l.endswith("type 1"):
                total += int(l.split("length 0x")[1].split()[0], 16)
        assert "usable bytes (low 32 bits): 0x%08X" % total in lines, (hex(total), lines[-2:])
        assert "A20 usable before enabling: yes" in lines and "A20 usable after enabling:  yes" in lines, lines


@check(12, "A20 forced off first: the test says no, then yes after port 0x92")
def _(work):
    img = _os12(work, stage2="traps/stage2_a20off.asm")
    with Vm(img) as vm:
        assert vm.wait_for_serial("usable bytes"), "no output"
        lines = [l.strip() for l in vm.serial_text().split("\n")]
        assert "A20 usable before enabling: no" in lines and "A20 usable after enabling:  yes" in lines, lines


@check(12, "setting bit 0 of port 0x92 resets the machine (QEMU -no-reboot quits)")
def _(work):
    img = _os12(work, stage2="traps/stage2_reset.asm")
    with Vm(img) as vm:
        assert vm.wait_exit(10), "QEMU is still running"


@check(12, "stage 1 with the wrong drive number prints 'Disk read error'")
def _(work):
    s1 = nasm_bin(os.path.join(OS, "traps", "stage1_wrongdrive.asm"), work + "/s1.bin")
    s2 = nasm_bin(os.path.join(OS, "stage2.asm"), work + "/s2.bin")
    with open(work + "/wd.img", "wb") as f:
        f.write(s1 + s2)
    with Vm(work + "/wd.img") as vm:
        assert vm.wait_for_screen("Disk read error"), vm.screen_text()
        assert "stage 2 loaded" not in vm.serial_text(), "stage 2 ran anyway"


# --------------------------------------------------------------- module 13

def _pm(work, source="pm.asm", name="pm.bin", extra=None):
    path = source if os.path.isabs(source) else os.path.join(OS, source)
    nasm_bin(path, work + "/" + name)
    return Vm(work + "/" + name, extra=extra)


@check(13, "pm.asm: protected mode, flat segments, the text on screen, the register lines the lesson quotes")
def _(work):
    with _pm(work) as vm:
        assert vm.wait_for_screen("Now in 32-bit protected mode"), "message not on screen"
        time.sleep(0.5)
        assert vm.screen_text()[0].startswith("Now in 32-bit protected mode")
        assert vm.screen_attrs(0, 0, 28) == [0x2F] * 28
        dump = [l.rstrip() for l in vm.hmp("info registers").splitlines()]
        for line in ("CS =0008 00000000 ffffffff 00cf9a00 DPL=0 CS32 [-R-]",
                     "DS =0010 00000000 ffffffff 00cf9300 DPL=0 DS   [-WA]",
                     "GDT=     00007c58 00000017"):
            assert line in dump, "the lesson quotes %r; the machine shows %r" % (line, [l for l in dump if l[:3] == line[:3]])
        assert "CR0=00000011" in dump[-0:] or any(l.startswith("CR0=00000011") for l in dump), "CR0"


@check(13, "pm.asm: the descriptor constants decode to what the lesson says")
def _(work):
    d = 0x00CF9A000000FFFF
    limit = (d & 0xFFFF) | (((d >> 48) & 0xF) << 16)
    base = ((d >> 16) & 0xFFFFFF) | (((d >> 56) & 0xFF) << 24)
    assert (limit, base, (d >> 40) & 0xFF, (d >> 52) & 0xF) == (0xFFFFF, 0, 0x9A, 0xC)


def _triple_fault_log(work, source, name):
    log = work + "/" + name + ".log"
    with _pm(work, source, name + ".bin", extra=["-d", "int,cpu_reset", "-D", log]) as vm:
        assert vm.wait_exit(10), "the machine did not reset"
    with open(log) as f:
        return [l.strip() for l in f if "check_exception" in l or "Triple fault" in l]


@check(13, "a data selector in the far jump ends in a triple fault, with the log the lesson quotes")
def _(work):
    lines = _triple_fault_log(work, "traps/pm_wrongsel.asm", "wrongsel")
    assert lines[:4] == ["check_exception old: 0xffffffff new 0xd", "check_exception old: 0xd new 0xd",
                         "check_exception old: 0x8 new 0xd", "Triple fault"], lines[:5]


@check(13, "a BIOS call from protected mode ends in the same triple fault")
def _(work):
    lines = _triple_fault_log(work, "traps/pm_bios.asm", "bios")
    assert lines[:4] == ["check_exception old: 0xffffffff new 0xd", "check_exception old: 0xd new 0xd",
                         "check_exception old: 0x8 new 0xd", "Triple fault"], lines[:5]


@check(13, "a near jump instead of the far jump hangs with cs still 0")
def _(work):
    with _pm(work, "traps/pm_nearjump.asm", "near.bin") as vm:
        time.sleep(2.5)
        assert not vm.proc.poll(), "the machine reset or quit"
        regs = vm.registers()
        assert regs["CS"] == 0 and regs["CR0"] & 1 == 1, (hex(regs["CS"]), hex(regs["CR0"]))
        assert not any("protected" in l for l in vm.screen_text()), "the message printed"
        assert vm.screen_text()[0].startswith("SeaBIOS"), "the banner was overwritten"


# --------------------------------------------------------------- module 14

@check(14, "lm.asm: long mode, the register lines and the text the lesson quotes")
def _(work):
    with _pm(work, "lm.asm", "lm.bin") as vm:
        assert vm.wait_for_screen("Now in 64-bit long mode"), "message not on screen"
        time.sleep(0.5)
        assert vm.screen_text()[0].startswith("Now in 64-bit long mode")
        assert vm.screen_attrs(0, 0, 23) == [0x1F] * 23
        dump = [l.rstrip() for l in vm.hmp("info registers").splitlines()]
        for line in ("CS =0018 0000000000000000 ffffffff 00af9a00 DPL=0 CS64 [-R-]",
                     "EFER=0000000000000500"):
            assert line in dump, "the lesson quotes %r" % line
        assert any(l.startswith("CR0=80000011 CR2=0000000000000000 CR3=0000000000001000 CR4=00000020") for l in dump), \
            [l for l in dump if l.startswith("CR0")]


@check(14, "a CPU without long mode: the CPUID test writes NO64")
def _(work):
    with _pm(work, "lm.asm", "lm.bin", extra=["-cpu", "qemu32"]) as vm:
        deadline = time.time() + 6
        while time.time() < deadline and not vm.screen_text()[0].startswith("NO64"):
            time.sleep(0.1)
        assert vm.screen_text()[0].startswith("NO64"), repr(vm.screen_text()[0])
        assert vm.screen_attrs(0, 0, 4) == [0x4F] * 4


@check(14, "an unmapped code page ends in a page-fault triple fault, with the log the lesson quotes")
def _(work):
    lines = _triple_fault_log(work, "traps/lm_nomap.asm", "nomap")
    assert lines[:4] == ["check_exception old: 0xffffffff new 0xe", "check_exception old: 0xe new 0xe",
                         "check_exception old: 0x8 new 0xe", "Triple fault"], lines[:5]


@check(14, "PAE left off: QEMU ignores the cr0 write (CR0 0x11, EFER 0x100, no message)")
def _(work):
    with _pm(work, "traps/lm_nopae.asm", "nopae.bin") as vm:
        time.sleep(2.5)
        assert not vm.proc.poll(), "QEMU quit or reset"
        dump = [l.rstrip() for l in vm.hmp("info registers").splitlines()]
        assert any(l.startswith("CR0=00000011") for l in dump), [l for l in dump if l.startswith("CR0")]
        assert "EFER=0000000000000100" in dump, [l for l in dump if l.startswith("EFER")]
        assert not any("long mode" in l for l in vm.screen_text())


@check(14, "the challenge solution without its second page faults")
def _(work):
    with open(os.path.join(HERE, "solutions", "14.asm")) as f:
        source = f.read()
    assert "    mov dword [PD + 8], 0x200000 | 0x83" in source
    source = source.replace("    mov dword [PD + 8], 0x200000 | 0x83", "    ; (second page left out)")
    path = work + "/nomap2.asm"
    with open(path, "w") as f:
        f.write(source)
    log = work + "/nomap2.log"
    with _pm(work, path, "nomap2.bin", extra=["-d", "int,cpu_reset", "-D", log]) as vm:
        assert vm.wait_exit(10), "the machine did not reset"


# --------------------------------------------------------------- module 15

def build_kernel(work, kernel_c=None, entry_patch=None, cflags_remove=(), pad=b"\0"):
    """Build os/kernel the way its Makefile does, in a scratch copy, so that
    variants can be tried. Returns the path of the disk image."""
    src = os.path.join(OS, "kernel")
    for name in ("boot.asm", "stage2.asm", "entry.asm", "kernel.c", "linker.ld"):
        shutil.copy(os.path.join(src, name), work)
    if kernel_c is not None:
        with open(work + "/kernel.c", "w") as f:
            f.write(kernel_c)
    if entry_patch is not None:
        old, new = entry_patch
        with open(work + "/entry.asm") as f:
            text = f.read()
        assert old in text, old
        with open(work + "/entry.asm", "w") as f:
            f.write(text.replace(old, new, 1))
    flags = ["-Wall", "-Wextra", "-O2", "-ffreestanding", "-fno-pie", "-fno-stack-protector",
             "-mno-red-zone", "-mgeneral-regs-only", "-fno-asynchronous-unwind-tables"]
    flags = [f for f in flags if f not in cflags_remove]
    for cmd in (["nasm", "-f", "bin", "boot.asm", "-o", "boot.bin"],
                ["nasm", "-f", "bin", "stage2.asm", "-o", "stage2.bin"],
                ["nasm", "-f", "elf64", "entry.asm", "-o", "entry.o"],
                ["gcc"] + flags + ["-c", "kernel.c", "-o", "kernel.o"],
                ["ld", "--no-warn-rwx-segments", "-T", "linker.ld", "-o", "kernel.elf", "entry.o", "kernel.o"],
                ["objcopy", "-O", "binary", "kernel.elf", "kernel.bin"]):
        done = subprocess.run(cmd, cwd=work, capture_output=True, text=True)
        assert done.returncode == 0 and not (cmd[0] == "gcc" and done.stderr), \
            "%s: %s" % (" ".join(cmd), done.stderr.strip())
    with open(work + "/kernel.bin", "rb") as f:
        kernel = f.read()
    size = 16 * 512
    assert len(kernel) <= size, "the kernel is %d bytes, boot.asm reads %d" % (len(kernel), size)
    kernel += pad * (size - len(kernel))
    with open(work + "/os.img", "wb") as out:
        for name in ("boot.bin", "stage2.bin"):
            with open(work + "/" + name, "rb") as f:
                out.write(f.read())
        out.write(kernel)
    return work + "/os.img"


KERNEL_OUTPUT = [
    "Hello from a kernel written in C",
    "kmain is at 0x0000000000010180",
    ".bss counter = 0x0000000000000000",
    ".data value  = 0x0000000000001234",
]


@check(15, "the C kernel boots: screen, serial and registers are what the lesson quotes")
def _(work):
    with Vm(build_kernel(work)) as vm:
        assert vm.wait_for_serial(".data value"), repr(vm.serial_text())
        time.sleep(0.3)
        assert vm.serial_text().splitlines() == KERNEL_OUTPUT, vm.serial_text().splitlines()
        assert [l for l in vm.screen_text() if l.strip()] == KERNEL_OUTPUT, vm.screen_text()
        dump = [l.rstrip() for l in vm.hmp("info registers").splitlines()]
        assert "CS =0018 0000000000000000 ffffffff 00af9a00 DPL=0 CS64 [-R-]" in dump
        assert "EFER=0000000000000500" in dump
    syms = subprocess.run(["nm", work + "/kernel.elf"], capture_output=True, text=True).stdout.splitlines()
    assert "0000000000010000 T _start" in syms and "0000000000010180 T kmain" in syms, syms
    size = subprocess.run(["size", work + "/kernel.elf"], capture_output=True, text=True).stdout.split("\n")[1].split()
    assert size[:3] == ["724", "4", "12"], size


@check(15, "os/kernel/Makefile builds the same image as the steps above")
def _(work):
    shutil.copytree(os.path.join(OS, "kernel"), work + "/k", ignore=shutil.ignore_patterns("*.bin", "*.o", "*.elf", "*.img"))
    done = subprocess.run(["make", "-C", work + "/k"], capture_output=True, text=True)
    assert done.returncode == 0 and "warning" not in done.stderr, done.stderr
    with open(work + "/k/os.img", "rb") as f:
        made = f.read()
    with open(build_kernel(work), "rb") as f:
        assert f.read() == made, "make and the manual steps built different images"


@check(15, "volatile: gcc drops the first of two stores without it, keeps both with it")
def _(work):
    with open(work + "/vol.c", "w") as f:
        f.write("#include <stdint.h>\nvoid plain(void){uint16_t *s=(uint16_t*)0xB8000; s[0]=0x0F41; s[0]=0x0F42;}\n"
                "void checked(void){volatile uint16_t *s=(volatile uint16_t*)0xB8000; s[0]=0x0F41; s[0]=0x0F42;}\n")
    out = subprocess.run(["gcc", "-O2", "-ffreestanding", "-fno-pie", "-fno-asynchronous-unwind-tables",
                          "-fcf-protection=none", "-S", "-masm=intel", work + "/vol.c", "-o", "-"],
                         capture_output=True, text=True).stdout
    plain = out.split("plain:")[1].split("checked:")[0]
    checked = out.split("checked:")[1]
    assert plain.count("WORD PTR ds:753664") == 1 and "mov\teax, 3906" in plain, plain
    assert checked.count("WORD PTR ds:753664") == 2 and "mov\teax, 3905" in checked, checked


@check(15, "SSE without -mgeneral-regs-only: #UD first, then a triple fault")
def _(work):
    source = ("#include <stdint.h>\nstruct regs { uint64_t a, b, c, d; };\n"
              "void kmain(void)\n{\n    volatile struct regs r = { 0 };\n    (void)r;\n"
              "    for (;;) __asm__ volatile (\"hlt\");\n}\n")
    img = build_kernel(work, kernel_c=source, cflags_remove=("-mgeneral-regs-only",))
    log = work + "/sse.log"
    with Vm(img, extra=["-d", "int,cpu_reset", "-D", log]) as vm:
        assert vm.wait_exit(10), "the machine did not reset"
    lines = [l.strip() for l in open(log) if "check_exception" in l or "Triple fault" in l]
    assert lines[0] == "check_exception old: 0xffffffff new 0x6" and lines[-1] == "Triple fault", lines


@check(15, "the zeroing loop: 0xAA padding shows up in .bss without it, and not with it")
def _(work):
    os.makedirs(work + "/a")
    os.makedirs(work + "/b")
    with Vm(build_kernel(work + "/a", entry_patch=("    rep stosb\n", "    ; (no zeroing)\n"), pad=b"\xaa")) as vm:
        assert vm.wait_for_serial(".data value")
        assert ".bss counter = 0xffffffffaaaaaaaa" in vm.serial_text().splitlines(), vm.serial_text()
    with Vm(build_kernel(work + "/b", pad=b"\xaa")) as vm:
        assert vm.wait_for_serial(".data value")
        assert ".bss counter = 0x0000000000000000" in vm.serial_text().splitlines(), vm.serial_text()


# --------------------------------------------------------------- module 16

def build_interrupts(work, patches=()):
    """Build os/interrupts in a scratch copy, applying (file, old, new) text
    patches first. Module 15's boot.asm, stage2.asm, entry.asm and
    linker.ld are used unchanged, as the real Makefile does. Returns the
    path of the disk image."""
    for name in ("boot.asm", "stage2.asm", "entry.asm", "linker.ld"):
        shutil.copy(os.path.join(OS, "kernel", name), work)
    for name in ("console.h", "console.c", "isr.asm", "idt.c", "kmain.c"):
        shutil.copy(os.path.join(OS, "interrupts", name), work)
    for name, old, new in patches:
        with open(work + "/" + name) as f:
            text = f.read()
        assert old in text, "patch target not found in %s: %r" % (name, old)
        with open(work + "/" + name, "w") as f:
            f.write(text.replace(old, new, 1))
    flags = ["-Wall", "-Wextra", "-O2", "-ffreestanding", "-fno-pie", "-fno-stack-protector",
             "-mno-red-zone", "-mgeneral-regs-only", "-fno-asynchronous-unwind-tables"]
    steps = [["nasm", "-f", "bin", "boot.asm", "-o", "boot.bin"],
             ["nasm", "-f", "bin", "stage2.asm", "-o", "stage2.bin"],
             ["nasm", "-f", "elf64", "entry.asm", "-o", "entry.o"],
             ["nasm", "-f", "elf64", "isr.asm", "-o", "isr.o"]]
    steps += [["gcc"] + flags + ["-c", c + ".c", "-o", c + ".o"] for c in ("console", "idt", "kmain")]
    steps += [["ld", "--no-warn-rwx-segments", "-T", "linker.ld", "-o", "kernel.elf",
               "entry.o", "isr.o", "console.o", "idt.o", "kmain.o"],
              ["objcopy", "-O", "binary", "kernel.elf", "kernel.bin"]]
    for cmd in steps:
        done = subprocess.run(cmd, cwd=work, capture_output=True, text=True)
        assert done.returncode == 0 and not (cmd[0] == "gcc" and done.stderr), \
            "%s: %s" % (" ".join(cmd), done.stderr.strip())
    with open(work + "/kernel.bin", "rb") as f:
        kernel = f.read()
    assert len(kernel) <= 16 * 512, "the kernel is %d bytes, boot.asm reads %d" % (len(kernel), 16 * 512)
    kernel += b"\0" * (16 * 512 - len(kernel))
    with open(work + "/os.img", "wb") as out:
        for name in ("boot.bin", "stage2.bin"):
            with open(work + "/" + name, "rb") as f:
                out.write(f.read())
        out.write(kernel)
    return work + "/os.img"


def symbol(work, name):
    nm = subprocess.run(["nm", work + "/kernel.elf"], capture_output=True, text=True).stdout
    return [int(l.split()[0], 16) for l in nm.splitlines() if l.endswith(" " + name)][0]


@check(16, "interrupts: a caught divide error, 100 timer ticks, keys by scancode, and the IDT register")
def _(work):
    with Vm(build_interrupts(work)) as vm:
        assert vm.wait_for_serial("timer: 100 ticks", 10), repr(vm.serial_text())
        lines = vm.serial_text().splitlines()
        assert lines[0] == "Interrupts: IDT, PIC and PIT set up", lines
        assert lines[1] == "exception 0 at 0x00000000000106bd" and lines[2] == "returned from the handler", lines
        assert lines[3] == "timer: 100 ticks", lines
        for key in "asd":
            vm.sendkey(key)
            time.sleep(0.4)
        assert vm.wait_for_serial("0x0000000000000020"), vm.serial_text()
        keys = [l for l in vm.serial_text().splitlines() if l.startswith("key: ")]
        assert keys == ["key: 0x000000000000001e", "key: 0x000000000000001f", "key: 0x0000000000000020"], keys
        screen = [l for l in vm.screen_text() if l.strip()]
        assert screen[:4] == lines[:4] and screen[4:7] == keys, screen
        dump = [l.rstrip() for l in vm.hmp("info registers").splitlines()]
        idt = [l for l in dump if l.startswith("IDT=")]
        assert idt and idt[0].split()[-1] == "000002ff", idt     # 48 gates * 16 bytes - 1
    assert symbol(work, "isr_table") and symbol(work, "ticks")


@check(16, "no end-of-interrupt: the timer fires once, then never again")
def _(work):
    img = build_interrupts(work, [("idt.c", "    outb(0x20, 0x20);           /* end of interrupt: without this, no more arrive */",
                                   "    /* (trap variant: no end-of-interrupt) */")])
    with Vm(img) as vm:
        assert vm.wait_for_serial("returned from the handler", 8)
        time.sleep(2.5)
        assert "timer: 100 ticks" not in vm.serial_text(), "the timer kept going"
        assert int.from_bytes(vm.mem(symbol(work, "ticks"), 8), "little") == 1


@check(16, "no PIC remap: the timer arrives as exception 8, the double fault")
def _(work):
    img = build_interrupts(work, [("idt.c", "    pic_remap();\n", "    (void)pic_remap;            /* (trap variant: never called) */\n")])
    with Vm(img) as vm:
        assert vm.wait_for_serial("exception 8 at", 8), repr(vm.serial_text())


@check(16, "an IDT gate with the 32-bit code selector: even the divide error ends in a triple fault")
def _(work):
    img = build_interrupts(work, [("idt.c", "#define CODE64 0x18", "#define CODE64 0x08")])
    log = work + "/badsel.log"
    with Vm(img, extra=["-d", "int,cpu_reset", "-D", log]) as vm:
        assert vm.wait_exit(10), "the machine did not reset"
    lines = [l.strip() for l in open(log) if "check_exception" in l or "Triple fault" in l]
    assert lines[:4] == ["check_exception old: 0xffffffff new 0xd", "check_exception old: 0xd new 0xd",
                         "check_exception old: 0x8 new 0xd", "Triple fault"], lines


def main():
    wanted = {int(a) for a in sys.argv[1:]}
    if not qemu_available():
        print("qemu-system-x86_64 not found: the operating-system checks were NOT run")
        return 0
    failed = 0
    for module, name, fn in CHECKS:
        if wanted and module not in wanted:
            continue
        work = tempfile.mkdtemp(prefix="asmos-")
        try:
            fn(work)
            print("module {:2d}: ok      {}".format(module, name))
        except Exception as err:          # a failed check is reported, not fatal
            failed += 1
            print("module {:2d}: FAILED  {}\n    {}".format(module, name, err))
        finally:
            shutil.rmtree(work, ignore_errors=True)
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
