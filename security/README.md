# Security course

A topic course, not a language course: it teaches security fundamentals,
implemented in Python. The language courses are written in the language they
teach; this one is written in Python because that is its implementation
language, and every lesson's code is readable, runnable example code.

One module so far (BASIC). INTERMEDIATE and ADVANCED are printed empty in the
menu, the way the GUI course does, so the planned arc stays visible.

| # | Module | Teaches |
| --- | --- | --- |
| 1 | Your first request | HTTP request/response, the Server header as information disclosure, missing security headers as a finding, and the urllib 4xx/5xx-raises trap |

## Running

```bash
cd security/src
python3 main.py          # or ./main.py
```

Standard library only, the same no-runtime-dependencies rule the other
courses follow. Module 1 makes one real HTTPS GET to `https://example.com`
(a stable demonstration host run by IANA) with a 5 second timeout; offline,
it prints a captured real run instead. Nothing is scanned, attacked, or sent
anywhere -- it reads the response the same way a browser would.

`requests` vs `urllib`: real security code often uses the third-party
`requests` library. urllib is in the standard library and teaches the same
request/response shape, so the course uses it and names the difference in the
module rather than adding a dependency.

## Verifying

```bash
cd security/src
python3 -c "import ast; ast.parse(open('lessons_basics.py').read())"
python3 main.py                       # walk module 1 to its SUMMARY
python3 ../tools/check-teaching-order.py
```

The offline fallback in `lessons_basics.py` (`_CAPTURED_FALLBACK`) must be a
real run, not invented. Refresh it from an online machine with:

```bash
python3 - <<'PY'
import urllib.request
H = ["Strict-Transport-Security","X-Content-Type-Options",
     "Content-Security-Policy","X-Frame-Options"]
with urllib.request.urlopen("https://example.com", timeout=5) as r:
    print("status", r.status)
    print("Server ->", repr(r.headers.get("Server")))
    for h in H:
        print(f"{h}: {'present' if r.headers.get(h) else 'MISSING'}")
PY
```

Paste the real values into `_CAPTURED_FALLBACK`. Header values drift, so this
is a periodic check, not a one-time one.
