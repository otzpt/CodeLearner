#!/usr/bin/env python3
"""Generate the Python manual's module pages: one page per module.

Each page documents one module, like docs.python.org/3/library/<module>.html:
a signature line per function, class or constant, then what it does. The
signatures and descriptions are read from the running Python (inspect and the
module's own docstrings), so they are the interpreter's own words for the
version that runs the page. Run it again after a Python upgrade:

    python3 libraries/tools/make-python-pages.py

Pages are committed; this script is the reproducible source of the entries.
The EXAMPLES code of each page is written by hand in EXAMPLES below and is run
by check-pages.py with warnings turned into errors.
"""

import importlib
import inspect
import os
import re
import sys
import textwrap

HERE = os.path.dirname(os.path.abspath(__file__))
OUT = os.path.join(os.path.dirname(HERE), "python")
WIDTH = 66
MAX_ENTRIES_CHARS = 21000


def clean_rst(text):
    """Turn reStructuredText from a docstring into plain text."""
    text = re.sub(r":\w+(?::\w+)*:`~?([^`]+)`", r"\1", text)   # :func:`x` -> x
    text = text.replace("``", "").replace("`", "")
    return " ".join(text.split())


def first_paragraph(text):
    """The first paragraph of a docstring, plus the next one when the first
    ends with a colon (it introduces a list that would otherwise be lost)."""
    if not text:
        return ""
    paragraphs = inspect.cleandoc(text).split("\n\n")
    chosen = paragraphs[0]
    if chosen.rstrip().endswith(":") and len(paragraphs) > 1:
        chosen += " " + paragraphs[1]
    return clean_rst(chosen)


def get(path):
    """Resolve 'module.attr.attr' or 'attr' (builtins) to an object."""
    parts = path.split(".")
    for split in range(len(parts), 0, -1):
        try:
            obj = importlib.import_module(".".join(parts[:split]))
        except ImportError:
            continue
        for name in parts[split:]:
            obj = getattr(obj, name)
        return obj
    import builtins
    obj = builtins
    for name in parts:
        obj = getattr(obj, name)
    return obj


def signature_of(label, obj):
    try:
        return "%s%s" % (label, inspect.signature(obj))
    except (ValueError, TypeError):
        doc = inspect.getdoc(obj) or ""
        line = doc.split("\n", 1)[0].strip()
        # many C functions start their docstring with their own signature
        if line.startswith(label.split(".")[-1] + "("):
            return label.rsplit(".", 1)[0] + "." + line if "." in label else line
        return label


def entry(label, desc_override=None):
    obj = get(label)
    if label in CONSTANTS:
        sig = "%s = %s" % (label, CONSTANTS[label][0])
        desc = CONSTANTS[label][1]
    else:
        sig = signature_of(label, obj)
        desc = desc_override or first_paragraph(inspect.getdoc(obj))
        if label in DESCRIPTIONS:
            desc = DESCRIPTIONS[label]
    # a docstring may start by repeating the signature: drop that line
    desc = re.sub(r"^\S+\(.*?\)\s*(->\s*\S+\s*)?", "", desc, count=1) if desc.startswith(label.split(".")[-1] + "(") else desc
    lines = textwrap.wrap(desc, WIDTH) or ["(no description in the module's docstring)"]
    sig_lines = textwrap.wrap(sig, 72, subsequent_indent="    ", break_long_words=False) or [sig]
    body = "\n".join("   " + l for l in lines)
    return "ENTRY: " + sig_lines[0] + "".join("\n" + l for l in sig_lines[1:]) + "\n" + body


