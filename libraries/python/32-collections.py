"""
TITLE: collections: container datatypes
GROUP: Text and data
LIBRARY: Python standard library (collections)
USES: import collections
SUMMARY: Specialised containers: Counter counts things, defaultdict builds
  groups, deque is a fast queue at both ends, namedtuple names the
  fields of a tuple.
ENTRY: collections.Counter(iterable=None, /, **kwds)
   Dict subclass for counting hashable items. Sometimes called a bag
   or multiset. Elements are stored as dictionary keys and their
   counts are stored as dictionary values.
ENTRY: collections.Counter.most_common(self, n=None)
   List the n most common elements and their counts from the most
   common to the least. If n is None, then list all element counts.
ENTRY: collections.defaultdict(default_factory=None, /, [...]) --> dict with
    default factory
   --> dict with default factory
ENTRY: collections.deque
   A list-like sequence optimized for data accesses near its
   endpoints.
ENTRY: collections.namedtuple(typename, field_names, *, rename=False,
    defaults=None, module=None)
   Returns a new subclass of tuple with named fields.
ENTRY: collections.OrderedDict
   Dictionary that remembers insertion order
ENTRY: collections.ChainMap(*maps)
   A ChainMap groups multiple dicts (or other mappings) together to
   create a single, updateable view.
NOTES:
  - A Counter returns 0 for a missing key instead of raising
    KeyError.
  - A defaultdict creates a missing entry on lookup, which can
    grow it by accident: dd['x'] adds 'x'.
  - deque.popleft() is O(1); list.pop(0) is O(n).
  - In new code a dataclass is often clearer than a namedtuple.
  - Signatures and descriptions are Python's own, read from the
    module's docstrings for the version that runs this page.
    Version notes (added, changed, deprecated) are in the online
    documentation.
SEE: https://docs.python.org/3/library/collections.html, pydoc collections
"""

from collections import Counter, defaultdict, deque, namedtuple

counts = Counter("mississippi")
print("Counter:", counts, " most_common(2):", counts.most_common(2), " missing key:", counts["z"])

groups = defaultdict(list)
for word in ("apple", "avocado", "banana", "blueberry", "cherry"):
    groups[word[0]].append(word)
print("defaultdict(list):", dict(groups))

queue = deque([1, 2, 3], maxlen=3)
queue.append(4)
queue.appendleft(0)
print("deque with maxlen=3 after append(4), appendleft(0):", list(queue), " popleft:", queue.popleft())

Point = namedtuple("Point", ["x", "y"])
point = Point(3, y=4)
print("namedtuple:", point, point.x + point.y, point._asdict())
