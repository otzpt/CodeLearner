#!/usr/bin/env python3
"""Walk every module of the Assembly course and check what it promises.

For each module this starts ./asm-course, picks the module, answers every
prompt with a wrong answer (the course accepts that and shows the right one),
asks to see the example solution, and checks that the module reaches its
SUMMARY and the menu quits cleanly. Then every example solution the course
shows is assembled and run, and its output compared with what the challenge
text says it must print.

    make && python3 check-course.py [module numbers...]

The course reads with raw read(2), so a line typed ahead of its prompt is
lost. Input is therefore sent through a pseudo-terminal, one line each time
the program has gone quiet, the way a person types.
"""

import os
import pty
import re
import select
import shutil
import subprocess
import sys
import tempfile
import termios
import time

HERE = os.path.dirname(os.path.abspath(__file__))
BINARY = os.path.join(HERE, "asm-course")

# What each module's challenge says its example must print, and the input to
# feed it one line at a time. Module 2 is run twice: the example holds 25,
# and the challenge also promises "small" when it holds 3.
CHALLENGES = {
    1: dict(expect="done\n", feed=[]),
    2: dict(expect="big\n", feed=[]),
    3: dict(expect="1\n2\n3\n4\n5\n", feed=[]),
    4: dict(expect="Guess (1-100): Too high.\nGuess (1-100): Too low.\n"
                   "Guess (1-100): Got it.\n", feed=["50", "20", "37"]),
    5: dict(expect="desserts\n", feed=[]),
    6: dict(expect="", feed=[], exit=55),      # fib(10) is the exit status
    7: dict(expect="", feed=[], exit=5),       # the 1 bits in 0xB6
    8: dict(expect="", feed=[], exit=16),      # strlen('operating system')
    9: dict(expect="5\n0\n10\n", feed=[], c_main="examples/09/clamp_main.c"),
    10: dict(boot=True),                       # a 512-byte boot sector that shows OK
    11: dict(boot=True),                       # a red X at row 5, column 10
    12: dict(boot=True),                       # sector 1 prints 'two' on COM1
    13: dict(boot=True),                       # protected mode, a white-on-red P
    14: dict(boot=True),                       # long mode, an M read back from 3 MiB
    15: dict(kernel=True),                     # print_dec added to the C kernel
    16: dict(interrupts=True),                 # a dot a second from the timer handler
}


def _boot_ok_line(vm):
    if not vm.wait_for_screen("OK", 8.0):
        return "did not show OK on the emulated screen"
    if "OK" not in [l.strip() for l in vm.screen_text()]:
        return "OK is not on a line of its own"


def _boot_red_x(vm):
    end = time.time() + 8.0
    while time.time() < end and vm.screen_text()[5][10:11] != "X":
        time.sleep(0.1)
    screen = vm.screen_text()
    if screen[5][10:11] != "X":
        return "row 5, column 10 holds %r, not 'X'" % screen[5][10:11]
    if vm.screen_attrs(5, 10)[0] != 0x04:
        return "the X has attribute %#x, not 0x04 (red on black)" % vm.screen_attrs(5, 10)[0]
    if screen[0].strip():
        return "the screen was not cleared: row 0 still holds %r" % screen[0]


def _boot_two_on_serial(vm):
    if not vm.wait_for_serial("two", 8.0):
        return "nothing on the serial port, got %r" % vm.serial_text()
    if vm.serial_text().split("\n")[0] != "two":
        return "the first serial line is %r, not 'two'" % vm.serial_text().split("\n")[0]


def _boot_pm_p(vm):
    end = time.time() + 8.0
    while time.time() < end and vm.screen_text()[0][:1] != "P":
        time.sleep(0.1)
    if vm.screen_text()[0][:1] != "P":
        return "the top-left cell holds %r, not 'P'" % vm.screen_text()[0][:1]
    if vm.screen_attrs(0, 0)[0] != 0x4F:
        return "the P has attribute %#x, not 0x4F" % vm.screen_attrs(0, 0)[0]
    regs = vm.registers()
    if regs["CS"] != 0x08 or regs["CR0"] & 1 != 1:
        return "CS = %#x, CR0 = %#x: not in protected mode with cs = 8" % (regs["CS"], regs["CR0"])


