"""
TITLE: Running another program (subprocess)
GROUP: Basics
USES: import subprocess, shutil
SUMMARY: subprocess.run() starts a program, waits, and gives you its output.
  It is the way to use a command line tool (lspci, ip, xrandr) from Python.
NOTES:
  - Pass the command as a LIST: ["ip", "-br", "addr"]. Do not build a string
    and use shell=True with user input: that is how injection happens.
  - capture_output=True, text=True gives result.stdout as a str.
  - result.returncode is 0 on success. check=True raises
    CalledProcessError instead.
  - Always pass timeout=: a program that hangs will hang yours.
  - shutil.which("lspci") finds a program on PATH, or returns None. Use it
    before running a tool that may not be installed.
  - If the information is also in a file or a system call, read that instead:
    it is faster and has no dependency.
SEE: pydoc subprocess.run, pydoc shutil.which
"""

import shutil
import subprocess

print("echo ->", subprocess.run(["echo", "hello"], capture_output=True, text=True, timeout=5).stdout.strip())

for tool in ("uname", "lspci", "no-such-tool"):
    print(f"which {tool}:", shutil.which(tool) or "(not installed)")

result = subprocess.run(["uname", "-sr"], capture_output=True, text=True, timeout=5)
print("uname -sr ->", result.stdout.strip(), "| return code", result.returncode)

failed = subprocess.run(["ls", "/no/such/directory"], capture_output=True, text=True, timeout=5)
print("ls on a missing directory: return code", failed.returncode, "|", failed.stderr.strip())
