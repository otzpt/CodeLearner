/*
 * The HTML course's lessons.
 *
 * Like gui/, this course is plain C with no dependency beyond a compiler. It
 * teaches HTML; it does not open a browser or link against one.
 *
 * A terminal cannot render a page. Where a result depends on a browser the
 * lesson quotes what one did, and those values were measured in Chromium 152
 * by script while the module was written.
 *
 * Computed for real when the lesson runs: URL encoding a form, reading UTF-8 bytes as another charset.
 */

#include <stdio.h>
#include <ctype.h>

#include "lessons.h"
#include "ui.h"

#define COUNT(array) (int) (sizeof (array) / sizeof (array)[0])

/* What module 4 decodes: UTF-8 bytes, written out so the source stays ASCII. */
static const char LINE[] = "Jos\xC3\xA9 comeu p\xC3\xA3o em S\xC3\xA3o Jo\xC3\xA3o: 5 \xE2\x82\xAC";

struct Field {
    const char *name;
    const char *value;
};

static void print_encoded(const char *text)
{
    for (; *text != '\0'; text++) {
        unsigned char c = (unsigned char) *text;

        if (isalnum(c) || c == '*' || c == '-' || c == '.' || c == '_') {
            putchar(c);
        } else if (c == ' ') {
            putchar('+');
        } else {
            printf("%%%02X", c);
        }
    }
}

/* The rule a form follows: name=value pairs, and no name means not sent. */
static void print_query(const struct Field *fields, int count)
{
    int printed = 0;

    for (int i = 0; i < count; i++) {
        if (fields[i].name == NULL) {
            continue;
        }
        if (printed++ > 0) {
            putchar('&');
        }
        print_encoded(fields[i].name);
        putchar('=');
        print_encoded(fields[i].value);
    }
}

/* Each byte becomes one character. 0x82 is the only byte in 0x80..0x9F that
 * LINE holds, and windows-1252 maps it to U+201A, not to the same number. */
static void print_as_windows_1252(const char *text)
{
    for (; *text != '\0'; text++) {
        unsigned char c = (unsigned char) *text;

        if (c < 0x80) {
            putchar(c);
        } else if (c == 0x82) {
            fputs("\xE2\x80\x9A", stdout);
        } else {
            putchar(0xC0 | (c >> 6));
            putchar(0x80 | (c & 0x3F));
        }
    }
}

