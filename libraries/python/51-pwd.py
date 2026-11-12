"""
TITLE: pwd: the password database
GROUP: System information
LIBRARY: Python standard library (pwd)
USES: import pwd
SUMMARY: Reads /etc/passwd (and whatever the system's name service adds):
  the account behind a user id, its home directory and login shell.
  Unix only.
ENTRY: pwd.getpwuid(uidobj, /)
   Return the password database entry for the given numeric user ID.
ENTRY: pwd.getpwnam(name, /)
   Return the password database entry for the given user name.
ENTRY: pwd.getpwall()
   Return a list of all available password database entries, in
   arbitrary order.
ENTRY: pwd.struct_passwd(iterable=(), /)
   pwd.struct_passwd: Results from getpw*() routines.
NOTES:
  - $SHELL is the shell you started from; pw_shell is the login
    shell: they can differ.
  - getpwuid() raises KeyError for an unknown id.
  - The module does not exist on Windows.
  - Signatures and descriptions are Python's own, read from the
    module's docstrings for the version that runs this page.
    Version notes (added, changed, deprecated) are in the online
    documentation.
SEE: https://docs.python.org/3/library/pwd.html, pydoc pwd
"""

import os
import pwd

account = pwd.getpwuid(os.getuid())
print("uid           ", os.getuid())
print("pw_name       ", account.pw_name)
print("pw_dir        ", account.pw_dir)
print("pw_shell      ", account.pw_shell)
print("$SHELL        ", os.environ.get("SHELL", "(not set)"))
print("getpwnam('root').pw_uid:", pwd.getpwnam("root").pw_uid)
try:
    pwd.getpwnam("no-such-user-here")
except KeyError as error:
    print("unknown user raises KeyError:", error)
