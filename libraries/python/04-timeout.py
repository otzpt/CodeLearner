"""
TITLE: how-to: give up on something slow
GROUP: How-to guides
USES: import subprocess, socket
SUMMARY: Anything that waits on the outside world can wait for ever. Most calls
  that do take a timeout, in seconds, and raise an exception when it runs out.
SIGNATURE:
  subprocess.run(args, timeout=None)
  socket.create_connection(address, timeout=None)
  socket.socket.settimeout(seconds)
  urllib.request.urlopen(url, timeout=None)
PARAMETERS:
  - timeout, seconds: how long to wait, as a float. None waits for ever.
  - address: a (host, port) tuple.
RETURNS: The same as without a timeout, when it finishes in time.
ERRORS:
  - subprocess.TimeoutExpired: the program took too long, and has been killed.
  - TimeoutError: a socket operation ran out of time (socket.timeout is
    another name for it since Python 3.10).
  - urlopen can raise either TimeoutError or urllib.error.URLError.
NOTES:
  - The default is usually no limit at all, so a timeout has to be asked for.
  - A socket timeout applies to each operation, not to the whole exchange.
  - Time things with time.monotonic(), because the wall clock can jump.
USEDFOR:
  - Any network call, and any external command that might hang.
SEE: pydoc subprocess.run, pydoc socket.socket.settimeout
"""

import socket
import subprocess
import sys
import time

try:
    subprocess.run([sys.executable, "-c", "import time; time.sleep(30)"], timeout=0.3)
except subprocess.TimeoutExpired as error:
    print(f"subprocess: {type(error).__name__} after {error.timeout} s")

with socket.create_server(("127.0.0.1", 0)) as server:
    with socket.create_connection(server.getsockname(), timeout=2) as client:
        client.settimeout(0.2)
        started = time.monotonic()
        try:
            client.recv(100)
        except TimeoutError as error:
            print(f"socket: {type(error).__name__} after about {time.monotonic() - started:.1f} s")