def _boot_lm_m(vm):
    end = time.time() + 8.0
    while time.time() < end and vm.screen_text()[0][:1] != "M":
        time.sleep(0.1)
    if vm.screen_text()[0][:1] != "M":
        return "the top-left cell holds %r, not 'M'" % vm.screen_text()[0][:1]
    if vm.screen_attrs(0, 0)[0] != 0x1F:
        return "the M has attribute %#x, not 0x1F" % vm.screen_attrs(0, 0)[0]
    efer = [l for l in vm.hmp("info registers").splitlines() if l.startswith("EFER=")]
    if not efer or int(efer[0].split("=")[1], 16) != 0x500:
        return "EFER is %r, not 0x500" % efer


BOOT_CHECKS = {10: _boot_ok_line, 11: _boot_red_x, 12: _boot_two_on_serial, 13: _boot_pm_p, 14: _boot_lm_m}
BOOT_SIZES = {10: 512, 11: 512, 12: 1024, 13: 512, 14: 512}      # what each challenge says the file must be

# Programs the lessons show and quote the output of. Each is built from the
# real files under examples/ and run; what it prints must equal `expect`, and
# every line of that output must also appear on the module's own screens, so
# the text cannot drift from what the machine does. {w} is a scratch folder.
EXAMPLES = [
    dict(module=9, name="max.asm + main.c",
         build=[["nasm", "-f", "elf64", "examples/09/max.asm", "-o", "{w}/max.o"],
                ["gcc", "-Wall", "-Wextra", "examples/09/main.c", "{w}/max.o", "-o", "{w}/demo"]],
         run="{w}/demo", expect="asm_max(3, 9)   = 9\nasm_max(-5, -2) = -2\n", exit=0),
    dict(module=9, name="hello.asm",
         build=[["nasm", "-f", "elf64", "examples/09/hello.asm", "-o", "{w}/hello.o"],
                ["gcc", "{w}/hello.o", "-o", "{w}/hello"]],
         run="{w}/hello", expect="the answer is 42\n", exit=0),
    dict(module=9, name="start.asm + free.c",
         build=[["nasm", "-f", "elf64", "examples/09/start.asm", "-o", "{w}/start.o"],
                ["gcc", "-Wall", "-Wextra", "-ffreestanding", "-nostdlib", "-static", "-fno-pie",
                 "-no-pie", "-O2", "-c", "examples/09/free.c", "-o", "{w}/free.o"],
                ["ld", "{w}/start.o", "{w}/free.o", "-o", "{w}/free"]],
         run="{w}/free", expect="hello from C, with no libc\n", exit=7),
]


