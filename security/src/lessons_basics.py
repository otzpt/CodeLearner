"""Security course, module 1 - the basics.

Written in Python, like the Python course: the source is itself worked
example code. This course teaches security, not the language, so the rule
"written in the language it teaches" is adapted -- the implementation
language is Python and every lesson's code is readable, runnable example
code the student can open.

Same two project rules apply unchanged:
  - Nothing is claimed without being shown. Where a call can run here, it
    runs; its real result is printed, never asserted.
  - Teach the trap, not the syntax. What is worth a module is the thing
    that makes people lose an afternoon or ship a real hole.

Dependencies: standard library only (urllib, stdlib elsewhere), the same
no-runtime-dependencies rule every other course follows. Real security work
often reaches for the third-party `requests` library; urllib does the same
job here without adding a dependency, and the difference is noted in the
module.
"""

import urllib.error
import urllib.request

import ui

# A short timeout so a dead network fails fast instead of hanging the
# lesson -- the single most common reason a first network script appears
# to "freeze" is a request with no timeout against an unreachable host.
HTTP_TIMEOUT_SECONDS = 5

# example.com is run by IANA as a stable, safe demonstration host. It is a
# deliberately boring target: fetching it is as harmless as opening it in a
# browser, and it is the right first target precisely because it is real.
TARGET_URL = "https://example.com"

# The response headers a defender looks for first. Their ABSENCE is the
# finding -- a browser applies each one only if the server sends it, so a
# site that omits them is leaving a protection switched off.
SECURITY_HEADERS = [
    "Strict-Transport-Security",
    "X-Content-Type-Options",
    "Content-Security-Policy",
    "X-Frame-Options",
]


def lesson_01_first_request():
    ui.title("MODULE 1 - YOUR FIRST REQUEST")

    ui.heading("PART 1: a request is two halves -- you send, it answers")

    print("  Every HTTP exchange is a request you send and a response the")
    print("  server sends back. The response is a status line, a block of")
    print("  headers, and then the body. Security lives in the first two")
    print("  as much as the body everyone looks at.")
    print()
    print("  urllib.request.urlopen opens a real connection and hands back")
    print("  a response object. Always pass a timeout:")
    print()
    print("    with urllib.request.urlopen(url, timeout=5) as response:")
    print("        status  = response.status      # the status line")
    print("        headers = response.headers     # the header block")
    print()
    print("  Real security code often uses the third-party `requests`")
    print("  library instead (requests.get(url, timeout=5)). It is nicer,")
    print("  but it is a dependency; urllib is in the standard library and")
    print("  teaches the same request/response shape, so this course uses")
    print("  it and names the difference rather than hiding it.")
    print()
    print(f"  A real call, right now, to {TARGET_URL}:")
    print()

    headers = _live_request()

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 2: the Server header -- information you gave away free")

    print("  The response headers describe the server. The Server header")
    print("  often names the software, and sometimes its exact version:")
    print()
    server = headers.get("Server") if headers else None
    print(f"    Server: {server!r}")
    print()
    print("  This is information disclosure. A version number tells an")
    print("  attacker which known vulnerabilities to try first -- the")
    print("  recon step before any attack is just reading what the target")
    print("  volunteers. Hardened servers trim or remove this header. The")
    print("  trap is thinking recon needs special tools: the first tool is")
    print("  reading the response you already have.")

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 3: the headers that AREN'T there")

    print("  Some security headers protect the user only if the server")
    print("  sends them. A browser cannot enforce a rule it was never")
    print("  given. So the finding is the MISSING one, and you can only")
    print("  see a missing thing by checking for it on purpose:")
    print()
    print("    for name in SECURITY_HEADERS:")
    print("        value = response.headers.get(name)")
    print("        # value is None when the server never sent it")
    print()

    if headers is not None:
        for name in SECURITY_HEADERS:
            value = headers.get(name)
            state = "present" if value else "MISSING"
            print(f"    {name:<28} {state}")
        print()
        print("  Each MISSING line is a real finding on a real, famous site")
        print("  -- not a mistake in this course. Absence is the normal")
        print("  state of the web; the headers are opt-in, and most hosts")
        print("  never opt in.")
    else:
        print("  (No network just now -- see the captured run in PART 1.)")

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("PART 4: the trap -- urlopen RAISES on a 4xx/5xx")

    print("  A 404 or 500 is not an exception in every language. In")
    print("  JavaScript's fetch it is a normal response you must check. In")
    print("  Python's urllib it is raised:")
    print()
    print("    try:")
    print("        with urllib.request.urlopen(url, timeout=5) as r:")
    print("            ...")
    print("    except urllib.error.HTTPError as e:   # a 4xx/5xx answer")
    print("        ...  # e.code, e.reason")
    print("    except urllib.error.URLError as e:    # no answer at all")
    print("        ...  # DNS failure, refused connection, timeout")
    print()
    print("  HTTPError is a subclass of URLError, so it must be caught")
    print("  first or its own clause never runs. A scanner that treats a")
    print("  crash as 'host is down' when the host actually answered 403")
    print("  has just recorded the opposite of the truth -- the trap is")
    print("  letting the exception stand in for the result.")

    ui.wait_enter()
    ui.clear_screen()
    ui.exercise(1)

    ui.question(
        "A server answers 404. Does urllib.request.urlopen return a\n"
        "  response or raise?  (answer: return or raise)",
        "raise",
        "urllib raises urllib.error.HTTPError on any 4xx/5xx -- unlike JS fetch, which returns a response.",
    )
    ui.question(
        "response.headers.get(\"X-Frame-Options\") is None. Did the\n"
        "  server send that header?  (answer: yes or no)",
        "no",
        "None means it was never sent -- and a browser can only enforce a header it received.",
    )
    ui.question(
        "The Server header reveals the exact software version. Is that\n"
        "  helpful to an attacker?  (answer: yes or no)",
        "yes",
        "It points them straight at the known vulnerabilities for that version -- free recon.",
    )

    ui.challenge(
        [
            "Write a script that GETs https://example.com with a 5 second",
            "timeout and prints exactly one line:",
            "",
            "  has-hsts: <yes or no>",
            "",
            "where the answer is yes if the response has a",
            "Strict-Transport-Security header and no otherwise. Catch a",
            "network failure (urllib.error.URLError) and print:",
            "",
            "  network error",
            "",
            "instead of crashing.",
        ],
        [],
        ["has-hsts: no"],
        [
            "import urllib.request",
            "import urllib.error",
            "",
            "url = \"https://example.com\"",
            "try:",
            "    with urllib.request.urlopen(url, timeout=5) as response:",
            "        hsts = response.headers.get(\"Strict-Transport-Security\")",
            "    print(\"has-hsts:\", \"yes\" if hsts else \"no\")",
            "except urllib.error.URLError:",
            "    print(\"network error\")",
        ],
    )

    ui.wait_enter()
    ui.clear_screen()
    ui.heading("SUMMARY")

    print("   - an HTTP response is a status line, headers, and a body;")
    print("     security is read from the first two, not just the body")
    print("   - the Server header is information disclosure -- a version")
    print("     number is the attacker's first recon, given away free")
    print("   - security headers are opt-in; the finding is the MISSING")
    print("     one, visible only because you checked for it on purpose")
    print("   - urllib RAISES on 4xx/5xx (HTTPError, a URLError subclass);")
    print("     never let the exception stand in for the real result")
    print("   - always pass timeout= -- a first network script with no")
    print("     timeout is the classic 'why did it freeze' afternoon")
    ui.wait_enter()


