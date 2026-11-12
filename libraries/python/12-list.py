"""
TITLE: list: mutable sequences (methods)
GROUP: Built-ins
LIBRARY: Python built-ins (no import needed)
SUMMARY: A list is an ordered, mutable sequence. sort(), reverse(),
  append() and the other mutating methods change the list in place
  and return None.
ENTRY: list.append(self, object, /)
   Append object to the end of the list.
ENTRY: list.extend(self, iterable, /)
   Extend list by appending elements from the iterable.
ENTRY: list.insert(self, index, object, /)
   Insert object before index.
ENTRY: list.remove(self, value, /)
   Remove first occurrence of value.
ENTRY: list.pop(self, index=-1, /)
   Remove and return item at index (default last).
ENTRY: list.index(self, value, start=0, stop=9223372036854775807, /)
   Return first index of value.
ENTRY: list.count(self, value, /)
   Return number of occurrences of value.
ENTRY: list.sort(self, /, *, key=None, reverse=False)
   Sort the list in ascending order and return None.
ENTRY: list.reverse(self, /)
   Reverse *IN PLACE*.
ENTRY: list.copy(self, /)
   Return a shallow copy of the list.
ENTRY: list.clear(self, /)
   Remove all items from list.
NOTES:
  - sort() and reverse() return None. `items = items.sort()` sets
    items to None.
  - remove() deletes the first equal item and raises ValueError if
    there is none.
  - b = a makes a second name for the same list; a.copy() or a[:]
    makes a new one (shallow).
  - Signatures and descriptions are Python's own, read from the
    module's docstrings for the version that runs this page.
    Version notes (added, changed, deprecated) are in the online
    documentation.
SEE: https://docs.python.org/3/library/stdtypes.html#list, help(list)
"""

items = [3, 1, 4, 1, 5]
items.append(9)
items.extend([2, 6])
items.insert(0, 0)
print("after append/extend/insert:", items)
print("pop() ->", items.pop(), " pop(0) ->", items.pop(0), " now", items)
items.remove(1)          # removes the FIRST 1 only
print("remove(1):", items, " count(1) =", items.count(1), " index(4) =", items.index(4))
result = items.sort()
print("sort() returns", result, "and sorts in place:", items)
print("sorted() returns a new list:", sorted(items, reverse=True), "original:", items)
alias, clone = items, items.copy()
items.append(99)
print("alias sees the change:", alias[-1] == 99, " copy does not:", clone[-1] != 99)
try:
    [].pop()
except IndexError as error:
    print("pop on an empty list raises IndexError:", error)
