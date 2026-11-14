# Security Policy

## Scope

CodeLearner is a set of command-line courses. Each runs the examples it
ships with and the practice files the student writes, with that language's
own toolchain, on the student's machine. There is no telemetry and no
account system. The only network request is the launcher's `[u] Check for
updates` (one request to the GitHub releases API, only when you pick it).
The realistic security surface is small:

- The launcher (`launcher/`) starting a course with `node` via `system()`,
  and the lesson engine (`learn/`) running examples and the student's own
  practice files with that language's toolchain.
- A future contribution introducing a real vulnerability into the shared
  tooling (`learn/`) or a course's `course.js`.

Apart from the practice folder, `~/codelearner/`, where exercises and
progress are saved, none of this touches a file outside the install or
another user's data, and the only outbound request is the one above. There is no
privileged operation anywhere in this codebase.

## What is not in scope

Some lessons deliberately show unsafe code running, as teaching material:
the C course's crashes and AddressSanitizer reports, and the Security
course's SQL injection, command injection (it only ever runs `echo`) and
path traversal, each followed by the fix. That is the point of those
lessons and is not a vulnerability report.

## Reporting a vulnerability

If you find something that is a real vulnerability rather than a teaching
example -- a crash triggerable by ordinary use of the course, a bug in the
launcher or the lesson engine that could run something other than the
intended course or practice file, or similar -- please open a GitHub issue describing it. There is no
dedicated security contact or bug bounty; this is a small educational
project maintained by one person, and issues are the fastest way to reach
them.