# Constants and attributes whose docstring is just their type's: described here.
CONSTANTS = {
    "math.pi": (repr(__import__("math").pi), "The mathematical constant pi, to the precision of a float."),
    "math.e": (repr(__import__("math").e), "The mathematical constant e, the base of natural logarithms."),
    "math.tau": (repr(__import__("math").tau), "The constant tau, 2 * pi."),
    "math.inf": (repr(__import__("math").inf), "A floating-point positive infinity. -math.inf is negative infinity."),
    "math.nan": (repr(__import__("math").nan), "A floating-point 'not a number' value. It is never equal to anything, itself included: test with math.isnan()."),
    "time.CLOCK_BOOTTIME": (repr(__import__("time").CLOCK_BOOTTIME), "A clock id for time.clock_gettime(): seconds since boot, including time the machine was suspended. Linux."),
    "time.CLOCK_MONOTONIC": (repr(__import__("time").CLOCK_MONOTONIC), "A clock id for time.clock_gettime(): a clock that never goes backwards, from an unspecified start. It stops while the machine is suspended on Linux."),
    "sys.argv": ("[...]", "The list of command-line arguments passed to the script. sys.argv[0] is the script name; the rest are the arguments, as strings."),
    "sys.platform": (repr(sys.platform), "A string identifying the platform: 'linux', 'darwin', 'win32'. Compare with startswith('linux') on older Pythons."),
    "sys.version_info": ("sys.version_info(major=..., minor=..., micro=..., releaselevel=..., serial=...)", "The version of the running interpreter as a named tuple. Compare tuples: sys.version_info >= (3, 10)."),
    "sys.version": (repr(sys.version.split()[0] + " ..."), "A string with the interpreter's version number and build information."),
    "sys.path": ("[...]", "The list of directories searched for modules to import. The first entry is the script's own directory."),
    "sys.stdin": ("<_io.TextIOWrapper ...>", "The file object for standard input."),
    "sys.stdout": ("<_io.TextIOWrapper ...>", "The file object for standard output. print() writes here. Its isatty() tells whether it is a terminal."),
    "sys.stderr": ("<_io.TextIOWrapper ...>", "The file object for standard error, for messages that must not be mixed into piped output."),
    "sys.executable": ("'/usr/bin/python3'", "The absolute path of the running Python interpreter."),
    "sys.byteorder": (repr(sys.byteorder), "'little' or 'big': the byte order of this machine."),
    "sys.maxsize": (repr(sys.maxsize), "The largest value a Py_ssize_t can hold: the largest list index plus one on this build."),
    "os.environ": ("environ({...})", "A mapping of the environment variables, as strings. Read with os.environ['HOME'] (KeyError if missing) or os.environ.get('HOME', default). Changing it affects child processes."),
    "os.name": (repr(os.name), "The operating system module in use: 'posix', 'nt' or 'java'."),
    "subprocess.PIPE": (repr(__import__("subprocess").PIPE), "A value for stdin, stdout or stderr: create a pipe to the child that the parent can read or write."),
    "subprocess.DEVNULL": (repr(__import__("subprocess").DEVNULL), "A value for stdin, stdout or stderr: send the stream to os.devnull, discarding it."),
    "locale.LC_ALL": (repr(__import__("locale").LC_ALL), "A locale category for setlocale(): all categories at once."),
    "socket.AF_INET": ("<AddressFamily.AF_INET: 2>", "The IPv4 address family, for socket.socket()."),
    "socket.SOCK_STREAM": ("<SocketKind.SOCK_STREAM: 1>", "A stream (TCP) socket type, for socket.socket()."),
    "socket.SOCK_DGRAM": ("<SocketKind.SOCK_DGRAM: 2>", "A datagram (UDP) socket type, for socket.socket()."),
    "re.IGNORECASE": ("re.IGNORECASE", "A flag: match letters regardless of case. Short form re.I."),
    "re.MULTILINE": ("re.MULTILINE", "A flag: ^ and $ match at the start and end of every line, not only of the whole string. Short form re.M."),
    "re.DOTALL": ("re.DOTALL", "A flag: . also matches a newline. Short form re.S."),
    "termios.TIOCGWINSZ": (hex(__import__("termios").TIOCGWINSZ), "The ioctl request that asks a terminal for its size, for fcntl.ioctl(). The value is this system's; read it from this module, do not type the number."),
    "termios.ECHO": (hex(__import__("termios").ECHO), "A local-mode flag: characters typed are echoed back to the screen. Clear it (for passwords) in the lflag word of tcgetattr()."),
    "termios.ICANON": (hex(__import__("termios").ICANON), "A local-mode flag: canonical (line-at-a-time) input with line editing. Clear it to read single keys."),
    "termios.TCSANOW": (str(__import__("termios").TCSANOW), "A 'when' argument for tcsetattr(): apply the change immediately. TCSADRAIN waits for output to drain, TCSAFLUSH also discards unread input."),
}
for _name in ("ENOENT", "EACCES", "EEXIST", "EINVAL", "EPERM", "EAGAIN", "EINTR", "ENOMEM", "EPIPE", "ENOTDIR", "EISDIR", "ENOSPC", "EBADF", "ECHILD", "ETIMEDOUT", "ECONNREFUSED"):
    _errno = __import__("errno")
    CONSTANTS["errno." + _name] = (str(getattr(_errno, _name)), os.strerror(getattr(_errno, _name)) + ". The matching exception is " + (type(OSError(getattr(_errno, _name), "")).__name__) + ".")