def _live_request():
    """Run the real request, print what came back, and return the headers.

    Returns the response headers on success, or None when the network is
    unreachable. The fallback prints a run captured while building this
    module -- real output, not invented, so the module still demonstrates
    rather than asserts when it is run offline.
    """
    print(f'    urllib.request.urlopen("{TARGET_URL}", timeout={HTTP_TIMEOUT_SECONDS})')
    print()
    try:
        with urllib.request.urlopen(TARGET_URL, timeout=HTTP_TIMEOUT_SECONDS) as response:
            print(f"  Running: status {response.status}")
            print(f"  Running: Server header -> {response.headers.get('Server')!r}")
            return response.headers
    except urllib.error.HTTPError as e:
        # The host answered, with a bad status. Still a real response whose
        # headers we can read -- so hand them back rather than treating this
        # like the host being down.
        print(f"  Running: server answered {e.code} {e.reason}")
        return e.headers
    except urllib.error.URLError:
        print("  Couldn't reach the network just now -- here's a run")
        print("  captured while building this module (real output):")
        print()
        # CAPTURED-FALLBACK: filled from a verified live run. See the capture
        # command in the course README before editing these values.
        for line in _CAPTURED_FALLBACK:
            print(f"  {line}")
        return None


# A real run of PART 1-3 against example.com, captured on 2026-10-23 so the
# offline path shows real output instead of inventing it. Verify/refresh with
# the command in security/README.md ("Verifying") -- header values drift.
_CAPTURED_FALLBACK = [
    "Running (fallback): status 200",
    "Running (fallback): Server header -> 'cloudflare'",
    "Running (fallback): Strict-Transport-Security  MISSING",
    "Running (fallback): X-Content-Type-Options     MISSING",
    "Running (fallback): Content-Security-Policy    MISSING",
    "Running (fallback): X-Frame-Options            MISSING",
]
