# Security Policy

## Scope

CodeLearner is a set of offline command-line programs. Each course runs
only the code it ships with -- there is no network access, no telemetry, no
account system, and no data collected or transmitted. The realistic
security surface is small:

- A course running arbitrary input through `scanf`/`cin`/`input()`/
  `readline` in a way that could crash it or corrupt memory (relevant
  mainly to the C and C++ courses, which do not have a garbage collector).
- The launcher (`launcher/`) invoking a course binary or script via
  `system()`.
- A future contribution introducing a real vulnerability into the shared
  tooling (`tools/check-teaching-order.py`) or a course's own `ui`
  module.

None of this touches a network, a file outside the repository, or another
user's data. There is no privileged operation anywhere in this codebase.

## What is not in scope

The C and C++ courses' own lessons deliberately explain and sometimes
trigger memory bugs (a segmentation fault, a deliberately unmatched
`malloc`) as teaching material. That is the point of those modules and is
not a vulnerability report -- it is documented in the lesson text itself,
and the course as a whole is verified clean under AddressSanitizer,
UndefinedBehaviorSanitizer, and LeakSanitizer (see
[`docs/writing-a-course.md`](docs/writing-a-course.md)).

## Reporting a vulnerability

If you find something that is a real vulnerability rather than a teaching
example -- a crash triggerable by ordinary use of the course, a bug in the
launcher that could execute something other than the intended course
binary, or similar -- please open a GitHub issue describing it. There is no
dedicated security contact or bug bounty; this is a small educational
project maintained by one person, and issues are the fastest way to reach
them.