void lesson_01_structure(void)
{
    title("MODULE 1 - STRUCTURE AND MEANING");

    heading("PART 1: the skeleton, and the line that picks a mode");

    printf("  Every page starts the same way:\n\n");

    print_code(
        "<!doctype html>\n"
        "<html lang=\"en\">\n"
        "<head>\n"
        "  <meta charset=\"utf-8\">\n"
        "  <title>My page</title>\n"
        "</head>\n"
        "<body>\n"
        "  <p>Hello.</p>\n"
        "</body>\n"
        "</html>\n"
    );

    printf("  The first line looks like decoration. It selects how the page\n");
    printf("  is laid out. Without it a browser switches to quirks mode,\n");
    printf("  which copies the bugs of 1990s browsers on purpose so that\n");
    printf("  very old pages keep working.\n\n");

    printf("  This page sets a 28 pixel font on the body and has a table in\n");
    printf("  it:\n\n");

    print_code(
        "<body style=\"font-size: 28px\">\n"
        "<p>Paragraph text</p>\n"
        "<table><tr><td>Table text</td></tr></table>\n"
    );

    printf("  Measured in Chromium 152:\n\n");
    printf("    with <!doctype html> first:  mode CSS1Compat, table text 28px\n");
    printf("    without it:                  mode BackCompat, table text 16px\n");
    printf("\n");

    printf("  One missing line changed the layout, and no error appeared\n");
    printf("  anywhere.\n\n");

    wait_enter();
    clear_screen();
    heading("PART 2: HTML repairs mistakes instead of reporting them");

    printf("  A C compiler refuses a program it cannot parse. An HTML parser\n");
    printf("  never refuses anything: the specification says exactly how to\n");
    printf("  repair every mistake, and the page renders anyway. What you\n");
    printf("  get is not always what you wrote.\n\n");

    printf("  A paragraph can only hold inline content, so a <div> inside\n");
    printf("  one cannot exist:\n\n");

    print_code(
        "<p>First line\n"
        "<div>Is this div inside the paragraph?</div>\n"
        "Last line</p>\n"
    );

    printf("  Measured in Chromium 152:\n\n");
    printf("    paragraphs in the finished tree:  2\n");
    printf("    divs inside a paragraph:          0\n");
    printf("\n");

    printf("  The parser closed the paragraph when it reached the div. The\n");
    printf("  </p> at the end had nothing left to close, so it created a\n");
    printf("  new, empty paragraph.\n\n");

    printf("  To see the tree a browser really built, use its Inspect tool\n");
    printf("  on your own page. Reading that tree, rather than your source,\n");
    printf("  is how you debug HTML.\n\n");

    wait_enter();
    clear_screen();
    heading("PART 3: elements say what something is");

    printf("  These elements describe the parts of a page: <header>, <nav>,\n");
    printf("  <main>, <article>, <section>, <aside>, <footer>.\n\n");

    print_code(
        "<body>\n"
        "  <header>\n"
        "    <h1>Site name</h1>\n"
        "    <nav><a href=\"/\">Home</a> <a href=\"/about\">About</a></nav>\n"
        "  </header>\n"
        "  <main>\n"
        "    <article>\n"
        "      <h2>A post</h2>\n"
        "      <p>The text of the post.</p>\n"
        "    </article>\n"
        "  </main>\n"
        "  <footer>Contact details</footer>\n"
        "</body>\n"
    );

    printf("  On screen a <main> and a <div> look the same, plain blocks.\n");
    printf("  The difference is what they tell software: <main> is a\n");
    printf("  landmark a screen reader can jump to, and a <div> is generic.\n\n");

    printf("   - One visible <main> per page.\n");
    printf("   - Pick a heading level by its place in the outline, not by\n");
    printf("     how big it looks. Size is CSS's job.\n");
    printf("   - <strong> means important and <em> means stressed. <b> and\n");
    printf("     <i> only change the look.\n");
    printf("   - Use <div> when there is no meaning to express, only\n");
    printf("     something to style.\n");

    wait_enter();
    clear_screen();
    exercise(1);

    question("You write a <div> inside a <p>. How many elements end up\n"
             "  inside the paragraph? (a number)",
             "0",
             "The parser closes the paragraph when it reaches\n"
             "             the div.");

    question("Which line keeps a page out of quirks mode?",
             "<!doctype html>",
             "It has to be first in the file.");

    question("An <h2> looks too big, so you change it to <h4>. Good idea?\n"
             "  (yes or no)",
             "no",
             "The level describes the outline. Change the size\n"
             "             with CSS.");

    {
        const char *task[] = {
            "Write a file called about.html.",
        };
        const char *expected[] = {
            "The browser tab says: About me",
            "The page shows, in order: a main heading, a paragraph, a",
            "second-level heading saying Hobbies, a paragraph, and a",
            "footer line.",
            "Inspect shows header, main and footer elements, and the",
            "html element has a lang attribute.",
        };
        const char *solution[] = {
            "<!doctype html>",
            "<html lang=\"en\">",
            "<head>",
            "  <meta charset=\"utf-8\">",
            "  <title>About me</title>",
            "</head>",
            "<body>",
            "  <header>",
            "    <h1>Ana Silva</h1>",
            "  </header>",
            "  <main>",
            "    <p>I am learning HTML.</p>",
            "    <section>",
            "      <h2>Hobbies</h2>",
            "      <p>Chess and cycling.</p>",
            "    </section>",
            "  </main>",
            "  <footer>Written by hand.</footer>",
            "</body>",
            "</html>",
        };
        challenge(task, COUNT(task), NULL, 0,
                  expected, COUNT(expected), solution, COUNT(solution));
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    if (!print_summary_pt(1)) {
        printf("   - <!doctype html> selects standards mode. Forget it and the\n");
        printf("     layout rules change silently.\n");
        printf("   - HTML never reports an error. It repairs, and the repair\n");
        printf("     may not be what you meant.\n");
        printf("   - Inspect shows the tree the browser built, which is the one\n");
        printf("     that counts.\n");
        printf("   - Elements describe meaning. Their look belongs to CSS.\n");
    }
    wait_enter();
}

void lesson_02_forms(void)
{
    title("MODULE 2 - FORMS");

    heading("PART 1: a label is more than text next to a box");

    print_code(
        "<input type=\"checkbox\" id=\"news\"> <label for=\"news\">With a label</label>\n"
        "<input type=\"checkbox\"> Without a label, plain text\n"
    );

    printf("  Measured in Chromium 152:\n\n");
    printf("    clicking the first label:   box ticked\n");
    printf("    clicking the plain text:    box not ticked\n");
    printf("\n");

    printf("  The for attribute has to equal the input's id. A screen reader\n");
    printf("  also reads the label when the field gets focus. Without one,\n");
    printf("  the field is announced with no name.\n\n");

    wait_enter();
    clear_screen();
    heading("PART 2: a field with no name is not sent");

    print_code(
        "<form action=\"sent.html\" method=\"get\">\n"
        "  <label>Email <input name=\"email\" value=\"ana@example.com\"></label>\n"
        "  <label>Password <input name=\"password\" type=\"password\" value=\"abc123\"></label>\n"
        "  <label>Nickname <input value=\"no name attribute\"></label>\n"
        "  <button>Send</button>\n"
        "</form>\n"
    );

    printf("  A form sends name=value pairs joined by &, and skips any field\n");
    printf("  with no name. This program can apply that rule for real:\n\n");

    {
        const struct Field fields[] = {
            { "email", "ana@example.com" },
            { "password", "abc123" },
            { NULL, "no name attribute" },
        };

        printf("    ?");
        print_query(fields, COUNT(fields));
        printf("\n\n");
    }

    printf("  Measured in Chromium 152:\n\n");
    printf("    fields the form holds for sending:  email, password\n");
    printf("\n");

    printf("  That shows two traps at once. The third field vanished without\n");
    printf("  any error. And method=\"get\" put the password in the URL, where\n");
    printf("  it stays in the browser history. Anything sensitive uses\n");
    printf("  method=\"post\", and that still needs HTTPS.\n\n");

    wait_enter();
    clear_screen();
    heading("PART 3: validation in the browser is a convenience");

    print_code(
        "<form>\n"
        "  <input name=\"email\" type=\"email\" required>\n"
        "  <button>Submit</button>\n"
        "</form>\n"
    );

    printf("  Measured in Chromium 152:\n\n");
    printf("    field empty:        form.checkValidity() is false\n");
    printf("    field holds abc:    the field's checkValidity() is false\n");
    printf("\n");

    printf("  Add one attribute and every check is switched off, for anyone:\n\n");

    print_code(
        "<form novalidate>\n"
    );

    printf("  Anyone editing the page with DevTools can do the same.\n");
    printf("  Validation attributes help the person filling in the form.\n");
    printf("  They protect nothing: a server has to check everything again.\n\n");

    wait_enter();
    clear_screen();
    heading("PART 4: a button in a form submits it");

    print_code(
        "<form>\n"
        "  <button>Show a hint</button>\n"
        "  <button type=\"button\">Show a hint, safely</button>\n"
        "</form>\n"
    );

    printf("  Measured in Chromium 152:\n\n");
    printf("    the first button's .type:  submit\n");
    printf("\n");

    printf("  A <button> with no type inside a form submits the form when\n");
    printf("  pressed, even though nobody asked it to. Write type=\"button\"\n");
    printf("  on every button that is not meant to send the form.\n\n");

    wait_enter();
    clear_screen();
    exercise(2);

    question("Which attribute on an input decides whether its value is\n"
             "  sent at all?",
             "name",
             "No name, no entry in what is sent.");

    question("Is required enough to guarantee a server receives a value?\n"
             "  (yes or no)",
             "no",
             "It runs in the browser, and novalidate or\n"
             "             DevTools removes it.");

    question("What type does a <button> inside a form have when none is\n"
             "  written?",
             "submit",
             "So it sends the form when pressed.");

    {
        const char *task[] = {
            "Write signup.html with a sign-up form. Use action=\"\" and",
            "method=\"get\" so the result shows in your own address bar.",
        };
        const char *input[] = {
            "an email and an age, typed into your form",
        };
        const char *expected[] = {
            "Clicking the word Email or Age puts the cursor in that",
            "field.",
            "Submitting with the email empty is refused by the",
            "browser.",
            "An age of 200 is refused. The limit is 120.",
            "After a valid submit the address bar contains email= and",
            "age=",
        };
        const char *solution[] = {
            "<!doctype html>",
            "<html lang=\"en\">",
            "<head>",
            "  <meta charset=\"utf-8\">",
            "  <title>Sign up</title>",
            "</head>",
            "<body>",
            "  <form action=\"\" method=\"get\">",
            "    <p><label for=\"email\">Email</label>",
            "       <input id=\"email\" name=\"email\" type=\"email\" required></p>",
            "    <p><label for=\"age\">Age</label>",
            "       <input id=\"age\" name=\"age\" type=\"number\" min=\"0\" max=\"120\"></p>",
            "    <button>Sign up</button>",
            "  </form>",
            "</body>",
            "</html>",
        };
        challenge(task, COUNT(task), input, COUNT(input),
                  expected, COUNT(expected), solution, COUNT(solution));
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    if (!print_summary_pt(2)) {
        printf("   - <label for> makes the text clickable and names the field\n");
        printf("     for screen readers.\n");
        printf("   - A field with no name is not sent, and nothing warns you.\n");
        printf("   - method=\"get\" puts values in the URL. Never use it for\n");
        printf("     passwords.\n");
        printf("   - Browser validation is a convenience. The server decides.\n");
        printf("   - A button in a form submits it unless it says\n");
        printf("     type=\"button\".\n");
    }
    wait_enter();
}

void lesson_03_accessibility(void)
{
    title("MODULE 3 - ACCESSIBILITY");

    heading("PART 1: put the mouse down and press Tab");

    print_code(
        "<button>Real button</button>\n"
        "<a href=\"#top\">Real link</a>\n"
        "<div class=\"fake\">Looks like a button</div>\n"
    );

    printf("  Measured in Chromium 152:\n\n");
    printf("    <button>   tabIndex 0    reached by the Tab key\n");
    printf("    <a href>   tabIndex 0    reached by the Tab key\n");
    printf("    <div>      tabIndex -1   skipped\n");
    printf("\n");

    printf("  A <button> gets keyboard focus, Enter and Space for free. A\n");
    printf("  styled <div> gets none of them. Someone who cannot use a mouse\n");
    printf("  cannot reach it at all. Test your own pages with the Tab key.\n");
    printf("  It takes ten seconds.\n\n");

    wait_enter();
    clear_screen();
    heading("PART 2: ARIA changes the label, not the behaviour");

    printf("  role=\"button\" tells a screen reader that this is a button. It\n");
    printf("  adds no keyboard support:\n\n");

    print_code(
        "<div role=\"button\">A div with role=\"button\"</div>\n"
    );

    printf("  Measured in Chromium 152:\n\n");
    printf("    <div role=\"button\">   tabIndex -1   still skipped\n");
    printf("\n");

    printf("  The first rule of ARIA is to prefer the native element. A\n");
    printf("  <button> needs no role, no tabindex and no key handling, and\n");
    printf("  it cannot be half finished.\n\n");

    wait_enter();
    clear_screen();
    heading("PART 3: text alternatives");

    printf("  When an image cannot load, which also happens on a slow\n");
    printf("  connection, the alt text is what is left:\n\n");

    print_code(
        "<img src=\"missing-photo.png\" alt=\"A cat asleep on a keyboard\">\n"
        "<img src=\"missing-decoration.png\" alt=\"\">\n"
    );

    printf("  Measured in Chromium 152:\n\n");
    printf("    first image:   drawn as its text, 188 pixels wide\n");
    printf("    second image:  0 pixels wide, nothing shown\n");
    printf("\n");

    printf("   - alt says what the image communicates, not the word photo.\n");
    printf("   - A purely decorative image gets alt=\"\", so a screen reader\n");
    printf("     skips it.\n");
    printf("   - With no alt attribute at all, a screen reader may read the\n");
    printf("     file name out loud.\n");

    wait_enter();
    clear_screen();
    heading("PART 4: links and colour");

    printf("  Screen reader users often list every link on a page, so link\n");
    printf("  text has to make sense on its own. A list of \"click here\" says\n");
    printf("  nothing. \"Download the 2026 report\" says where it goes.\n\n");

    printf("  Do not carry meaning in colour alone. A field marked only by a\n");
    printf("  red border says nothing to someone who cannot see the red. Say\n");
    printf("  it in words as well.\n\n");

    wait_enter();
    clear_screen();
    exercise(3);

    question("Can a <div role=\"button\"> be reached with the Tab key? (yes\n"
             "  or no)",
             "no",
             "A role changes what is announced. It adds no\n"
             "             behaviour.");

    question("What should alt be on a decorative image? (one word)",
             "empty",
             "alt=\"\", so the image is skipped.");

    question("Which element do you use for something that performs an\n"
             "  action on the page?",
             "button",
             "Keyboard support comes with it.");

    {
        const char *task[] = {
            "Write card.html with one image, a heading, and an action the",
            "user can trigger.",
        };
        const char *input[] = {
            "only the Tab key, no mouse",
        };
        const char *expected[] = {
            "With the image file missing, its description is visible.",
            "Tab reaches the action, and Enter activates it.",
            "The link in the page does not say click here.",
        };
        const char *solution[] = {
            "<!doctype html>",
            "<html lang=\"en\">",
            "<head>",
            "  <meta charset=\"utf-8\">",
            "  <title>Card</title>",
            "</head>",
            "<body>",
            "  <main>",
            "    <h1>Lisbon trip</h1>",
            "    <img src=\"tram.jpg\" alt=\"A yellow tram climbing a narrow street\">",
            "    <p><a href=\"trip.html\">Read the Lisbon trip notes</a></p>",
            "    <button type=\"button\">Save for later</button>",
            "  </main>",
            "</body>",
            "</html>",
        };
        challenge(task, COUNT(task), input, COUNT(input),
                  expected, COUNT(expected), solution, COUNT(solution));
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    if (!print_summary_pt(3)) {
        printf("   - Test with the Tab key.\n");
        printf("   - Native elements come with behaviour. Styled divs do not.\n");
        printf("   - ARIA changes what is announced and nothing else.\n");
        printf("   - alt describes what the image communicates, and is empty on\n");
        printf("     decoration.\n");
        printf("   - Link text and meaning must survive without sight of the\n");
        printf("     page around them.\n");
    }
    wait_enter();
}

void lesson_04_metadata(void)
{
    title("MODULE 4 - METADATA");

    heading("PART 1: the charset line");

    printf("  A file is bytes. <meta charset> says how to turn them into\n");
    printf("  letters. Saved as UTF-8, this line's first letters take these\n");
    printf("  bytes:\n\n");

    printf("    text:   %s\n", LINE);
    printf("    bytes:  ");
    for (int i = 0; i < 5; i++) {
        printf("%02X ", (unsigned char) LINE[i]);
    }
    printf("\n\n");

    printf("  The accented letter takes two bytes. Now the same bytes, read\n");
    printf("  by a page that declares <meta charset=\"iso-8859-1\">. This\n");
    printf("  program does that decoding for real:\n\n");

    printf("    ");
    print_as_windows_1252(LINE);
    printf("\n\n");

    printf("  Measured in Chromium 152:\n\n");
    printf("    that exact text, from the same bytes, in a page declaring iso-8859-1\n");
    printf("    (browsers treat that label as windows-1252)\n");
    printf("\n");

    printf("  If your terminal is not set to UTF-8, the accented lines above\n");
    printf("  will look wrong for a second reason, and the byte values are\n");
    printf("  the part to trust.\n\n");

    printf("  The browser only looks for this declaration in the first 1024\n");
    printf("  bytes, so it goes first in the <head>. Save your files as\n");
    printf("  UTF-8 and declare UTF-8.\n\n");

    wait_enter();
    clear_screen();
    heading("PART 2: the viewport line");

    printf("  Phones lay a page out as if the screen were about 980 pixels\n");
    printf("  wide and then shrink it, unless the page says otherwise:\n\n");

    print_code(
        "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
    );

    printf("  Measured in Chromium 152, emulating a 375 pixel phone:\n\n");
    printf("    with that line:     page laid out 375 pixels wide\n");
    printf("    without it:         page laid out 980 pixels wide, then shrunk\n");
    printf("\n");

    wait_enter();
    clear_screen();
    heading("PART 3: what is read by other programs");

    print_code(
        "<meta name=\"description\" content=\"One sentence about this page.\">\n"
        "<link rel=\"canonical\" href=\"https://example.com/article\">\n"
        "<meta property=\"og:title\" content=\"Title shown in a link preview\">\n"
        "<meta property=\"og:image\" content=\"https://example.com/preview.png\">\n"
    );

    printf("  Search engines may use the description as the snippet in their\n");
    printf("  results, and messaging apps read the og: tags to draw a link\n");
    printf("  preview. This course cannot run a search engine, so that\n");
    printf("  sentence is stated and not demonstrated.\n\n");

    printf("  What you can check yourself is <title>: it is the text in the\n");
    printf("  browser tab and the default name of a bookmark.\n\n");

    wait_enter();
    clear_screen();
    heading("PART 4: relative links and base");

    printf("  <base href> changes what every relative URL in the page means,\n");
    printf("  and that includes links to a place on the same page:\n\n");

    print_code(
        "<base href=\"https://example.com/\">\n"
        "<a href=\"#notes\">Jump to the notes on this page</a>\n"
    );

    printf("  Measured in Chromium 152:\n\n");
    printf("    that link's real address:  https://example.com/#notes\n");
    printf("\n");

    wait_enter();
    clear_screen();
    exercise(4);

    question("Within how many bytes of the start must the charset be\n"
             "  declared? (a number)",
             "1024",
             "So it goes first in the head.");

    question("Which meta name stops phones laying a page out at desktop\n"
             "  width?",
             "viewport",
             "width=device-width is the part that matters.");

    question("Which element sets the text in the browser tab?",
             "title",
             "It is also the default bookmark name.");

    {
        const char *task[] = {
            "Add a complete head to the about.html from module 1, and put",
            "a name with an accent in the page.",
        };
        const char *expected[] = {
            "The accented name displays correctly.",
            "In your browser's device mode the text is at normal",
            "reading size.",
            "The browser tab shows your title.",
            "The source has a description of one sentence.",
        };
        const char *solution[] = {
            "<head>",
            "  <meta charset=\"utf-8\">",
            "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">",
            "  <title>About me</title>",
            "  <meta name=\"description\" content=\"A page about learning HTML.\">",
            "</head>",
        };
        challenge(task, COUNT(task), NULL, 0,
                  expected, COUNT(expected), solution, COUNT(solution));
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    if (!print_summary_pt(4)) {
        printf("   - <meta charset=\"utf-8\"> first in the head, and files saved\n");
        printf("     as UTF-8.\n");
        printf("   - No viewport tag means a shrunken page on a phone.\n");
        printf("   - description and og: tags are for other programs, not for\n");
        printf("     your page.\n");
        printf("   - <base> rewrites every relative URL, including #anchors.\n");
    }
    wait_enter();
}

void lesson_05_practical(void)
{
    title("MODULE 5 - PRACTICAL PAGES");

    heading("PART 1: tables are for data");

    printf("  A table has a caption, and header cells that say what they\n");
    printf("  head. Layout belongs to CSS.\n\n");

    print_code(
        "<table>\n"
        "  <caption>Trains to Porto</caption>\n"
        "  <thead>\n"
        "    <tr><th scope=\"col\">Leaves</th><th scope=\"col\">Arrives</th></tr>\n"
        "  </thead>\n"
        "  <tbody>\n"
        "    <tr><td>09:00</td><td>12:03</td></tr>\n"
        "  </tbody>\n"
        "</table>\n"
    );

    printf("  <th scope> lets a screen reader announce the heading with each\n");
    printf("  cell.\n\n");

    wait_enter();
    clear_screen();
    heading("PART 2: images should reserve their space");

    printf("  Until an image loads, the browser does not know its size\n");
    printf("  unless you say so. Text below it then jumps down when the\n");
    printf("  image arrives.\n\n");

    print_code(
        "<img src=\"a.jpg\" alt=\"\" width=\"300\" height=\"100\">\n"
        "<img src=\"b.jpg\" alt=\"\">\n"
    );

    printf("  Measured in Chromium 152, with both files missing:\n\n");
    printf("    with width and height:  300 by 100 reserved\n");
    printf("    without them:           0 pixels wide\n");
    printf("\n");

    printf("  That was measured with an empty alt. If an image with alt text\n");
    printf("  fails for good, the browser draws the text instead and ignores\n");
    printf("  the two attributes: 76 by 18 pixels, measured the same way.\n\n");

    printf("  Add loading=\"lazy\" to images far down a long page so that they\n");
    printf("  are only fetched when they are close to being seen.\n\n");

    wait_enter();
    clear_screen();
    heading("PART 3: widgets with no script");

    print_code(
        "<details>\n"
        "  <summary>What does this cost?</summary>\n"
        "  <p>Nothing.</p>\n"
        "</details>\n"
        "<button popovertarget=\"help\">Help</button>\n"
        "<div id=\"help\" popover>This is a popover. Press Escape.</div>\n"
    );

    printf("  Measured in Chromium 152:\n\n");
    printf("    after the button is clicked:  the popover is open\n");
    printf("\n");

    printf("  HTML has interactive elements of its own, and neither of these\n");
    printf("  needs any JavaScript. Before writing a script for something\n");
    printf("  like this, check whether an element already does it. The\n");
    printf("  element comes with keyboard handling that you would otherwise\n");
    printf("  have to write yourself.\n\n");

    wait_enter();
    clear_screen();
    exercise(5);

    question("Which two attributes stop a page jumping when an image\n"
             "  loads? (two words, joined by and)",
             "width and height",
             "The browser reserves the space in advance.");

    question("Which element gives a collapsible section with no script?",
             "details",
             "Its summary child is the part you click.");

    question("Should a table be used to put two columns of text side by\n"
             "  side? (yes or no)",
             "no",
             "Tables are for data. Layout is CSS's job.");

    {
        const char *task[] = {
            "Write recipe.html, a recipe page using only HTML.",
        };
        const char *input[] = {
            "Tab and Enter, to test it",
        };
        const char *expected[] = {
            "One h1, and h2 headings for ingredients and method.",
            "An ingredients list, and numbered steps.",
            "A table of cooking times with a caption and th cells.",
            "An image with alt, width and height.",
            "A details section holding a tip, which opens with the",
            "keyboard.",
        };
        const char *solution[] = {
            "<!doctype html>",
            "<html lang=\"en\">",
            "<head>",
            "  <meta charset=\"utf-8\">",
            "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">",
            "  <title>Pancakes</title>",
            "</head>",
            "<body>",
            "  <main>",
            "    <h1>Pancakes</h1>",
            "    <img src=\"pancakes.jpg\" alt=\"A stack of pancakes\" width=\"400\" height=\"200\">",
            "    <h2>Ingredients</h2>",
            "    <ul><li>Flour</li><li>Milk</li><li>One egg</li></ul>",
            "    <h2>Method</h2>",
            "    <ol><li>Mix.</li><li>Fry.</li></ol>",
            "    <table>",
            "      <caption>Cooking times</caption>",
            "      <tr><th scope=\"col\">Size</th><th scope=\"col\">Minutes per side</th></tr>",
            "      <tr><td>Small</td><td>1</td></tr>",
            "    </table>",
            "    <details><summary>A tip</summary><p>Rest the batter.</p></details>",
            "  </main>",
            "</body>",
            "</html>",
        };
        challenge(task, COUNT(task), input, COUNT(input),
                  expected, COUNT(expected), solution, COUNT(solution));
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    if (!print_summary_pt(5)) {
        printf("   - Tables are for data, with a caption and th scope.\n");
        printf("   - width and height reserve an image's space.\n");
        printf("   - details and popover work with no script at all.\n");
        printf("   - A correct page is mostly choosing the right elements.\n");
    }
    wait_enter();
}
