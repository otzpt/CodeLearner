"""
TITLE: Dicts, Counter and JSON
GROUP: Basics
USES: import collections, json
SUMMARY: A dict is the natural shape for "label -> value" system information.
  Counter counts things; json turns dicts into text and back.
NOTES:
  - dict.get(key, default) avoids KeyError; setdefault and defaultdict build
    groups.
  - Counter("mississippi").most_common(2) gives the commonest items.
  - json.dumps(data, indent=2) for people, json.dumps(data) for machines.
    Only dicts, lists, strings, numbers, booleans and None survive.
  - Outputting machine-readable information (--json) lets other tools use it.
  - Dicts keep insertion order, so the fields print in the order you added
    them.
SEE: pydoc collections.Counter, pydoc json
"""

import json
from collections import Counter

info = {"os": "Ubuntu 26.04", "kernel": "7.0.0", "cores": 12, "battery": None}
print("get with default:", info.get("gpu", "(none)"))

text = json.dumps(info, indent=2)
print(text)
print("round trip equal:", json.loads(text) == info)
print("most common letters in 'mississippi':", Counter("mississippi").most_common(2))
