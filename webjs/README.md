# Web JavaScript course

JavaScript in a web page, for someone who has never written one. A terminal
cannot show a page, so each example runs through `learn/web.js`, a small
stand-in for a browser, and shows what the page's script prints with
`console.log`.

## Lessons

1. A script on a page
2. Changing the page
3. Reacting to clicks: events
4. Reading what the visitor types
5. Adding and removing elements

## What the stand-in covers

`document.querySelector`, `querySelectorAll`, `getElementById`,
`createElement`, `textContent`, `innerHTML`, `classList`, attributes,
`appendChild`, `remove`, `addEventListener` with clicks that bubble, and
`input` events. `sim.click(selector)` and `sim.type(selector, text)` play the
user and exist only on the PC. In a real browser, `console.log` output is in
the developer tools (F12).

## Running

From the launcher, or directly:

```bash
node webjs/learn.js
```

Your practice files and progress go in `~/codelearner/webjs/`.

## Checking

```bash
node learn/check.js webjs            # every example, question, solution and starter
python3 learn/check-course.py webjs  # every lesson played to the end
```

