# Web courses

HTML, CSS and JavaScript for the browser, as three separate courses of five
modules each.

| Course | Folder | Modules |
| --- | --- | --- |
| HTML | [`html/`](html/) | Structure and meaning, forms, accessibility, metadata, practical pages |
| CSS | [`css/`](css/) | The box and normal flow, Flexbox, Grid, responsive design, modern CSS and the cascade |
| Web JavaScript | [`js/`](js/) | The DOM, events, forms and input, `fetch` and the network, state and storage |

These are the one exception to a course being a terminal program, because what
they teach only exists in a page. A course here is a folder of static HTML, with
no build step and no dependency other than a browser.

The HTML and CSS courses contain no JavaScript. Answers are in `<details>`, and
each demo is an `<iframe srcdoc>` showing the same text printed above it.
The Web JavaScript course runs each code box when the page loads, and its
helpers are in [`js/course.js`](js/course.js).

## Running

From the launcher, pick HTML, CSS or Web JavaScript under WEB. Or open the
course's `index.html` yourself, or run:

```bash
./open html
```

`open.bat html` does the same on Windows.

## How it was verified

`node web/check-course.js` compares each demo with its listing, checks that
every module has the usual shape, and checks local links. CI runs it, and
`tools/check-teaching-order.py` covers the exercises.

Neither can see a screen, so each claim was also measured in a browser
(Chromium 152, driven by script at a 1280 by 900 window and at phone width):
computed styles, element sizes and positions, `compatMode`, tab order, the form
fields actually sent, and everything the JavaScript demos print. Doing that
corrected three lessons that had been written from memory, including one where
a broken image with `alt` text turned out to ignore `width` and `height`.

Only Chromium was used. Firefox and Safari have not been checked. These
statements are stated, not demonstrated, and say so where relevant: what search
engines and link previews do with metadata, and how a screen reader announces
things.
