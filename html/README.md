# HTML course

5 modules: Structure and meaning, Forms, Accessibility, Metadata, Practical pages.

A terminal program in plain C, like `gui/`. It teaches HTML and never
opens a browser. The challenges are files you write yourself and open in
whatever browser you normally use.

## Running

```bash
make
./html-course
```

## How it was verified

A terminal cannot render a page, so where a result depends on a browser the
lesson quotes what one did. Those values were measured in Chromium 152, by
script, while the modules were written. Firefox and Safari were not checked.

Computed for real when the lesson runs: URL encoding a form, reading UTF-8 bytes as another charset.

CI builds it with `-Wall -Wextra -Werror` and walks every module to its
SUMMARY. `tools/check-teaching-order.py` covers the exercises.
