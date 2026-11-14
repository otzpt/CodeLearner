# HTML course

For someone who has never written a web page. A terminal cannot draw a page,
so each example runs through `learn/web.js`, a small stand-in for a browser
that prints the element tree the browser builds, and warns about tags closed
in the wrong order or never closed.

## Lessons

1. Tags and elements
2. A whole page
3. Nesting and closing tags
4. Links and images
5. Lists and tables
6. Forms: asking the visitor
7. Page structure that means something

## Seeing it drawn

Open your practice file in any browser. The course never opens one.

## Running

From the launcher, or directly:

```bash
node html/learn.js
```

Your practice files and progress go in `~/codelearner/html/`.

## Checking

```bash
node learn/check.js html            # every example, question, solution and starter
python3 learn/check-course.py html  # every lesson played to the end
```

