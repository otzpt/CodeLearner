# Building a single course, standalone

The release on GitHub ships the universal launcher bundled with all five
courses. If you only want one language -- no launcher, no other four courses
-- here's how to get just that one, and optionally turn it into a real
standalone executable that needs nothing installed.

## The quick way: build it yourself

Every course already builds or runs on its own; the launcher is just a menu
in front of them.

```bash
# C
cd c && make && ./c-course

# C++
cd cpp && make && ./cpp-course

# Python -- no build step
cd python/src && python3 main.py

# JavaScript -- no build step
cd javascript/src && node main.js

# Java -- compiles and runs in one command, no separate javac step
cd java && ./run          # ./run.bat on Windows
```

That's the whole thing for C and C++: one binary, copy it wherever you like.
Python, JavaScript and Java need their own toolchain installed on whatever
machine runs them -- that's not a limitation to work around, it's the
project's own rule (see `docs/writing-a-course.md`): a course has no runtime
dependency beyond the language it teaches.

## Making Python, JavaScript or Java a real standalone executable

If you want one of the interpreted courses to also be a single file that
needs nothing installed -- no separate Python, Node, or JDK on the target
machine -- that needs a real bundler, one per language, and none of them are
part of this repo's own release process. Reasons in short: each is its own
toolchain with its own failure modes, none of them are "add one line to a
Makefile," and this project's whole design is "needs its language's
toolchain" -- bundling the runtime is a deliberate opt-in you're choosing to
do, not something CodeLearner's release does for you.

If you still want it:

**Python -- [PyInstaller](https://pyinstaller.org/):**

```bash
pip install pyinstaller
cd python/src
pyinstaller --onefile --name python-course main.py
# output: dist/python-course(.exe)
```

Must run on the target OS -- PyInstaller does not cross-compile. Building a
Windows .exe requires actually running this on Windows.

**JavaScript -- Node's built-in [Single Executable Applications](https://nodejs.org/api/single-executable-applications.html)**
(no extra install needed, Node 20+):

```bash
cd javascript/src
node --experimental-sea-config sea-config.json   # see Node's SEA docs for the config file shape
node -e "require('fs').copyFileSync(process.execPath, 'javascript-course')"
npx postject javascript-course NODE_SEA_BLOB sea-prep.blob \
    --sentinel-fuse NODE_SEA_FUSE_fce680ab2cc467b6e072b8b5df1996b2
```

Also platform-specific -- build on the OS you're targeting.

**Java -- `jpackage`** (ships with the JDK, `jlink` bundles a minimal JRE so
the result needs no separate Java install):

```bash
cd java
javac -d classes src/*.java
jpackage --input classes --main-jar . --main-class Main \
    --name java-course --type app-image
```

`--type exe` or `--type msi` on Windows, `--type deb`/`--type rpm` on Linux,
produce an installer instead of a raw app-image directory. `jpackage` and
`jlink` are in the JDK but not always symlinked next to `java`/`javac` --
check `$JAVA_HOME/bin/` if they're not on PATH.

## Why this isn't automated in the release workflow

Three different bundlers, each platform-specific, each with its own broken
edge cases, for five courses that already work fine with their language
installed -- that's a lot of CI surface for something most people don't need.
If you build one of these routinely enough to want it automated, that's a
real, reasonable thing to open an issue about.
