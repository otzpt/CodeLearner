"""
TITLE: sorted(), list.sort(): sorting
GROUP: Common tasks
USES: built-in sorted(), list.sort(); operator.itemgetter
SUMMARY: sorted() returns a new sorted list from anything iterable.
  list.sort() sorts a list where it is. A key function says what to compare.
SIGNATURE:
  sorted(iterable, key=None, reverse=False)
  list.sort(key=None, reverse=False)
  operator.itemgetter(item)
PARAMETERS:
  - iterable: the things to sort.
  - key: a function applied to each element, whose result is compared.
  - reverse: True for largest first.
  - item: the index or dict key to pick out.
RETURNS: sorted returns a new list. list.sort returns None, and changes the
  list. itemgetter returns a function.
ERRORS:
  - TypeError when two elements cannot be compared, such as 1 and "a".
NOTES:
  - Do not write x = x.sort(): list.sort returns None, so x is lost.
  - Strings sort by code point, so "Zoe" comes before "ana". Use key=str.casefold
    to ignore case.
  - Sorting is stable: equal elements keep their order. Sort by the second
    field first and the first field last to sort by both.
  - key=lambda p: (p["age"], p["name"]) sorts by a tuple, in one pass.
USEDFOR:
  - Sorting records by a field, and file names for a stable listing.
SEE: pydoc sorted, https://docs.python.org/3/howto/sorting.html
"""

from operator import itemgetter

numbers = [42, -7, 19, 3]
print("sorted copy:", sorted(numbers))
print("original   :", numbers)

numbers.sort(reverse=True)
print("sorted in place, largest first:", numbers)

names = ["pear", "Zoe", "apple"]
print("by code point:", sorted(names))
print("ignoring case:", sorted(names, key=str.casefold))

people = [{"name": "Ana", "age": 30}, {"name": "Rui", "age": 25}, {"name": "Eva", "age": 41}]
print("by age:", [person["name"] for person in sorted(people, key=itemgetter("age"))])

try:
    sorted([1, "a"])
except TypeError as error:
    print("TypeError:", error)
