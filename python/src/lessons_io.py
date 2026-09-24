"""Modules 12 and 13 - the course leaves the process.

Modules 1-11 stayed inside one running process. Module 12 is the first to
touch something outside it -- a real file on disk -- and the language
feature module 9 already pointed toward when it explained why Python has
no destructors: `with`. Every real file operation in module 12 runs inside
a `tempfile.TemporaryDirectory()`, so nothing it writes lands in the
student's own working directory, and the temp directory is itself cleaned
up on the way out even if something inside it raises -- the same guarantee
the module is teaching, used to build the module.

Module 13 goes past the local machine entirely: a real HTTP call through
`urllib.request`, whose response object supports the same `with` block
module 12 just taught.
"""
import json
import os
import tempfile
import urllib.error
import urllib.request

import ui

API_TIMEOUT_SECONDS = 5
POST_URL = "https://jsonplaceholder.typicode.com/posts/1"
MISSING_URL = "https://jsonplaceholder.typicode.com/posts/999999"
BAD_HOST_URL = "https://does-not-exist.invalid/"


def lesson_12_files():
    ui.title("MODULE 12 - FILES AND CONTEXT MANAGERS")

    ui.heading("PART 1: open / read / write / close -- and the trap")

    print("  open() hands back a file object; close() releases the")
    print("  operating system handle behind it:")
    print()
    print('    f = open(path, "w")')
    print('    f.write("hello\\n")')
    print("    f.close()")
    print()

    with tempfile.TemporaryDirectory() as tmpdir:
        manual_path = os.path.join(tmpdir, "manual.txt")

        f = open(manual_path, "w")
        f.write("hello\n")
        f.close()
        print(f"  Running: wrote and closed by hand. f.closed is {f.closed}")

        print()
        print("  The trap: forget the close() call, or let an exception")
        print("  jump from open() to somewhere past close(), and the handle")
        print("  leaks -- there is no line left to run that would free it.")
        print()

        forgotten = open(manual_path)
        print("  Running: opened again, and simply never called .close().")
        print(f"    forgotten.closed is {forgotten.closed}")
        forgotten.close()  # by hand, now, so this course leaves nothing open

        print()
        print("  Same trap, sharper: real work between open() and close()")
        print("  that can raise. If it does, close() is never reached:")
        print()
        print("    f2 = open(path)")
        print('    int("not a number")   # raises before f2.close() below')
        print("    f2.close()")
        print()

        f2 = open(manual_path)
        try:
            int("not a number")
            f2.close()
        except ValueError as e:
            print(f"  Running: caught {type(e).__name__}: {e}")
        print(f"    f2.closed is {f2.closed}  -- close() was never reached")
        f2.close()  # by hand, now, so this course leaves nothing open

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 2: with open(...) as f: -- close(), guaranteed")

    print("  A context manager ties cleanup to the block, not to a line of")
    print("  code you have to remember to write and to actually reach:")
    print()
    print("    with open(path) as f:")
    print("        ...")
    print("    # f.close() already ran, whether the block finished or raised")
    print()

    with tempfile.TemporaryDirectory() as tmpdir:
        with_path = os.path.join(tmpdir, "with.txt")

        with open(with_path, "w") as f3:
            f3.write("fine\n")
        print(f"  Running: block finished normally. f3.closed is {f3.closed}")

        print()
        print("  Now force an exception from inside the block, the same way")
        print("  PART 1's leak did, and check the same thing afterward:")
        print()

        try:
            with open(with_path) as f4:
                raise ValueError("simulated failure inside the with block")
        except ValueError as e:
            print(f"  Running: caught outside the with block: {e}")
        print(f"    f4.closed is {f4.closed}  -- close() ran anyway")
        print()
        print("  Compare that to f2.closed in PART 1: False there, True")
        print("  here. Same shape of failure, different guarantee.")

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 3: reading and writing text")

    print("  .write() puts exactly the characters you give it on disk --")
    print("  no newline is added for you:")
    print()

    with tempfile.TemporaryDirectory() as tmpdir:
        merged_path = os.path.join(tmpdir, "merged.txt")
        with open(merged_path, "w") as f:
            f.write("one")
            f.write("two")
            f.write("three")
        with open(merged_path) as f:
            merged = f.read()
        print(f"  Running: three .write() calls, no \\n -> {merged!r}")
        print("  One line on disk, not three -- the trap this module warned")
        print("  about above, now in text instead of a closed handle.")

        print()
        print("  Writing \\n explicitly keeps the lines apart:")
        print()

        lines_path = os.path.join(tmpdir, "lines.txt")
        with open(lines_path, "w") as f:
            f.write("one\n")
            f.write("two\n")
            f.write("three")  # last line, no trailing newline on purpose

        with open(lines_path) as f:
            all_lines = f.readlines()
        print(f"  Running: .readlines() -> {all_lines!r}")
        print("  Every line keeps its own \\n -- .readlines() never strips")
        print("  it. The last one has none, because .write() never added")
        print("  one there either.")

        print()
        print("  Iterating the file object directly gives the same lines,")
        print("  one at a time, without building the whole list first:")
        print()
        with open(lines_path) as f:
            for line in f:
                print(f"    Running: repr(line) = {line!r}")

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 4: the json module")

    print("  json.dump writes a Python value as JSON straight to an open")
    print("  file; json.load reads it back. Round-tripping a dict:")
    print()
    print("    with open(path, \"w\") as f:")
    print("        json.dump(data, f)")
    print()
    print("    with open(path) as f:")
    print("        data = json.load(f)")
    print()

    with tempfile.TemporaryDirectory() as tmpdir:
        data_path = os.path.join(tmpdir, "data.json")
        original = {"name": "Ada", "age": 36, "topics": ["files", "json"]}

        with open(data_path, "w") as f:
            json.dump(original, f)

        with open(data_path) as f:
            loaded = json.load(f)

        print(f"  Running: wrote {original!r}")
        print(f"  Running: read back {loaded!r}")
        print(f"  loaded == original is {loaded == original}")

    print()
    print("  json.dumps()/json.loads() do the same conversion through a")
    print("  string instead of a file -- for JSON headed over a network")
    print("  socket rather than to disk. The file-based forms above are")
    print("  what you reach for when the data really is a file.")

    ui.wait_enter()
    ui.clear_screen()
    ui.exercise(12)

    ui.question(
        "You call open(path) and never call .close(). Checked right\n"
        "  away afterward, is f.closed True or False?  (answer: True or False)",
        "False",
        "Nothing closes it until you say so -- or until with does it for you.",
    )
    ui.question(
        "Inside a with open(path) as f: block, code raises and the\n"
        "  exception is never caught inside the block. When the block\n"
        "  exits, is the file open or closed?  (answer: open or closed)",
        "closed",
        "with guarantees close() runs on the way out, exception or not.",
    )
    ui.question(
        ".readlines() on a three-line file returns each line with\n"
        "  its trailing newline attached, or stripped?  (answer: attached or stripped)",
        "attached",
        r".readlines() never strips \n -- only a last line lacking one",
    )

    ui.challenge(
        [
            "Read a name from stdin. Write it into a JSON file as",
            '{"name": <the name>} using json.dump. Open that same file',
            "again, read it back with json.load, and print the name.",
        ],
        ["Alice"],
        ["Alice"],
        [
            "import json",
            "",
            "name = input()",
            "",
            'with open("data.json", "w") as f:',
            '    json.dump({"name": name}, f)',
            "",
            'with open("data.json") as f:',
            "    data = json.load(f)",
            "",
            'print(data["name"])',
        ],
    )

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("SUMMARY")

    print("   - open()/.write()/.close() work, but only if close() actually")
    print("     runs -- forgetting it, or raising first, leaks the handle")
    print("   - with open(...) as f: closes the file on the way out,")
    print("     exception or not -- proven above, not just claimed")
    print("   - .readlines() keeps every \\n; .write() never adds one")
    print("   - json.dump/json.load round-trip a value through a real file;")
    print("     json.dumps/loads do the same through a string")
    print()
    print("  Module 9 said garbage collection timing was never a guarantee")
    print("  you could build on. This is the guarantee Python gives you")
    print("  instead, and it took a real file and a real exception to show.")
    ui.wait_enter()


