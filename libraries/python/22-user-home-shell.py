"""
TITLE: User name, home directory and login shell
GROUP: System information
USES: import pwd, os, getpass
SUMMARY: pwd.getpwuid(os.getuid()) looks the running user up in the account
  database and returns the name, home directory and login shell. The
  environment variables are the quick route.
NOTES:
  - $USER, $HOME and $SHELL are convenient but can be changed or missing (cron,
    sudo, containers). The pwd entry is the reliable answer.
  - pw_shell and $SHELL are the user's LOGIN shell. The shell you are typing in
    right now may be different: see the shell and terminal page.
  - getpass.getuser() checks the environment first and then pwd: fine for a
    display name.
  - os.getlogin() needs a controlling terminal and often raises OSError: avoid it.
  - pwd is Unix only.
SEE: pydoc pwd, pydoc getpass
"""

import getpass
import os
import pwd

account = pwd.getpwuid(os.getuid())
print("uid           ", os.getuid())
print("pw_name       ", account.pw_name)
print("pw_dir        ", account.pw_dir)
print("pw_shell      ", account.pw_shell)
print("$SHELL        ", os.environ.get("SHELL", "(not set)"))
print("getpass.getuser()", getpass.getuser())
