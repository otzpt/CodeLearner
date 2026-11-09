/*
 * The CSS course's lessons.
 *
 * Like gui/, this course is plain C with no dependency beyond a compiler. It
 * teaches CSS; it does not open a browser or link against one.
 *
 * A terminal cannot render a page. Where a result depends on a browser the
 * lesson quotes what one did, and those values were measured in Chromium 152
 * by script while the module was written.
 *
 * Computed for real when the lesson runs: the arithmetic of box sizes, shares and clamp, and specificity.
 */

#include <stdio.h>

#include "lessons.h"
#include "ui.h"

#define COUNT(array) (int) (sizeof (array) / sizeof (array)[0])

/* Module 5's three columns: ids, classes, elements. */
struct Specificity {
    const char *selector;
    int ids;
    int classes;
    int elements;
};

/* Compared left to right, and a column never carries into the next: that is
 * why eleven classes lose to one id. */
static int compare(const struct Specificity *a, const struct Specificity *b)
{
    if (a->ids != b->ids) {
        return a->ids - b->ids;
    }
    if (a->classes != b->classes) {
        return a->classes - b->classes;
    }
    return a->elements - b->elements;
}

static void print_winner(const struct Specificity *a, const struct Specificity *b)
{
    printf("    %-24s  (%d,%d,%d)\n", a->selector, a->ids, a->classes, a->elements);
    printf("    %-24s  (%d,%d,%d)\n", b->selector, b->ids, b->classes, b->elements);
    printf("    wins: %s\n\n", compare(a, b) > 0 ? a->selector : b->selector);
}

