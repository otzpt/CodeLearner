# Web JavaScript course

5 modules: The DOM, Events, Forms and input, fetch and the network, State, storage and a small app.

A terminal program in plain C, like `gtk/`. It teaches Web JavaScript and never
opens a browser. The challenges are files you write yourself and open in
whatever browser you normally use.

## Running

```bash
make
./webjs-course
```

## How it was verified

A terminal cannot render a page, so where a result depends on a browser the
lesson quotes what one did. Those values were measured in Chromium 152, by
script, while the modules were written. Firefox and Safari were not checked.

CI builds it with `-Wall -Wextra -Werror` and walks every module to its
SUMMARY. `tools/check-teaching-order.py` covers the exercises.
