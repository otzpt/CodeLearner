"""
TITLE: dict: key-value mappings (methods)
GROUP: Built-ins
LIBRARY: Python built-ins (no import needed)
SUMMARY: A dict maps keys to values and keeps insertion order. Keys must be
  hashable (str, int, tuple of those); lists and dicts cannot be
  keys.
ENTRY: dict.get(self, key, default=None, /)
   Return the value for key if key is in the dictionary, else
   default.
ENTRY: dict.setdefault(self, key, default=None, /)
   Insert key with a value of default if key is not in the
   dictionary.
ENTRY: dict.keys(self, /)
   Return a set-like object providing a view on the dict's keys.
ENTRY: dict.values(self, /)
   Return an object providing a view on the dict's values.
ENTRY: dict.items(self, /)
   Return a set-like object providing a view on the dict's items.
ENTRY: dict.update
   D.update([E, ]**F) -> None. Update D from mapping/iterable E and
   F. If E is present and has a .keys() method, then does: for k in
   E.keys(): D[k] = E[k] If E is present and lacks a .keys() method,
   then does: for k, v in E: D[k] = v In either case, this is
   followed by: for k in F: D[k] = F[k]
ENTRY: dict.pop
   D.pop(k[,d]) -> v, remove specified key and return the
   corresponding value.
ENTRY: dict.popitem(self, /)
   Remove and return a (key, value) pair as a 2-tuple.
ENTRY: dict.copy(self, /)
   Return a shallow copy of the dict.
ENTRY: dict.fromkeys(iterable, value=None, /)
   Create a new dictionary with keys from iterable and values set to
   value.
ENTRY: dict.clear(self, /)
   Remove all items from the dict.
NOTES:
  - d[key] raises KeyError for a missing key; d.get(key, default)
    does not.
  - Changing a dict's size while looping over it raises
    RuntimeError; loop over list(d) instead.
  - d.keys(), d.values() and d.items() are live views, not copies.
  - Signatures and descriptions are Python's own, read from the
    module's docstrings for the version that runs this page.
    Version notes (added, changed, deprecated) are in the online
    documentation.
SEE: https://docs.python.org/3/library/stdtypes.html#mapping-types-dict, help(dict)
"""

info = {"os": "Void", "cores": 12}
print("get with default:", info.get("gpu", "(none)"), " get without:", info.get("gpu"))
try:
    info["gpu"]
except KeyError as error:
    print("info['gpu'] raises KeyError:", error)
print("setdefault:", info.setdefault("shell", "bash"), info)
info.update({"cores": 16, "kernel": "6.18"})
print("update:", info)
print("pop with default:", info.pop("nothing", "missing"), " pop:", info.pop("kernel"))
for key, value in info.items():
    print(f"  {key:<6} {value}")
print("keys is a live view:", list(info.keys()))
print("fromkeys:", dict.fromkeys("abc", 0))
