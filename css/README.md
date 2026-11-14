# CSS course

For someone who has never written CSS. A terminal cannot draw a page, so
each example runs through `learn/web.js`, a small stand-in for a browser that
prints every element's final style and the rule that won each property.

## Lessons

1. Rules: selector and properties
2. Picking elements: class and id
3. When rules disagree: the cascade
4. Inheritance
5. style attributes and !important
6. Elements inside elements

## What the stand-in covers

Type, `.class`, `#id` and `*` selectors, compounds, descendant and child
combinators, comma lists, `!important`, the `style` attribute, and
inheritance of text properties (links keep their own colour, as in a
browser). It does not lay boxes out, so margins, flexbox and grid can be set
but not seen. Open your file in any browser to see it drawn.

## Running

From the launcher, or directly:

```bash
node css/learn.js
```

Your practice files and progress go in `~/codelearner/css/`.

## Checking

```bash
node learn/check.js css            # every example, question, solution and starter
python3 learn/check-course.py css  # every lesson played to the end
```

