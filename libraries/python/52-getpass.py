"""
TITLE: getpass: portable password input
GROUP: System information
LIBRARY: Python standard library (getpass)
USES: import getpass
SUMMARY: Prompts for a password without echoing it, and finds the current
  user name. getuser() looks at LOGNAME, USER, LNAME and USERNAME
  before the password database.
ENTRY: getpass.getuser()
   Get the username from the environment or password database.
ENTRY: getpass.getpass(prompt='Password: ', stream=None, *, echo_char=None)
   Prompt for a password, with echo turned off.
NOTES:
  - getpass() reads from /dev/tty, not from a pipe, so it cannot
    be fed by shell redirection.
  - getuser() trusts environment variables; use
    pwd.getpwuid(os.getuid()) for the real account.
  - Signatures and descriptions are Python's own, read from the
    module's docstrings for the version that runs this page.
    Version notes (added, changed, deprecated) are in the online
    documentation.
SEE: https://docs.python.org/3/library/getpass.html, pydoc getpass
"""

import getpass

print("getpass.getuser():", getpass.getuser())
# getpass.getpass("Password: ") reads from the terminal with echo turned off.
# It is not called here because this page runs without a terminal to type into.
