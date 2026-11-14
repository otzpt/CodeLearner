# Security course

Security fundamentals for someone who has never programmed, with examples in
Python. Each lesson runs a common mistake, then the fix. Everything runs
offline, on your own machine.

## Lessons

1. Never store a password as it was typed
2. Salt and slow hashing
3. Random values attackers cannot guess
4. SQL injection
5. Command injection
6. Path traversal
7. Putting user text into a web page

## What runs

The unsafe versions are real but harmless: the SQL injection runs against a
database in memory, the command injection only ever runs `echo`, and the path
traversal reads files the example itself made in a temporary folder. Needs
Python 3, standard library only. Lesson 5 uses `echo` and `wc`, which Windows
does not have.

## Running

From the launcher, or directly:

```bash
node security/learn.js
```

Your practice files and progress go in `~/codelearner/security/`.

## Checking

```bash
node learn/check.js security            # every example, question, solution and starter
python3 learn/check-course.py security  # every lesson played to the end
```

