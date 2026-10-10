"""The C headers the Libraries wiki documents, shared by the two check scripts."""

C_HEADERS = [
    # standard C
    "stdio.h", "stdlib.h", "string.h", "stdbool.h", "stdint.h", "ctype.h", "time.h", "errno.h",
    # POSIX and Linux
    "unistd.h", "sys/utsname.h", "pwd.h", "sys/types.h", "sys/stat.h", "sys/statvfs.h", "dirent.h",
    "sys/sysinfo.h",
    # additional
    "signal.h", "sys/ioctl.h", "termios.h", "fcntl.h", "limits.h",
]
