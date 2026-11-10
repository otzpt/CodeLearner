# CSS course

5 modules: The box and normal flow, Flexbox, Grid, Responsive design, Modern CSS and the cascade.

A terminal program in plain C, like `gtk/`. It teaches CSS and never
opens a browser. The challenges are files you write yourself and open in
whatever browser you normally use.

## Running

```bash
make
./css-course
```

## How it was verified

A terminal cannot render a page, so where a result depends on a browser the
lesson quotes what one did. Those values were measured in Chromium 152, by
script, while the modules were written. Firefox and Safari were not checked.

Computed for real when the lesson runs: the arithmetic of box sizes, shares and clamp, and specificity.

CI builds it with `-Wall -Wextra -Werror` and walks every module to its
SUMMARY. `tools/check-teaching-order.py` covers the exercises.