DESCRIPTIONS = {}

# ---------------------------------------------------------------------------
# One row per page: file name, TITLE, GROUP, module docs URL, one-paragraph
# description (DESCRIPTION), names to document, example code.
# ---------------------------------------------------------------------------
PAGES = []


def page(number, slug, title, group, summary, names, example, see=None, notes=()):
    PAGES.append(dict(file="%s-%s.py" % (number, slug), title=title, group=group, summary=summary,
                      names=names, example=example, see=see, notes=notes))


G_BUILTIN, G_FILES, G_TEXT, G_NUM, G_SYS = "Built-ins", "Files, paths and processes", "Text and data", "Numbers and time", "System information"
exec(open(os.path.join(HERE, "python-pages.txt"), encoding="utf-8").read())


def build(spec):
    seen = set()
    entries = []
    for name in spec["names"]:
        if isinstance(name, tuple):   # a handwritten (signature, description) entry
            lines = textwrap.wrap(name[1], WIDTH)
            entries.append("ENTRY: " + name[0] + "\n" + "\n".join("   " + l for l in lines))
            continue
        if name in seen:
            continue
        seen.add(name)
        entries.append(entry(name))
    head = ['"""', "TITLE: " + spec["title"], "GROUP: " + spec["group"]]
    modname = spec["file"].split("-", 1)[1][:-3]
    head.append("LIBRARY: Python standard library (" + modname + ")" if spec["group"] != G_BUILTIN else "LIBRARY: Python built-ins (no import needed)")
    if spec["group"] != G_BUILTIN:
        head.append("USES: import " + modname.split(".")[0] + ("" if "." not in modname else "   # then " + modname))
    summary = textwrap.wrap(spec["summary"], 66)
    head.append("SUMMARY: " + summary[0] + "".join("\n  " + l for l in summary[1:]))
    head.extend(entries)
    head.append("NOTES:")
    for note in list(spec["notes"]) + ["Signatures and descriptions are Python's own, read from the module's docstrings for the version that runs this page. Version notes (added, changed, deprecated) are in the online documentation."]:
        wrapped = textwrap.wrap(note, 62)
        head.append("  - " + wrapped[0])
        head.extend("    " + l for l in wrapped[1:])
    see = spec["see"] or "https://docs.python.org/3/library/%s.html, pydoc %s" % (modname, modname)
    head.append("SEE: " + see)
    head.append('"""')
    text = "\n".join(head)
    if len("\n".join(entries)) > MAX_ENTRIES_CHARS:
        sys.exit("%s: entries too long for the browser (%d chars)" % (spec["file"], len("\n".join(entries))))
    return text + "\n\n" + spec["example"].strip("\n") + "\n"


def main():
    only = sys.argv[1:]
    os.makedirs(OUT, exist_ok=True)
    for spec in PAGES:
        if only and not any(o in spec["file"] for o in only):
            continue
        path = os.path.join(OUT, spec["file"])
        with open(path, "w", encoding="utf-8") as handle:
            handle.write(build(spec))
        print("wrote", spec["file"])


if __name__ == "__main__":
    main()