void lesson_01_layout(void)
{
    title("MODULE 1 - THE BOX AND NORMAL FLOW");

    heading("PART 1: width does not mean what you expect");

    printf("  By default width sets the width of the content only. Padding\n");
    printf("  and border are added on top.\n\n");

    print_code(
        "div { width: 200px; padding: 20px; border: 5px solid steelblue; }\n"
        ".fixed { box-sizing: border-box; }\n"
    );

    {
        int width = 200;
        int padding = 20;
        int border = 5;

        printf("    default:     %d + 2 * %d + 2 * %d = %d pixels across\n",
               width, padding, border, width + 2 * padding + 2 * border);
        printf("    border-box:  %d pixels across, content squeezed to %d\n\n",
               width, width - 2 * padding - 2 * border);
    }

    printf("  Measured in Chromium 152:\n\n");
    printf("    offsetWidth of the two boxes:  250 and 200\n");
    printf("\n");

    printf("  Most stylesheets start by making every element behave like the\n");
    printf("  second box: * { box-sizing: border-box; }\n\n");

    wait_enter();
    clear_screen();
    heading("PART 2: vertical margins collapse");

    printf("  The first paragraph has a bottom margin of 40 pixels and the\n");
    printf("  second a top margin of 20. You might expect a gap of 60.\n\n");

    print_code(
        ".a { margin-bottom: 40px; }\n"
        ".b { margin-top: 20px; }\n"
    );

    {
        int below = 40;
        int above = 20;

        printf("    the gap is the larger margin: %d\n\n", below > above ? below : above);
    }

    printf("  Measured in Chromium 152:\n\n");
    printf("    gap between the two paragraphs:  40 pixels\n");
    printf("\n");

    printf("  Touching vertical margins merge, and the larger one wins.\n");
    printf("  Horizontal margins never collapse, and neither do the margins\n");
    printf("  of flex or grid items. That is often why a gap changes when\n");
    printf("  the parent becomes display: flex.\n\n");

    wait_enter();
    clear_screen();
    heading("PART 3: block and inline");

    printf("  A block takes the whole line and honours width. An inline\n");
    printf("  element flows inside text and ignores it:\n\n");

    print_code(
        "span { width: 300px; }\n"
        ".box { display: inline-block; }\n"
        "\n"
        "<p><span>inline</span></p>\n"
        "<p><span class=\"box\">inline-block</span></p>\n"
    );

    printf("  Measured in Chromium 152:\n\n");
    printf("    inline:        36 pixels wide, the width of its text\n");
    printf("    inline-block:  300 pixels wide\n");
    printf("\n");

    wait_enter();
    clear_screen();
    heading("PART 4: absolute is relative to something");

    printf("  An absolutely positioned element is placed against its nearest\n");
    printf("  ancestor that has a position of its own. If there is none, it\n");
    printf("  is placed against the whole page.\n\n");

    print_code(
        ".relative { position: relative; }\n"
        ".badge { position: absolute; top: 0; right: 0; }\n"
        "\n"
        "<div class=\"relative\">...<span class=\"badge\">A</span></div>\n"
        "<div>...<span class=\"badge\">B</span></div>\n"
    );

    printf("  Measured in Chromium 152, in a body with a 32 pixel margin:\n\n");
    printf("    badge A:  32 pixels from the top and right, the corner of its box\n");
    printf("    badge B:  0 pixels from the top and right, the corner of the page\n");
    printf("\n");

    printf("  z-index has a similar condition: it does nothing on an element\n");
    printf("  with no position.\n\n");

    print_code(
        ".a { z-index: 99; }           /* no position */\n"
        ".b { position: relative; }    /* overlaps .a, comes later */\n"
    );

    printf("  Measured in Chromium 152:\n\n");
    printf("    a with no position:         b is on top, 99 ignored\n");
    printf("    a with position: relative:  a is on top\n");
    printf("\n");

    wait_enter();
    clear_screen();
    exercise(1);

    question("width: 100px; padding: 10px; with the default box-sizing.\n"
             "  How many pixels across? (a number)",
             "120",
             "Padding is added on both sides.");

    question("Margins of 30px and 10px meet vertically. How many pixels is\n"
             "  the gap? (a number)",
             "30",
             "They collapse, and the larger one wins.");

    question("Which property must a parent have so that an absolute child\n"
             "  is placed inside it?",
             "position",
             "Usually position: relative.");

    {
        const char *task[] = {
            "Make a file with one box that has a 4 pixel border and 16",
            "pixels of padding.",
        };
        const char *expected[] = {
            "The whole box, border included, is exactly 300 pixels",
            "across. Check it with your browser's Inspect tool.",
            "A small label sits in its top-right corner, inside the",
            "box.",
        };
        const char *solution[] = {
            "<style>",
            "  .card {",
            "    box-sizing: border-box;",
            "    width: 300px;",
            "    padding: 16px;",
            "    border: 4px solid steelblue;",
            "    position: relative;",
            "  }",
            "  .label { position: absolute; top: 0; right: 0; }",
            "</style>",
            "<div class=\"card\">Content<span class=\"label\">new</span></div>",
        };
        challenge(task, COUNT(task), NULL, 0,
                  expected, COUNT(expected), solution, COUNT(solution));
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    if (!print_summary_pt(1)) {
        printf("   - box-sizing: border-box makes width mean what you expect.\n");
        printf("   - Vertical margins collapse. Horizontal ones and flex items\n");
        printf("     do not.\n");
        printf("   - Inline elements ignore width.\n");
        printf("   - position: absolute needs a positioned ancestor, and\n");
        printf("     z-index needs a position.\n");
    }
    wait_enter();
}

void lesson_02_flexbox(void)
{
    title("MODULE 2 - FLEXBOX");

    heading("PART 1: two axes, and they swap");

    printf("  justify-content works along the main axis and align-items\n");
    printf("  across it. Which is which depends on the direction, and that\n");
    printf("  is the mistake everyone makes first:\n\n");

    print_code(
        ".box { display: flex; justify-content: center; height: 80px; }\n"
        ".column { flex-direction: column; }\n"
    );

    printf("  Measured in Chromium 152, in a box 603 pixels wide:\n\n");
    printf("    row:     first item 276 pixels from the left, 0 from the top\n");
    printf("    column:  first item 0 pixels from the left, 20 from the top\n");
    printf("\n");

    printf("  The same justify-content centres left to right in a row, and\n");
    printf("  top to bottom in a column.\n\n");

    wait_enter();
    clear_screen();
    heading("PART 2: sharing out the space");

    printf("  flex: 1 means take an equal share of what is left. gap puts\n");
    printf("  space between items without any margin on the outer edges.\n");
    printf("  margin-left: auto on one item pushes it away from the others.\n\n");

    print_code(
        ".row { display: flex; gap: 8px; }\n"
        ".share div { flex: 1; }\n"
        ".push { margin-left: auto; }\n"
    );

    {
        int container = 603;
        int gap = 8;
        int items = 3;

        printf("    (%d - %d gaps of %d) / %d items = %.2f pixels each\n\n",
               container, items - 1, gap, items, (container - (items - 1) * gap) / (double) items);
    }

    printf("  Measured in Chromium 152 (offsetWidth rounds to whole pixels):\n\n");
    printf("    the three items:  196, 196 and 196 pixels\n");
    printf("\n");

    wait_enter();
    clear_screen();
    heading("PART 3: the item that refuses to shrink");

    printf("  A flex item will not shrink below the size of its content\n");
    printf("  unless you say so. One long word is enough to break a layout:\n\n");

    print_code(
        ".row { display: flex; width: 300px; }\n"
        ".row div { flex: 1; }\n"
        ".fixed div { min-width: 0; overflow-wrap: anywhere; }\n"
    );

    printf("  Measured in Chromium 152, with one very long word in a 300 pixel row:\n\n");
    printf("    plain row:  content is 394 pixels wide, it overflows\n");
    printf("    .fixed row: content is 300 pixels wide, the word wraps\n");
    printf("\n");

    printf("  The default is min-width: auto, which for a flex item means no\n");
    printf("  smaller than its content. This is the usual cause of a page\n");
    printf("  that scrolls sideways for no visible reason.\n\n");

    wait_enter();
    clear_screen();
    heading("PART 4: wrapping");

    printf("  Flex items stay on one line and squeeze, unless you allow\n");
    printf("  wrapping:\n\n");

    print_code(
        ".row { display: flex; gap: 4px; width: 300px; }\n"
        ".row div { width: 80px; }\n"
        ".wrap { flex-wrap: wrap; }\n"
    );

    printf("  Measured in Chromium 152, six items each asking for 80 pixels:\n\n");
    printf("    no wrap:  all six on one row, squeezed to 47 pixels\n");
    printf("    wrap:     three per row, each 80 pixels\n");
    printf("\n");

    wait_enter();
    clear_screen();
    exercise(2);

    question("With flex-direction: column, which property centres items\n"
             "  horizontally?",
             "align-items",
             "The axes swap with the direction.");

    question("What does min-width default to on a flex item? (one word)",
             "auto",
             "It will not go below its content's size.");

    question("Which property adds space between items but not on the outer\n"
             "  edges?",
             "gap",
             "No margin arithmetic needed.");

    {
        const char *task[] = {
            "Build a navigation bar using Flexbox.",
        };
        const char *input[] = {
            "resize the window narrower and wider",
        };
        const char *expected[] = {
            "A logo on the left, two links after it, and a sign-in",
            "link at the far right.",
            "There is space between the items but none before the",
            "first.",
            "The logo is vertically centred against the links.",
        };
        const char *solution[] = {
            "<style>",
            "  nav { display: flex; align-items: center; gap: 16px; }",
            "  .logo { font-size: 2rem; }",
            "  .signin { margin-left: auto; }",
            "</style>",
            "<nav>",
            "  <span class=\"logo\">Logo</span>",
            "  <a href=\"#a\">Home</a>",
            "  <a href=\"#b\">About</a>",
            "  <a class=\"signin\" href=\"#c\">Sign in</a>",
            "</nav>",
        };
        challenge(task, COUNT(task), input, COUNT(input),
                  expected, COUNT(expected), solution, COUNT(solution));
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    if (!print_summary_pt(2)) {
        printf("   - justify-content follows the main axis, and flex-direction\n");
        printf("     decides which axis that is.\n");
        printf("   - flex: 1 shares the remaining space.\n");
        printf("   - Items refuse to shrink below their content until you set\n");
        printf("     min-width: 0.\n");
        printf("   - flex-wrap: wrap lets items move to a new line.\n");
    }
    wait_enter();
}

void lesson_03_grid(void)
{
    title("MODULE 3 - GRID");

    heading("PART 1: rows and columns");

    printf("  Flexbox arranges along one line. Grid sets out rows and\n");
    printf("  columns together. The fr unit is a share of the free space:\n\n");

    print_code(
        ".grid { display: grid; grid-template-columns: 1fr 2fr; gap: 8px; }\n"
    );

    {
        double free_space = 603 - 8;

        printf("    %.0f pixels free, 3 shares: %.3f and %.3f\n\n",
               free_space, free_space / 3, free_space / 3 * 2);
    }

    printf("  Measured in Chromium 152:\n\n");
    printf("    the two columns:  198.328 and 396.672 pixels\n");
    printf("\n");

    printf("  The small difference is rounding: both measured values are\n");
    printf("  whole multiples of 1/64 of a pixel.\n\n");

    wait_enter();
    clear_screen();
    heading("PART 2: responsive with no media query");

    printf("  This means as many columns as fit, none under 120 pixels:\n\n");

    print_code(
        ".grid { grid-template-columns: repeat(auto-fit, minmax(120px, 1fr)); gap: 8px; }\n"
    );

    for (int width = 162; width <= 543; width += 381) {
        printf("    %d pixels: room for %d column(s)\n", width, (width + 8) / (120 + 8));
    }
    printf("\n");

    printf("  Measured in Chromium 152, with two items:\n\n");
    printf("    162 pixels:         one column\n");
    printf("    543 pixels, auto-fit:   267 267 0 0, the two items stretch\n");
    printf("    603 pixels, auto-fill:  four columns of 144.75, the items stay narrow\n");
    printf("\n");

    printf("  With few items, auto-fit collapses the empty columns so the\n");
    printf("  items stretch across the width. auto-fill keeps the empty\n");
    printf("  columns.\n\n");

    wait_enter();
    clear_screen();
    heading("PART 3: placing items");

    printf("  Lines are numbered from 1, and negative numbers count from the\n");
    printf("  end, so grid-column: 1 / -1 spans the whole row. Named areas\n");
    printf("  read like a picture of the layout:\n\n");

    print_code(
        ".page { display: grid; grid-template-columns: 100px 1fr;\n"
        "        grid-template-areas: \"head head\" \"side main\"; }\n"
        ".a { grid-area: head; }\n"
        ".b { grid-area: side; }\n"
        ".c { grid-area: main; }\n"
    );

    wait_enter();
    clear_screen();
    heading("PART 4: 1fr can be forced wider than you asked");

    printf("  A 1fr column will not go narrower than its content, the same\n");
    printf("  rule as Flexbox. A long line of code is enough:\n\n");

    print_code(
        ".grid { display: grid; width: 300px; }\n"
        ".plain { grid-template-columns: 100px 1fr; }\n"
        ".fixed { grid-template-columns: 100px minmax(0, 1fr); }\n"
        ".fixed pre { overflow: auto; }\n"
    );

    printf("  Measured in Chromium 152:\n\n");
    printf("    plain:   content 430 pixels wide in a 300 pixel grid\n");
    printf("    fixed:   content 300 pixels wide, the code scrolls\n");
    printf("\n");

    printf("  overflow: auto matters here too. An item that scrolls has no\n");
    printf("  minimum size of its own, so it would not have forced the\n");
    printf("  column open in the first place.\n\n");

    wait_enter();
    clear_screen();
    exercise(3);

    question("grid-template-columns: 1fr 1fr 2fr on a 400px grid with no\n"
             "  gap. How many pixels is the last column? (a number)",
             "200",
             "It gets two of the four shares.");

    question("In grid-column: 1 / -1, which line does -1 mean? (one word)",
             "last",
             "So the item spans every column.");

    question("With few items, which keeps empty columns, auto-fit or\n"
             "  auto-fill?",
             "auto-fill",
             "auto-fit collapses them.");

    {
        const char *task[] = {
            "Build a photo gallery of six boxes with Grid, using no media",
            "query.",
        };
        const char *input[] = {
            "resize the window",
        };
        const char *expected[] = {
            "Boxes are never narrower than 150 pixels.",
            "The number of columns changes as the window is resized.",
            "The first box spans the whole first row.",
        };
        const char *solution[] = {
            "<style>",
            "  .gallery {",
            "    display: grid;",
            "    grid-template-columns: repeat(auto-fill, minmax(150px, 1fr));",
            "    gap: 8px;",
            "  }",
            "  .gallery div { height: 100px; background: khaki; }",
            "  .gallery .wide { grid-column: 1 / -1; }",
            "</style>",
            "<div class=\"gallery\">",
            "  <div class=\"wide\"></div><div></div><div></div><div></div><div></div><div></div>",
            "</div>",
        };
        challenge(task, COUNT(task), input, COUNT(input),
                  expected, COUNT(expected), solution, COUNT(solution));
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    if (!print_summary_pt(3)) {
        printf("   - fr is a share of the free space.\n");
        printf("   - repeat(auto-fit, minmax(...)) adapts with no media query.\n");
        printf("   - grid-template-areas draws the layout in text.\n");
        printf("   - minmax(0, 1fr) stops content forcing a column wider.\n");
    }
    wait_enter();
}

void lesson_04_responsive(void)
{
    title("MODULE 4 - RESPONSIVE DESIGN");

    heading("PART 1: em compounds and rem does not");

    printf("  em is relative to the parent's font size, so nesting\n");
    printf("  multiplies it. rem is relative to the root, so it does not:\n\n");

    print_code(
        ".em ul  { font-size: 1.3em; }\n"
        ".rem ul { font-size: 1.3rem; }\n"
    );

    {
        double em = 16;

        for (int depth = 1; depth <= 3; depth++) {
            em *= 1.3;
            printf("    depth %d:  em %.3f pixels,  rem %.1f pixels\n", depth, em, 16 * 1.3);
        }
        printf("\n");
    }

    printf("  Measured in Chromium 152:\n\n");
    printf("    em:   20.8, 27.04, 35.152\n");
    printf("    rem:  20.8, 20.8, 20.8\n");
    printf("\n");

    wait_enter();
    clear_screen();
    heading("PART 2: media queries, and the order they go in");

    print_code(
        ".layout { display: block; }\n"
        "@media (min-width: 400px) {\n"
        "  .layout { display: flex; }\n"
        "}\n"
    );

    printf("  Measured in Chromium 152:\n\n");
    printf("    in a 238 pixel viewport:  block\n");
    printf("    in a 478 pixel viewport:  flex\n");
    printf("\n");

    printf("  A media query adds no specificity. If a plain rule comes after\n");
    printf("  it, the plain rule wins even when the query matches:\n\n");

    print_code(
        "@media (min-width: 100px) { p { color: crimson; } }\n"
        "p { color: steelblue; }\n"
    );

    printf("  Measured in Chromium 152:\n\n");
    printf("    the text colour, with the query matching:  steelblue\n");
    printf("\n");

    printf("  Put base rules first and queries after them.\n\n");

    wait_enter();
    clear_screen();
    heading("PART 3: fluid values");

    printf("  clamp(min, preferred, max) gives a value that follows the\n");
    printf("  window between two limits:\n\n");

    print_code(
        "p { font-size: clamp(14px, 8vw, 40px); }\n"
    );

    {
        const int widths[] = { 198, 598 };

        for (int i = 0; i < COUNT(widths); i++) {
            double preferred = widths[i] * 8 / 100.0;
            double size = preferred < 14 ? 14 : preferred > 40 ? 40 : preferred;

            printf("    %d pixel window: 8vw is %.2f, clamped to %.2f\n", widths[i], preferred, size);
        }
        printf("\n");
    }

    printf("  Measured in Chromium 152:\n\n");
    printf("    font size in those two windows:  15.84px and 40px\n");
    printf("\n");

    printf("  An element wider than its container overflows, unless told\n");
    printf("  otherwise. A 600 pixel item in a 258 pixel window stayed 600\n");
    printf("  wide, and with max-width: 100%% it measured 242.\n\n");

    wait_enter();
    clear_screen();
    heading("PART 4: ask the container, not the window");

    printf("  A media query knows the size of the window. A component does\n");
    printf("  not care about the window, it cares how much room it was\n");
    printf("  given. A container query asks exactly that:\n\n");

    print_code(
        ".slot { container-type: inline-size; }\n"
        ".card { display: block; }\n"
        "@container (min-width: 300px) {\n"
        "  .card { display: flex; }\n"
        "}\n"
    );

    printf("  Measured in Chromium 152, the same component twice in one window:\n\n");
    printf("    in a 200 pixel slot:  block\n");
    printf("    in a wider slot:      flex\n");
    printf("\n");

    wait_enter();
    clear_screen();
    exercise(4);

    question("1.5em inside 1.5em on a 16px root. How many pixels is the\n"
             "  font size? (a number)",
             "36",
             "16 times 1.5 times 1.5.");

    question("Does a media query add specificity? (yes or no)",
             "no",
             "A later plain rule still wins.");

    question("Which at-rule lets a component react to the space it was\n"
             "  given?",
             "@container",
             "With container-type on the parent.");

    {
        const char *task[] = {
            "Write a page with a main area and a sidebar.",
        };
        const char *input[] = {
            "resize the window across 600 pixels",
        };
        const char *expected[] = {
            "Below 600 pixels the sidebar is under the main area,",
            "full width.",
            "From 600 pixels up they are side by side, and the",
            "sidebar is 200 pixels wide.",
            "The heading size changes smoothly with the window,",
            "between 24 and 48 pixels.",
        };
        const char *solution[] = {
            "<style>",
            "  h1 { font-size: clamp(24px, 6vw, 48px); }",
            "  .page { display: block; }",
            "  @media (min-width: 600px) {",
            "    .page { display: grid; grid-template-columns: minmax(0, 1fr) 200px; }",
            "  }",
            "</style>",
            "<h1>Title</h1>",
            "<div class=\"page\"><main>Main</main><aside>Sidebar</aside></div>",
        };
        challenge(task, COUNT(task), input, COUNT(input),
                  expected, COUNT(expected), solution, COUNT(solution));
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    if (!print_summary_pt(4)) {
        printf("   - em multiplies when nested. rem does not.\n");
        printf("   - Base rules first, media queries after them.\n");
        printf("   - clamp() gives a value limits on both sides.\n");
        printf("   - @container responds to the available space, not the\n");
        printf("     window.\n");
    }
    wait_enter();
}

void lesson_05_modern(void)
{
    title("MODULE 5 - MODERN CSS AND THE CASCADE");

    heading("PART 1: who wins");

    printf("  When rules conflict, the browser compares specificity first\n");
    printf("  and source order after it. Specificity has three columns,\n");
    printf("  compared left to right. A column never carries into the next:\n\n");

    print_code(
        "#one { color: steelblue; }\n"
        ".a.b.c.d.e.f.g.h.i.j.k { color: crimson; }\n"
    );

    {
        const struct Specificity id_rule = { "#one", 1, 0, 0 };
        const struct Specificity class_rule = { ".a.b.c.d.e.f.g.h.i.j.k", 0, 11, 0 };

        print_winner(&id_rule, &class_rule);
    }

    printf("  Measured in Chromium 152:\n\n");
    printf("    the text colour:  steelblue\n");
    printf("\n");

    wait_enter();
    clear_screen();
    heading("PART 2: a custom property that is the wrong type");

    printf("  Custom properties are checked when they are used, not when\n");
    printf("  they are declared. That removes the safety net you normally\n");
    printf("  have:\n\n");

    print_code(
        "body { color: steelblue; }\n"
        "p { color: crimson; }\n"
        ".plain { color: 20px; }\n"
        ".variable { --size: 20px; color: var(--size); }\n"
    );

    printf("  Measured in Chromium 152:\n\n");
    printf("    color: 20px         crimson, the bad declaration was dropped\n");
    printf("    color: var(--size)  steelblue, the inherited colour\n");
    printf("\n");

    printf("  An invalid var() has already replaced the earlier rule by the\n");
    printf("  time it fails, so the value falls back to inherited or\n");
    printf("  initial, not to the previous declaration.\n\n");

    wait_enter();
    clear_screen();
    heading("PART 3: :is() and :where() look the same");

    printf("  :is() takes the specificity of its most specific argument,\n");
    printf("  even an argument that matches nothing. :where() always counts\n");
    printf("  as zero:\n\n");

    print_code(
        ":is(#never-used, p) { color: crimson; }\n"
        "p.note { color: steelblue; }\n"
        "\n"
        ":where(#never-used, h4) { color: crimson; }\n"
        "h4.note { color: steelblue; }\n"
    );

    {
        const struct Specificity is_rule = { ":is(#never-used, p)", 1, 0, 0 };
        const struct Specificity p_rule = { "p.note", 0, 1, 1 };
        const struct Specificity where_rule = { ":where(#never-used, h4)", 0, 0, 0 };
        const struct Specificity h4_rule = { "h4.note", 0, 1, 1 };

        print_winner(&is_rule, &p_rule);
        print_winner(&where_rule, &h4_rule);
    }

    printf("  Measured in Chromium 152:\n\n");
    printf("    the paragraph:  crimson\n");
    printf("    the h4:         steelblue\n");
    printf("\n");

    wait_enter();
    clear_screen();
    heading("PART 4: layers change the order of battle");

    printf("  Styles outside any layer beat styles inside one, whatever\n");
    printf("  their specificity:\n\n");

    print_code(
        "@layer base { p#x { color: crimson; } }\n"
        "p { color: steelblue; }\n"
    );

    printf("  Measured in Chromium 152:\n\n");
    printf("    the text colour:  steelblue\n");
    printf("\n");

    wait_enter();
    clear_screen();
    heading("PART 5: :has() and nesting");

    printf("  :has() selects an element by what it contains:\n\n");

    print_code(
        "label:has(input:checked) { background: khaki; }\n"
    );

    printf("  Measured in Chromium 152:\n\n");
    printf("    after the box inside the label is ticked:  khaki background\n");
    printf("\n");

    printf("  Rules can now be nested the way preprocessors have allowed for\n");
    printf("  years. The & is the parent selector:\n\n");

    print_code(
        ".card {\n"
        "  color: steelblue;\n"
        "  & strong { color: crimson; }\n"
        "}\n"
    );

    printf("  Measured in Chromium 152:\n\n");
    printf("    .card:         steelblue\n");
    printf("    .card strong:  crimson\n");
    printf("\n");

    wait_enter();
    clear_screen();
    exercise(5);

    question("One id against ten classes. Which wins? (id or classes)",
             "id",
             "Specificity compares column by column.");

    question("What is the specificity of :where(#a, .b)? (a number)",
             "0",
             "That is the whole point of :where().");

    question("An unlayered p against a layered #x. Which wins? (p or #x)",
             "p",
             "Layer order comes before specificity.");

    {
        const char *task[] = {
            "Build a light and dark theme with custom properties, using",
            "no script.",
        };
        const char *input[] = {
            "tick a checkbox on your page",
        };
        const char *expected[] = {
            "Colours are defined once, as custom properties.",
            "Ticking the box switches the whole page to dark colours.",
            "Unticking it switches back.",
        };
        const char *solution[] = {
            "<style>",
            "  body { --bg: white; --text: black; background: var(--bg); color: var(--text); }",
            "  body:has(#dark:checked) { --bg: #222; --text: #eee; }",
            "</style>",
            "<label><input type=\"checkbox\" id=\"dark\"> Dark</label>",
            "<p>Some text.</p>",
        };
        challenge(task, COUNT(task), input, COUNT(input),
                  expected, COUNT(expected), solution, COUNT(solution));
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    if (!print_summary_pt(5)) {
        printf("   - Specificity is compared column by column, then source\n");
        printf("     order decides.\n");
        printf("   - An invalid var() falls back to inherited or initial, not\n");
        printf("     to the previous rule.\n");
        printf("   - :is() takes its argument's weight. :where() weighs\n");
        printf("     nothing.\n");
        printf("   - Unlayered styles beat layered ones.\n");
        printf("   - :has() selects by contents, and rules can be nested with\n");
        printf("     &.\n");
    }
    wait_enter();
}
