"""
TITLE: json: JSON encoder and decoder
GROUP: Text and data
LIBRARY: Python standard library (json)
USES: import json
SUMMARY: Converts Python values to JSON text and back. Only dicts, lists,
  strings, numbers, booleans and None survive the trip; JSON object
  keys always become strings.
ENTRY: json.dumps(obj, *, skipkeys=False, ensure_ascii=True,
    check_circular=True, allow_nan=True, cls=None, indent=None,
    separators=None, default=None, sort_keys=False, **kw)
   Serialize obj to a JSON formatted str.
ENTRY: json.dump(obj, fp, *, skipkeys=False, ensure_ascii=True,
    check_circular=True, allow_nan=True, cls=None, indent=None,
    separators=None, default=None, sort_keys=False, **kw)
   Serialize obj as a JSON formatted stream to fp (a
   .write()-supporting file-like object).
ENTRY: json.loads(s, *, cls=None, object_hook=None, parse_float=None,
    parse_int=None, parse_constant=None, object_pairs_hook=None, **kw)
   Deserialize s (a str, bytes or bytearray instance containing a
   JSON document) to a Python object.
ENTRY: json.load(fp, *, cls=None, object_hook=None, parse_float=None,
    parse_int=None, parse_constant=None, object_pairs_hook=None, **kw)
   Deserialize fp (a .read()-supporting file-like object containing a
   JSON document) to a Python object.
ENTRY: json.JSONDecodeError(msg, doc, pos)
   Subclass of ValueError with the following additional properties:
   msg: The unformatted error message doc: The JSON document being
   parsed pos: The start index of doc where parsing failed lineno:
   The line corresponding to pos colno: The column corresponding to
   pos
ENTRY: json.JSONEncoder(*, skipkeys=False, ensure_ascii=True,
    check_circular=True, allow_nan=True, sort_keys=False, indent=None,
    separators=None, default=None)
   Extensible JSON <https://json.org> encoder for Python data
   structures.
ENTRY: json.JSONDecoder(*, object_hook=None, parse_float=None, parse_int=None,
    parse_constant=None, strict=True, object_pairs_hook=None)
   Simple JSON <https://json.org> decoder
NOTES:
  - dumps() returns a str, dump() writes to a file object. loads()
    takes a str, load() a file object.
  - A set, a datetime or bytes is not JSON: pass default=str (or a
    function) to dumps() to convert them.
  - NaN and Infinity are written as NaN and Infinity, which is not
    valid JSON; use allow_nan=False to refuse them.
  - JSON has no tuple: a tuple comes back as a list.
  - Signatures and descriptions are Python's own, read from the
    module's docstrings for the version that runs this page.
    Version notes (added, changed, deprecated) are in the online
    documentation.
SEE: https://docs.python.org/3/library/json.html, pydoc json
"""

import json

info = {"os": "Void", "kernel": "6.18", "cores": 12, "battery": None, "ok": True, 1: "one"}
text = json.dumps(info, indent=2)
print(text)
print("round trip equal:", json.loads(text) == info, "(False: the key 1 came back as '1')")
print("compact for machines:", json.dumps({"a": [1, 2]}, separators=(",", ":")))
print("sorted keys:", json.dumps({"b": 1, "a": 2}, sort_keys=True))
print("non-ASCII:", json.dumps("café"), json.dumps("café", ensure_ascii=False))
try:
    json.loads("{'single': 'quotes'}")
except json.JSONDecodeError as error:
    print("JSONDecodeError:", error.msg, "at line", error.lineno, "column", error.colno)
try:
    json.dumps({"x": {1, 2}})
except TypeError as error:
    print("TypeError:", error)