# Lines the lessons compute while they run. Each is checked against a value
# worked out here, independently of the assembly, so a lesson cannot quietly
# print a wrong number. (Addresses and the stack limit vary and are checked
# by rule in check_live instead.)
LIVE = {
    5: ["mov rax, [counter]   ->  rax = 42",
        "mov byte  [x], 5   ->  x = 0xffffffffffffff05",
        "mov qword [x], 5   ->  x = 0x0000000000000005",
        "mov eax, 5   ->  rax = 0x0000000000000005",
        "mov ax, 5    ->  rax = 0xffffffffffff0005",
        "mov al, 5    ->  rax = 0xffffffffffffff05",
        "sum of the five dwords        = %d" % sum([1, 2, 3, 4, 5]),
        "with rbx = 7                  = %d" % (7 * 5),
        "address + 0   = 0x44", "address + 1   = 0x33",
        "address + 2   = 0x22", "address + 3   = 0x11"],
    6: ["difference (before - after)   = 8",
        "[rsp], the value pushed       = 0x1111",
        "rsp mod 16, before the call   = 0",
        "rsp mod 16, inside the callee = 8",
        "sum7(1, 2, 3, 4, 5, 6, 7)     = %d" % sum(range(1, 8)),
        "after m6_good, rbx   = 0x1111",
        "after m6_bad,  rbx   = 0x1112",
        "fact(5)                            = 120",
        "stack used by fact(5), in bytes    = %d" % (4 * 16 + 8)],
    7: ["value          = 1011 0110",
        "and 0x0F       = %s" % format(0xB6 & 0x0F, "08b")[:4] + " " + format(0xB6 & 0x0F, "08b")[4:],
        "or  0x01       = %s %s" % (format(0xB6 | 1, "08b")[:4], format(0xB6 | 1, "08b")[4:]),
        "xor 0xFF       = %s %s" % (format(0xB6 ^ 0xFF, "08b")[:4], format(0xB6 ^ 0xFF, "08b")[4:]),
        "not (low byte) = %s %s" % (format(~0xB6 & 0xFF, "08b")[:4], format(~0xB6 & 0xFF, "08b")[4:]),
        "rax = -16        = 0x%016x" % (-16 & (2**64 - 1)),
        "shr rax, 2       = 0x%016x" % ((-16 & (2**64 - 1)) >> 2),
        "sar rax, 2       = 0x%016x" % (-4 & (2**64 - 1)),
        "1 shl 63         = 0x%016x" % (1 << 63),
        "255 + 1  ->  al = 0x00  CF=1 ZF=1 SF=0 OF=0",
        "127 + 1  ->  al = 0x80  CF=0 ZF=0 SF=1 OF=1",
        "0 - 1  ->  al = 0xff  CF=1 ZF=0 SF=1 OF=0",
        "cqo, then idiv       quotient = 0x%016x" % (-3 & (2**64 - 1)),
        "remainder = 0x%016x" % (-1 & (2**64 - 1)),
        "xor rdx, then idiv   quotient = 0x%016x" % (((2**64 - 7) // 2)),
        "movzx rax, al   (zero-extend)  = 0x0000000000000080",
        "movsx rax, al   (sign-extend)  = 0xffffffffffffff80",
        "32-bit code, access 0x9A, flags 0xC  = 0x00cf9a000000ffff",
        "32-bit data, access 0x92, flags 0xC  = 0x00cf92000000ffff",
        "64-bit code, access 0x9A, flags 0xA  = 0x00af9a000000ffff"],
    8: ["destination = Hello, rep movsb!",
        "rep stosb of '*'          : **********",
        "rep stosq of eight '#'    : ################",
        "repne scasb on 'assembly' : length = %d" % len("assembly"),
        "kernel vs kernal : first difference at index %d" % [a != b for a, b in zip("kernel", "kernal")].index(True),
        "kernel vs kernel : equal",
        "forward  (cld) : abababab",
        "backward (std) : ababcdef",
        "bad  : returned - dest = 5",
        "good : returned - dest = 0"],
    10: ["0x07C0:0x0000  =  0x%x" % (0x07C0 * 16),
         "0x0000:0x7C00  =  0x%x" % 0x7C00,
         "0xB800:0x0000  =  0x%x" % (0xB800 * 16),
         "0xFFFF:0x000F  =  0x%x" % (0xFFFF * 16 + 0xF),
         "0xFFFF:0x0010  =  0x%x" % (0xFFFF * 16 + 0x10)],
    13: ["limit  (bits 0-15, 48-51)  = 0xfffff",
         "base   (bits 16-39, 56-63) = 0x0",
         "access (bits 40-47)        = 0x9a",
         "flags  (bits 52-55)        = 0xc"],
}


def _split(v):
    """The PML4, PDPT, PD, PT indices and 12-bit offset of a virtual address."""
    return [(v >> s) & 0x1FF for s in (39, 30, 21, 12)] + [v & 0xFFF]


LIVE[14] = []
for name, addr in (("0xB8000, the screen:", 0xB8000), ("0xFFFFFFFF80000000:", 0xFFFFFFFF80000000)):
    pml4, pdpt, pd, pt, off = _split(addr)
    LIVE[14] += [name, "PML4 index = %d" % pml4, "PDPT index = %d" % pdpt, "PD index   = %d" % pd,
                 "PT index   = %d" % pt, "offset     = 0x%x" % off]


def check_live(number, text):
    """Returns a list of problems with the lines this module computed live."""
    shown = [l.strip() for l in text.splitlines()]
    problems = ["missing: " + want for want in LIVE.get(number, []) if want not in shown]
    if number == 5:                       # the first is an address; the second its contents
        got = [l for l in shown if l.startswith("mov rax, counter     ->  rax = 0x")]
        if not got:
            problems.append("missing: the address of counter")
    if number == 6:
        ret = [l.split("= ")[1] for l in shown if l.startswith(("return address on the stack", "address after the call"))]
        if len(ret) != 2 or ret[0] != ret[1]:
            problems.append("the return address on the stack differs from the address after the call: %r" % ret)
        after = [l.split("= ")[1] for l in shown if l.startswith("rcx after the syscall") or l.startswith("address after the syscall")]
        if len(after) != 2 or after[0] != after[1]:
            problems.append("rcx after the syscall differs from the address after it: %r" % after)
        limit = [l for l in shown if l.startswith("this process's stack limit")]
        import resource
        soft = resource.getrlimit(resource.RLIMIT_STACK)[0]
        if not limit or (soft != resource.RLIM_INFINITY and not limit[0].endswith("= %d" % soft)):
            problems.append("stack limit line %r, getrlimit says %r" % (limit, soft))
    return problems


def drive(argv, lines, quiet=0.08, timeout=60, cwd=None):
    """Run argv on a pty. `lines` is either a list to send one at a time, or a
    function that is given everything printed so far and returns the next line
    to type (or None to type nothing). A line is sent each time output goes quiet."""
    master, slave = pty.openpty()
    attrs = termios.tcgetattr(slave)
    attrs[3] &= ~termios.ECHO
    termios.tcsetattr(slave, termios.TCSANOW, attrs)
    proc = subprocess.Popen(argv, stdin=slave, stdout=slave, stderr=slave,
                            close_fds=True, cwd=cwd)
    os.close(slave)
    out = b""
    pending = list(lines) if not callable(lines) else None
    start = last = time.time()
    while True:
        ready, _, _ = select.select([master], [], [], 0.02)
        if ready:
            try:
                data = os.read(master, 65536)
            except OSError:
                break
            if not data:
                break
            out += data
            last = time.time()
        else:
            if proc.poll() is not None:
                break
            if time.time() - last > quiet:
                if pending is not None:
                    nxt = pending.pop(0) if pending else None
                else:
                    nxt = lines(out.decode(errors="replace").replace("\r\n", "\n"))
                if nxt is not None:
                    os.write(master, (nxt + "\n").encode())
                    last = time.time()
                elif time.time() - last > 0.6:
                    os.write(master, b"\x04")        # nothing to type: end of input
                    last = time.time()
        if time.time() - start > timeout:
            proc.kill()
            break
    code = proc.wait()
    os.close(master)
    return out.decode(errors="replace").replace("\r\n", "\n"), code


def module_count():
    text, _ = drive([BINARY], ["0"])
    return len(re.findall(r"^\s+\[\s*\d+\]", text, re.M)) - 1  # minus Quit


def walk(number):
    """One module from the menu to its SUMMARY. Returns (ok, text).

    Every prompt gets the answer that costs nothing: ENTER to page on, a wrong
    answer to a question (the course shows the right one), "y" to see the
    example solution. The module's number is typed at the first menu, "0" at
    the second."""
    state = {"picked": False}

    def respond(out):
        if out.endswith("Pick a module: "):
            if not state["picked"]:
                state["picked"] = True
                return str(number)
            return "0"
        if out.endswith("Press ENTER to continue..."):
            return ""
        if out.endswith("Your answer: "):
            return "x"
        if out.endswith("(y/N): "):
            return "y"
        return None

    text, code = drive([BINARY], respond, timeout=120)
    ok = ("SUMMARY" in text and code == 0 and "See you next time." in text)
    return ok, text


def shown_solution(text):
    """The example source the course printed between two rules, after it asked
    'Want to see example code?'."""
    rule = "  ------------------------------------------------------\n"
    start = text.find("Want to see example code?")
    if start < 0:
        return None
    parts = text[start:].split(rule)
    return parts[1].lstrip("\n") if len(parts) > 2 else None


def build_and_run(source, feed, workdir, c_main=None):
    asm = os.path.join(workdir, "prog.asm")
    obj = os.path.join(workdir, "prog.o")
    exe = os.path.join(workdir, "prog")
    with open(asm, "w") as f:
        f.write(source)
    link = ["ld", obj, "-o", exe]
    if c_main:
        link = ["gcc", os.path.join(HERE, c_main), obj, "-o", exe]
    for cmd in (["nasm", "-f", "elf64", asm, "-o", obj], link):
        done = subprocess.run(cmd, capture_output=True, text=True)
        if done.returncode:
            return None, done.stderr
    out, code = drive([exe], feed, quiet=0.15, timeout=10)
    return (out, code), ""


def check_boot_solution(source, check, size, workdir):
    """Assemble a boot sector, check its size, boot it and look for a line on screen."""
    sys.path.insert(0, HERE)
    from vm import Vm, qemu_available
    if not qemu_available():
        return None, "qemu-system-x86_64 not found: boot challenge NOT checked"
    asm = os.path.join(workdir, "boot.asm")
    img = os.path.join(workdir, "boot.bin")
    with open(asm, "w") as f:
        f.write(source)
    done = subprocess.run(["nasm", "-f", "bin", asm, "-o", img], capture_output=True, text=True)
    if done.returncode:
        return False, "does not assemble: " + done.stderr.strip()[:200]
    if os.path.getsize(img) != size:
        return False, "is %d bytes, not %d" % (os.path.getsize(img), size)
    with Vm(img) as vm:
        problem = check(vm)
    return (False, problem) if problem else (True, "")


def check_kernel_solution(solution, workdir):
    """Splice the shown print_dec into a copy of os/kernel/kernel.c, build the
    image with make, boot it and look for the line on the emulated screen."""
    sys.path.insert(0, HERE)
    from vm import Vm, qemu_available
    if not qemu_available():
        return None, "qemu-system-x86_64 not found: kernel challenge NOT checked"
    above, _, rest = solution.partition("/* ---- inside kmain, before the final for (;;) ---- */")
    function = above.split("*/", 1)[1]
    work = os.path.join(workdir, "kernel")
    shutil.copytree(os.path.join(HERE, "os", "kernel"), work,
                    ignore=shutil.ignore_patterns("*.bin", "*.o", "*.elf", "*.img"))
    with open(os.path.join(work, "kernel.c")) as f:
        source = f.read()
    source = source.replace("void kmain(void)", function.strip("\n") + "\n\nvoid kmain(void)", 1)
    source = source.replace("    for (;;)\n        __asm__ volatile", rest.strip("\n") + "\n\n    for (;;)\n        __asm__ volatile", 1)
    with open(os.path.join(work, "kernel.c"), "w") as f:
        f.write(source)
    done = subprocess.run(["make", "-C", work], capture_output=True, text=True)
    if done.returncode or "warning" in done.stderr:
        return False, "does not build cleanly: " + (done.stderr or done.stdout).strip()[-300:]
    with Vm(os.path.join(work, "os.img")) as vm:
        if not vm.wait_for_screen("dec = 123456789", 8.0):
            return False, "the screen does not show 'dec = 123456789': %r" % [l for l in vm.screen_text() if l.strip()]
        if not vm.wait_for_screen(".data value  = 0x0000000000001234", 3.0):
            return False, "the existing kernel output is gone"
    return True, ""


def check_interrupt_solution(workdir):
    """Patch the shown timer-handler change into a copy of os/interrupts, build,
    boot, and count the dots that arrive on the serial port."""
    sys.path.insert(0, HERE)
    from vm import Vm, qemu_available
    if not qemu_available():
        return None, "qemu-system-x86_64 not found: interrupts challenge NOT checked"
    import importlib.util
    spec = importlib.util.spec_from_file_location("check_os", os.path.join(HERE, "check-os.py"))
    co = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(co)
    patch = ("idt.c", "        ticks++;\n",
             "        ticks++;\n        if (ticks % 100 == 0)\n            print(\".\", 0x0E);\n")
    try:
        image = co.build_interrupts(workdir, [patch])
    except AssertionError as err:
        return False, "does not build cleanly: " + str(err)[-300:]
    with Vm(image) as vm:
        if not vm.wait_for_serial("timer: 100 ticks", 10.0):
            return False, "the kernel never reached 'timer: 100 ticks'"
        end = time.time() + 6.0
        while time.time() < end and vm.serial_text().count(".") < 4:
            time.sleep(0.2)
        dots = vm.serial_text().count(".")
        if dots < 4:
            return False, "only %d dots arrived on the serial port" % dots
    return True, ""


def check_examples(texts):
    """Build and run every EXAMPLES entry. texts maps module -> the screens' text."""
    failed = 0
    for ex in EXAMPLES:
        if ex["module"] not in texts:
            continue                      # that module was not walked this time
        with tempfile.TemporaryDirectory() as work:
            fill = lambda cmd: [a.replace("{w}", work) for a in cmd]
            error = None
            for cmd in ex["build"]:
                done = subprocess.run(fill(cmd), capture_output=True, text=True, cwd=HERE)
                if done.returncode:
                    error = "build failed: " + (done.stderr or done.stdout).strip()[:300]
                    break
            if error is None:
                done = subprocess.run([ex["run"].replace("{w}", work)], capture_output=True, text=True)
                if done.stdout != ex["expect"] or done.returncode != ex["exit"]:
                    error = "printed {!r} (exit {}), expected {!r} (exit {})".format(
                        done.stdout, done.returncode, ex["expect"], ex["exit"])
            if error is None:
                shown = {l.strip() for l in texts.get(ex["module"], "").splitlines()}
                missing = [l for l in ex["expect"].splitlines() if l.strip() not in shown]
                if missing:
                    error = "the module does not show its output line(s): {!r}".format(missing)
            print("example {} (module {}): {}".format(ex["name"], ex["module"], "ok" if error is None else "FAILED"))
            if error:
                print("  " + error)
                failed += 1
    return failed


def main():
    if not os.path.exists(BINARY):
        sys.exit("build first: make")

    count = module_count()
    wanted = [int(n) for n in sys.argv[1:]] or list(range(1, count + 1))
    failed = 0
    texts = {}

    for number in wanted:
        ok, text = walk(number)
        texts[number] = text
        print("module {:2d}: {}".format(number, "ok" if ok else "FAILED"))
        for problem in check_live(number, text):
            print("  live line " + problem)
            ok = False
            failed += 1
        if not ok:
            failed += 1
            print("  tail:", repr(text[-300:]))
            continue
        spec = CHALLENGES.get(number)
        if not spec:
            continue
        source = shown_solution(text)
        if source is None:
            print("  example solution: not found in the output")
            failed += 1
            continue
        if spec.get("interrupts"):
            with tempfile.TemporaryDirectory() as work:
                ok2, why = check_interrupt_solution(work)
            if ok2 is None:
                print("  example solution: " + why)
            elif ok2:
                print("  example solution: ok (built and booted in QEMU)")
            else:
                print("  example solution " + why)
                failed += 1
            continue
        if spec.get("kernel"):
            with tempfile.TemporaryDirectory() as work:
                ok2, why = check_kernel_solution(source, work)
            if ok2 is None:
                print("  example solution: " + why)
            elif ok2:
                print("  example solution: ok (built and booted in QEMU)")
            else:
                print("  example solution " + why)
                failed += 1
            continue
        if spec.get("boot"):
            with tempfile.TemporaryDirectory() as work:
                ok2, why = check_boot_solution(source, BOOT_CHECKS[number], BOOT_SIZES[number], work)
            if ok2 is None:
                print("  example solution: " + why)
            elif ok2:
                print("  example solution: ok (booted in QEMU)")
            else:
                print("  example solution " + why)
                failed += 1
            continue
        with tempfile.TemporaryDirectory() as work:
            variants = [(source, spec["expect"])]
            if number == 2:
                variants.append((source.replace("mov rax, 25", "mov rax, 3"), "small\n"))
            for src, expect in variants:
                result, err = build_and_run(src, spec["feed"], work, spec.get("c_main"))
                if result is None:
                    print("  example solution does not build:", err.strip()[:200])
                    failed += 1
                elif result[0] != expect or result[1] != spec.get("exit", 0):
                    print("  example solution printed {!r} (exit {}), expected {!r} (exit {})".format(
                        result[0], result[1], expect, spec.get("exit", 0)))
                    failed += 1
                else:
                    print("  example solution: ok")
    failed += check_examples(texts)
    sys.exit(1 if failed else 0)


if __name__ == "__main__":
    main()
