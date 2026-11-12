"""
TITLE: subprocess: running other programs
GROUP: Files, paths and processes
LIBRARY: Python standard library (subprocess)
USES: import subprocess
SUMMARY: Starts a program, optionally feeds it input and collects its
  output and exit status. run() covers almost every case; Popen is
  the lower-level class under it.
ENTRY: subprocess.run(*popenargs, input=None, capture_output=False,
    timeout=None, check=False, **kwargs)
   Run command with arguments and return a CompletedProcess instance.
ENTRY: subprocess.check_output(*popenargs, timeout=None, **kwargs)
   Run command with arguments and return its output.
ENTRY: subprocess.check_call(*popenargs, **kwargs)
   Run command with arguments. Wait for command to complete. If the
   exit code was zero then return, otherwise raise
   CalledProcessError. The CalledProcessError object will have the
   return code in the returncode attribute.
ENTRY: subprocess.call(*popenargs, timeout=None, **kwargs)
   Run command with arguments. Wait for command to complete or for
   timeout seconds, then return the returncode attribute.
ENTRY: subprocess.Popen(args, bufsize=-1, executable=None, stdin=None,
    stdout=None, stderr=None, preexec_fn=None, close_fds=True,
    shell=False, cwd=None, env=None, universal_newlines=None,
    startupinfo=None, creationflags=0, restore_signals=True,
    start_new_session=False, pass_fds=(), *, user=None, group=None,
    extra_groups=None, encoding=None, errors=None, text=None, umask=-1,
    pipesize=-1, process_group=None)
   Execute a child program in a new process.
ENTRY: subprocess.CompletedProcess(args, returncode, stdout=None, stderr=None)
   A process that has finished running.
ENTRY: subprocess.CalledProcessError(returncode, cmd, output=None, stderr=None)
   Raised when run() is called with check=True and the process
   returns a non-zero exit status.
ENTRY: subprocess.TimeoutExpired(cmd, timeout, output=None, stderr=None)
   This exception is raised when the timeout expires while waiting
   for a child process.
ENTRY: subprocess.PIPE = -1
   A value for stdin, stdout or stderr: create a pipe to the child
   that the parent can read or write.
ENTRY: subprocess.DEVNULL = -3
   A value for stdin, stdout or stderr: send the stream to
   os.devnull, discarding it.
NOTES:
  - Pass a list of arguments, not a string, unless you need the
    shell. shell=True with user input is a command-injection hole.
  - Without check=True, a failing program does not raise: look at
    returncode.
  - capture_output=True is stdout=PIPE plus stderr=PIPE. A very
    large output is held in memory.
  - timeout= kills the child and raises TimeoutExpired; without it
    a hung child hangs you.
  - Signatures and descriptions are Python's own, read from the
    module's docstrings for the version that runs this page.
    Version notes (added, changed, deprecated) are in the online
    documentation.
SEE: https://docs.python.org/3/library/subprocess.html, pydoc subprocess
"""

import subprocess

done = subprocess.run(["uname", "-s"], capture_output=True, text=True, check=True, timeout=5)
print("stdout:", repr(done.stdout), " returncode:", done.returncode)

failed = subprocess.run(["ls", "/no/such/dir"], capture_output=True, text=True)
print("exit status", failed.returncode, "- stderr:", failed.stderr.strip())

try:
    subprocess.run(["ls", "/no/such/dir"], capture_output=True, check=True)
except subprocess.CalledProcessError as error:
    print("check=True raises CalledProcessError, returncode", error.returncode)

try:
    subprocess.run(["sleep", "5"], timeout=0.2)
except subprocess.TimeoutExpired as error:
    print("timeout=0.2 raises TimeoutExpired after", error.timeout, "s")

try:
    subprocess.run(["no-such-program"])
except FileNotFoundError as error:
    print("a missing program raises FileNotFoundError:", error.filename)

print("a list is safe; a string needs shell=True:", subprocess.run("echo $((1 + 2))", shell=True, capture_output=True, text=True).stdout.strip())