def lesson_13_apis():
    ui.title("MODULE 13 - WORKING WITH APIS")

    ui.heading("PART 1: urlopen, read, and json.loads")

    print("  urllib.request.urlopen(url, timeout=...) opens a real HTTP")
    print("  connection and hands back a response object that behaves like")
    print("  the file objects module 12 already taught -- it even works as")
    print("  a context manager:")
    print()
    print("    with urllib.request.urlopen(url, timeout=5) as response:")
    print("        raw = response.read()          # bytes, not str")
    print('        data = json.loads(raw.decode("utf-8"))')
    print()
    print("  A real call, right now, with a timeout so a dead network")
    print("  can't hang the lesson:")
    print()
    print(f'    urllib.request.urlopen("{POST_URL}", timeout={API_TIMEOUT_SECONDS})')
    print()

    try:
        with urllib.request.urlopen(POST_URL, timeout=API_TIMEOUT_SECONDS) as response:
            raw = response.read()
            print(f"  Running: type(raw) is {type(raw).__name__}")
            post = json.loads(raw.decode("utf-8"))
            print(f'  Running: status {response.status}, title: "{post["title"]}"')
    except urllib.error.URLError:
        print("  Couldn't reach the network just now -- here's what a")
        print("  successful call returns (a real response, captured while")
        print("  building this module, not invented):")
        print()
        print("  Running (fallback): type(raw) is bytes")
        print(
            '  Running (fallback): status 200, title: "sunt aut facere '
            'repellat provident occaecati excepturi optio reprehenderit"'
        )

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 2: the trap -- urlopen RAISES on a 404")

    print("  This is the opposite of JavaScript's fetch. fetch resolves")
    print("  normally on a 404 -- the request reached the server, and the")
    print("  server answered; checking response.ok is the caller's job.")
    print("  urlopen treats any 4xx/5xx status as a raised exception:")
    print()
    print("    try:")
    print("        urllib.request.urlopen(url, timeout=5)")
    print("    except urllib.error.HTTPError as e:")
    print("        ...  # e.code, e.reason")
    print()
    print("  A real request to a URL confirmed to return 404:")
    print()
    print(f'    urllib.request.urlopen("{MISSING_URL}", timeout={API_TIMEOUT_SECONDS})')
    print()

    try:
        with urllib.request.urlopen(MISSING_URL, timeout=API_TIMEOUT_SECONDS) as response:
            print(f"  Running: no exception -- status {response.status}")
    except urllib.error.HTTPError as e:
        print(f"  Running: raised {type(e).__name__}")
        print(f"           e.code   = {e.code}")
        print(f"           e.reason = {e.reason!r}")
        print(f"           str(e)   = {e}")
    except urllib.error.URLError:
        print("  Couldn't reach the network just now -- here's what the")
        print("  real 404 above raised while building this module:")
        print()
        print("  Running (fallback): raised HTTPError")
        print("                      e.code   = 404")
        print("                      e.reason = 'Not Found'")
        print("                      str(e)   = HTTP Error 404: Not Found")

    print()
    print("  Same URL, same library, opposite behaviour from JavaScript's")
    print("  fetch: there, a 404 is a normal Response with .ok == False.")
    print("  Here, it never becomes a value -- it becomes an exception.")

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 3: URLError -- a genuine network failure")

    print("  HTTPError above fired because a server answered with bad")
    print("  news. This one fires because there is no server to answer at")
    print("  all -- a hostname that cannot resolve. HTTPError is actually")
    print("  a subclass of URLError, so the except order matters: HTTPError")
    print("  has to come first, or its own except clause never runs.")
    print()
    print("    try:")
    print(f'        urllib.request.urlopen("{BAD_HOST_URL}", timeout=5)')
    print("    except urllib.error.HTTPError as e:")
    print("        ...  # a bad status -- PART 2")
    print("    except urllib.error.URLError as e:")
    print("        ...  # no server at all -- this part")
    print()

    try:
        with urllib.request.urlopen(BAD_HOST_URL, timeout=API_TIMEOUT_SECONDS):
            print("  Running: no exception -- that host was not supposed to resolve.")
    except urllib.error.HTTPError as e:
        print(f"  Running: raised HTTPError instead ({e.code}) -- unexpected here")
    except urllib.error.URLError as e:
        print(f"  Running: raised {type(e).__name__}: {e}")
        print(f"           e.reason = {e.reason!r}")
        print("           (the exact OS wording varies -- the type, URLError,")
        print("           is what the except clause above actually matches)")

    print()
    print("  A timeout raises the same way -- urlopen wraps it in URLError")
    print("  too, which is the whole reason every call above passed")
    print(f"  timeout={API_TIMEOUT_SECONDS}: a dead connection fails fast")
    print("  instead of hanging the lesson.")

    ui.wait_enter()
    ui.clear_screen()
    ui.exercise(13)

    ui.question(
        "A real GET reaches the server and it answers 404 Not Found.\n"
        "  Does urllib.request.urlopen raise for that?  (answer: yes or no)",
        "yes",
        "Unlike JS's fetch, urlopen treats any 4xx/5xx status as urllib.error.HTTPError.",
    )
    ui.question(
        "Inside urlopen's with block, response.read() -- does it return\n"
        "  str or bytes?  (answer: str or bytes)",
        "bytes",
        "Same as module 12's file reads -- decode() before json.loads, same as json.load did on disk.",
    )
    ui.question(
        "A hostname that can't resolve, a connection refused, or a\n"
        "  timeout -- which exception type covers all three?",
        "urlerror",
        "urllib.error.URLError. HTTPError is a URLError too, for when a server did answer.",
    )

    ui.challenge(
        [
            "Here is what a live urlopen call's response.read().decode()",
            "already gave you, as a real JSON string:",
            "",
            '  raw = \'{"userId": 1, "id": 1, "title": "real API demo"}\'',
            "",
            "Parse it with json.loads and print the title.",
        ],
        [],
        ["real API demo"],
        [
            "import json",
            "",
            'raw = \'{"userId": 1, "id": 1, "title": "real API demo"}\'',
            "post = json.loads(raw)",
            'print(post["title"])',
        ],
    )

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("SUMMARY")

    print("   - urllib.request.urlopen(url, timeout=...) opens a real HTTP")
    print("     connection; the response works like the files module 12")
    print("     taught -- .read() returns bytes, decode() + json.loads()")
    print("     turns that into a dict")
    print("   - the trap: urlopen RAISES urllib.error.HTTPError on a 4xx/5xx")
    print("     status -- the opposite of JS's fetch, which resolves and")
    print("     leaves checking response.ok to the caller")
    print("   - urllib.error.URLError covers a genuine network failure (DNS,")
    print("     refused connection, timeout); HTTPError is a URLError, so it")
    print("     has to be caught first or its except clause never runs")
    print("   - timeout=... on every live call keeps a dead network from")
    print("     hanging the lesson instead of failing silently by waiting")
    ui.wait_enter()
