/*
 * The Web JavaScript course's lessons.
 *
 * Like gui/, this course is plain C with no dependency beyond a compiler. It
 * teaches Web JavaScript; it does not open a browser or link against one.
 *
 * A terminal cannot render a page. Where a result depends on a browser the
 * lesson quotes what one did, and those values were measured in Chromium 152
 * by script while the module was written.
 */

#include <stdio.h>

#include "lessons.h"
#include "ui.h"

#define COUNT(array) (int) (sizeof (array) / sizeof (array)[0])

void lesson_01_dom(void)
{
    title("MODULE 1 - THE DOM");

    heading("PART 1: finding an element");

    printf("  The JavaScript course in this project runs under Node, where\n");
    printf("  there is no page. This one is about what only a browser has.\n");
    printf("  It assumes you know variables, functions, arrays and if, the\n");
    printf("  first modules of that course.\n\n");

    printf("  The browser turns your HTML into a tree of objects, the DOM.\n");
    printf("  document.querySelector takes a CSS selector and returns the\n");
    printf("  first match:\n\n");

    print_code(
        "<h3 id=\"title\">Original text</h3>\n"
        "\n"
        "const heading = document.querySelector(\"#title\");\n"
        "console.log(\"Before:\", heading.textContent);\n"
        "heading.textContent = \"Changed by JavaScript\";\n"
        "console.log(\"After:\", heading.textContent);\n"
    );

    printf("  Output captured from Chromium 152:\n\n");
    printf("    Before: Original text\n");
    printf("    After: Changed by JavaScript\n");
    printf("\n");

    printf("  When nothing matches it returns null, not an error. The error\n");
    printf("  comes one line later:\n\n");

    print_code(
        "const missing = document.querySelector(\"#no-such-id\");\n"
        "console.log(\"It returned:\", missing);\n"
        "console.log(missing.textContent);\n"
    );

    printf("  Output captured from Chromium 152:\n\n");
    printf("    It returned: null\n");
    printf("    TypeError: Cannot read properties of null (reading 'textContent')\n");
    printf("\n");

    wait_enter();
    clear_screen();
    heading("PART 2: the script runs before the rest exists");

    printf("  The page is built from top to bottom, and a script runs at the\n");
    printf("  moment the parser reaches it. An element further down does not\n");
    printf("  exist yet:\n\n");

    print_code(
        "<script>\n"
        "  console.log(document.querySelector(\"#later\"));\n"
        "</script>\n"
        "<p id=\"later\">I come after the script above.</p>\n"
    );

    printf("  Output captured from Chromium 152:\n\n");
    printf("    null\n");
    printf("\n");

    printf("  The same search from a script placed after that paragraph\n");
    printf("  finds it. The usual fix is an external script with defer,\n");
    printf("  which waits until the document has been parsed:\n\n");

    print_code(
        "<script defer src=\"main.js\"></script>\n"
        "<p id=\"target\">Further down the page</p>\n"
    );

    printf("  Output captured from Chromium 152, from the deferred file:\n\n");
    printf("    #target is: Further down the page\n");
    printf("\n");

    wait_enter();
    clear_screen();
    heading("PART 3: textContent and innerHTML are not interchangeable");

    printf("  textContent treats a string as text. innerHTML treats it as\n");
    printf("  HTML, and that includes anything a user typed. This snippet\n");
    printf("  ran inside an async function, which is what lets it use await:\n\n");

    print_code(
        "const box = document.querySelector(\"#box\");\n"
        "const fromUser = '<img src=\"x\" onerror=\"this.alt = \\'the handler ran\\'\">';\n"
        "\n"
        "box.textContent = fromUser;\n"
        "console.log(\"textContent made\", box.children.length, \"elements\");\n"
        "\n"
        "box.innerHTML = fromUser;\n"
        "console.log(\"innerHTML made\", box.children.length, \"elements\");\n"
        "\n"
        "await new Promise((done) => setTimeout(done, 500));\n"
        "console.log(\"alt is now:\", box.firstChild.alt);\n"
    );

    printf("  Output captured from Chromium 152:\n\n");
    printf("    textContent made 0 elements\n");
    printf("    innerHTML made 1 elements\n");
    printf("    alt is now: the handler ran\n");
    printf("\n");

    printf("  That harmless handler ran because the text was parsed as HTML.\n");
    printf("  On a real site, it would be someone's code running with your\n");
    printf("  user's session. This is cross-site scripting. Never pass user\n");
    printf("  input to innerHTML.\n\n");

    wait_enter();
    clear_screen();
    heading("PART 4: a list that updates itself, and one that does not");

    printf("  querySelectorAll returns a snapshot. The older\n");
    printf("  getElementsByTagName returns a live list, which changes as the\n");
    printf("  page changes:\n\n");

    print_code(
        "<ul id=\"items\"><li>one</li><li>two</li></ul>\n"
        "\n"
        "const list = document.querySelector(\"#items\");\n"
        "const live = list.getElementsByTagName(\"li\");\n"
        "const snapshot = list.querySelectorAll(\"li\");\n"
        "\n"
        "const item = document.createElement(\"li\");\n"
        "item.textContent = \"three\";\n"
        "list.append(item);\n"
        "\n"
        "console.log(\"live:\", live.length);\n"
        "console.log(\"snapshot:\", snapshot.length);\n"
    );

    printf("  Output captured from Chromium 152:\n\n");
    printf("    live: 3\n");
    printf("    snapshot: 2\n");
    printf("\n");

    wait_enter();
    clear_screen();
    exercise(1);

    question("What does querySelector return when nothing matches?",
             "null",
             "The crash comes when you use it.");

    question("Which property is safe for text typed by a user, textContent\n"
             "  or innerHTML?",
             "textContent",
             "It never parses HTML.");

    question("Which script attribute makes it wait for the document to be\n"
             "  parsed?",
             "defer",
             "It only works on external scripts.");

    {
        const char *task[] = {
            "Write a page with an empty ul whose list items are created",
            "by JavaScript from an array.",
        };
        const char *input[] = {
            "the array [\"milk\", \"eggs\", \"bread\"] in your script",
        };
        const char *expected[] = {
            "The page lists milk, eggs and bread, one per line.",
            "The script does not use innerHTML.",
        };
        const char *solution[] = {
            "<ul id=\"shopping\"></ul>",
            "<script>",
            "  const list = document.querySelector(\"#shopping\");",
            "  for (const name of [\"milk\", \"eggs\", \"bread\"]) {",
            "    const item = document.createElement(\"li\");",
            "    item.textContent = name;",
            "    list.append(item);",
            "  }",
            "</script>",
        };
        challenge(task, COUNT(task), input, COUNT(input),
                  expected, COUNT(expected), solution, COUNT(solution));
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    if (!print_summary_pt(1)) {
        printf("   - querySelector returns null for no match, so check before\n");
        printf("     you use it.\n");
        printf("   - A script can only see what has already been parsed. Use\n");
        printf("     defer.\n");
        printf("   - innerHTML on user input is cross-site scripting.\n");
        printf("   - querySelectorAll is a snapshot.\n");
    }
    wait_enter();
}

void lesson_02_events(void)
{
    title("MODULE 2 - EVENTS");

    heading("PART 1: pass the function, do not call it");

    printf("  addEventListener wants a function to call later. Add\n");
    printf("  parentheses and you call it now, and pass whatever it returns:\n\n");

    print_code(
        "let calls = 0;\n"
        "function count() {\n"
        "  calls++;\n"
        "}\n"
        "button.addEventListener(\"click\", count());\n"
        "console.log(\"Calls before anyone has clicked:\", calls);\n"
    );

    printf("  Output captured from Chromium 152:\n\n");
    printf("    Calls before anyone has clicked: 1\n");
    printf("\n");

    printf("  And the button then does nothing, because count() returned\n");
    printf("  undefined and that is what got registered. Without the\n");
    printf("  parentheses, the function waits for the click.\n\n");

    wait_enter();
    clear_screen();
    heading("PART 2: an event travels up");

    printf("  A click on an element also fires on each of its ancestors.\n");
    printf("  event.target is what was really clicked and\n");
    printf("  event.currentTarget is the element whose listener is running:\n\n");

    print_code(
        "<div id=\"outer\">Outer <button id=\"inner\">Inner</button></div>\n"
        "\n"
        "for (const selector of [\"#inner\", \"#outer\"]) {\n"
        "  document.querySelector(selector).addEventListener(\"click\", (event) => {\n"
        "    console.log(\"listener on\", event.currentTarget.id, \"- target is\", event.target.id);\n"
        "  });\n"
        "}\n"
    );

    printf("  Output captured from Chromium 152, clicking the button:\n\n");
    printf("    listener on inner - target is inner\n");
    printf("    listener on outer - target is inner\n");
    printf("\n");

    wait_enter();
    clear_screen();
    heading("PART 3: one listener instead of many");

    printf("  Elements added later have no listeners of their own, but their\n");
    printf("  clicks still bubble up. So one listener on the parent covers\n");
    printf("  children that do not exist yet:\n\n");

    print_code(
        "list.addEventListener(\"click\", (event) => {\n"
        "  if (event.target.matches(\"li\")) {\n"
        "    console.log(\"Clicked:\", event.target.textContent);\n"
        "  }\n"
        "});\n"
    );

    printf("  Output captured from Chromium 152, clicking an li added after that ran:\n\n");
    printf("    Clicked: added later\n");
    printf("\n");

    printf("  A delegated listener often needs to know which item it got. A\n");
    printf("  data-* attribute carries a value on the element, and dataset\n");
    printf("  reads it back, always as a string:\n\n");

    print_code(
        "<p id=\"price\" data-cost=\"5\">Five euros</p>\n"
        "\n"
        "const item = document.querySelector(\"#price\");\n"
        "console.log(item.dataset.cost, typeof item.dataset.cost);\n"
    );

    printf("  Output captured from Chromium 152:\n\n");
    printf("    5 string\n");
    printf("\n");

    wait_enter();
    clear_screen();
    heading("PART 4: two ways to lose a listener");

    printf("  removeEventListener needs the same function object. Two arrow\n");
    printf("  functions that look the same are two different functions:\n\n");

    print_code(
        "button.addEventListener(\"click\", () => console.log(\"still listening\"));\n"
        "button.removeEventListener(\"click\", () => console.log(\"still listening\"));\n"
    );

    printf("  Output captured from Chromium 152, clicking afterwards:\n\n");
    printf("    still listening\n");
    printf("\n");

    printf("  And innerHTML += rebuilds everything inside the element from a\n");
    printf("  string, so every listener in it is gone:\n\n");

    print_code(
        "const button = document.querySelector(\"#b4\");\n"
        "button.addEventListener(\"click\", () => console.log(\"clicked\"));\n"
        "area.innerHTML += \"<p>Added with innerHTML +=</p>\";\n"
        "console.log(\"Is the original button still in the page?\", button.isConnected);\n"
    );

    printf("  Output captured from Chromium 152:\n\n");
    printf("    Is the original button still in the page? false\n");
    printf("    (and clicking the copy that replaced it logs nothing)\n");
    printf("\n");

    wait_enter();
    clear_screen();
    exercise(2);

    question("button.addEventListener(\"click\", save()): does save run now\n"
             "  or on the click?",
             "now",
             "The parentheses call it, once, while the\n"
             "             listener is being added.");

    question("Which property of the event holds the element actually\n"
             "  clicked?",
             "target",
             "currentTarget is the one whose listener is\n"
             "             running.");

    question("What is it called when one listener on a parent handles all\n"
             "  its children? (two words)",
             "event delegation",
             "It works because events bubble.");

    {
        const char *task[] = {
            "Write a page with a counter: a number and two buttons, plus",
            "and minus. Use one listener, on the element that contains",
            "both buttons.",
        };
        const char *input[] = {
            "click plus three times, then minus once",
        };
        const char *expected[] = {
            "The number starts at 0.",
            "After those clicks it shows 2.",
            "There is exactly one addEventListener call.",
        };
        const char *solution[] = {
            "<div id=\"counter\">",
            "  <button data-step=\"1\">+</button>",
            "  <span id=\"value\">0</span>",
            "  <button data-step=\"-1\">-</button>",
            "</div>",
            "<script>",
            "  let value = 0;",
            "  document.querySelector(\"#counter\").addEventListener(\"click\", (event) => {",
            "    if (event.target.matches(\"button\")) {",
            "      value += Number(event.target.dataset.step);",
            "      document.querySelector(\"#value\").textContent = value;",
            "    }",
            "  });",
            "</script>",
        };
        challenge(task, COUNT(task), input, COUNT(input),
                  expected, COUNT(expected), solution, COUNT(solution));
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    if (!print_summary_pt(2)) {
        printf("   - Pass the function. fn() calls it at once.\n");
        printf("   - Events bubble. target is the origin and currentTarget is\n");
        printf("     the listener's element.\n");
        printf("   - Delegation covers elements that do not exist yet.\n");
        printf("   - Removing a listener needs the same function object.\n");
        printf("   - innerHTML += destroys the listeners inside.\n");
    }
    wait_enter();
}

void lesson_03_forms(void)
{
    title("MODULE 3 - FORMS AND INPUT");

    heading("PART 1: every value is a string");

    printf("  .value is a string, even on type=\"number\":\n\n");

    print_code(
        "<input id=\"a\" type=\"number\" value=\"5\">\n"
        "<input id=\"b\" type=\"number\" value=\"3\">\n"
        "\n"
        "console.log(\"value + value:\", a.value + b.value);\n"
        "console.log(\"typeof a.value:\", typeof a.value);\n"
        "console.log(\"valueAsNumber:\", a.valueAsNumber + b.valueAsNumber);\n"
    );

    printf("  Output captured from Chromium 152:\n\n");
    printf("    value + value: 53\n");
    printf("    typeof a.value: string\n");
    printf("    valueAsNumber: 8\n");
    printf("\n");

    wait_enter();
    clear_screen();
    heading("PART 2: a checkbox's value is not its state");

    printf("  A checkbox's .value is the text on, whether it is ticked or\n");
    printf("  not. .checked is the state:\n\n");

    print_code(
        "console.log(\"value:\", box.value, \" checked:\", box.checked);\n"
        "box.checked = true;\n"
        "console.log(\"value:\", box.value, \" checked:\", box.checked);\n"
    );

    printf("  Output captured from Chromium 152:\n\n");
    printf("    value: on  checked: false\n");
    printf("    value: on  checked: true\n");
    printf("\n");

    wait_enter();
    clear_screen();
    heading("PART 3: submit, without leaving the page");

    printf("  A form navigates when it is submitted. event.preventDefault()\n");
    printf("  stops that. FormData then reads what would have been sent,\n");
    printf("  with the same rules: no name means not included, and an\n");
    printf("  unticked box is simply absent.\n\n");

    print_code(
        "<form id=\"f1\">\n"
        "  <input name=\"email\" value=\"ana@example.com\">\n"
        "  <input value=\"no name\">\n"
        "  <label><input type=\"checkbox\" name=\"news\"> News</label>\n"
        "  <button>Send</button>\n"
        "</form>\n"
        "\n"
        "form.addEventListener(\"submit\", (event) => {\n"
        "  event.preventDefault();\n"
        "  const data = new FormData(event.target);\n"
        "  console.log(JSON.stringify(Object.fromEntries(data)));\n"
        "});\n"
    );

    printf("  Output captured from Chromium 152:\n\n");
    printf("    {\"email\":\"ana@example.com\"}\n");
    printf("\n");

    wait_enter();
    clear_screen();
    heading("PART 4: checking validity from a script");

    printf("  checkValidity() asks whether a form would pass the browser's\n");
    printf("  own checks, without submitting anything:\n\n");

    print_code(
        "<form id=\"f2\"><input name=\"age\" type=\"number\" required></form>\n"
        "\n"
        "console.log(\"checkValidity():\", form.checkValidity());\n"
        "form.elements.age.value = 30;\n"
        "console.log(\"after setting age:\", form.checkValidity());\n"
    );

    printf("  Output captured from Chromium 152:\n\n");
    printf("    checkValidity(): false\n");
    printf("    after setting age: true\n");
    printf("\n");

    wait_enter();
    clear_screen();
    exercise(3);

    question("What is the type of input.value on type=\"number\"?",
             "string",
             "valueAsNumber gives a number.");

    question("Which property tells you whether a checkbox is ticked?",
             "checked",
             "value is on either way.");

    question("Which method stops a form from navigating away?",
             "preventDefault",
             "Called on the submit event.");

    {
        const char *task[] = {
            "Write a form with two number fields and a button. On submit,",
            "show their sum in the page, without the page reloading.",
        };
        const char *input[] = {
            "the numbers 5 and 3, then press the button",
        };
        const char *expected[] = {
            "The page shows 8, not 53.",
            "The page does not reload.",
        };
        const char *solution[] = {
            "<form id=\"sum\">",
            "  <input name=\"a\" type=\"number\" required>",
            "  <input name=\"b\" type=\"number\" required>",
            "  <button>Add</button>",
            "</form>",
            "<p id=\"result\"></p>",
            "<script>",
            "  document.querySelector(\"#sum\").addEventListener(\"submit\", (event) => {",
            "    event.preventDefault();",
            "    const { a, b } = event.target.elements;",
            "    document.querySelector(\"#result\").textContent = a.valueAsNumber + b.valueAsNumber;",
            "  });",
            "</script>",
        };
        challenge(task, COUNT(task), input, COUNT(input),
                  expected, COUNT(expected), solution, COUNT(solution));
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    if (!print_summary_pt(3)) {
        printf("   - .value is always a string.\n");
        printf("   - .checked is the state of a checkbox.\n");
        printf("   - preventDefault() keeps you on the page.\n");
        printf("   - FormData follows the same rules as a real submission.\n");
    }
    wait_enter();
}

void lesson_04_fetch(void)
{
    title("MODULE 4 - FETCH AND THE NETWORK");

    heading("PART 1: a response, in two steps");

    printf("  fetch gives you a promise for the response. Reading the body\n");
    printf("  is a second promise, and a body can only be read once. These\n");
    printf("  snippets ran inside an async function. This request is to a\n");
    printf("  data: URL, so it needs no network:\n\n");

    print_code(
        "const url = 'data:application/json,{\"answer\":42}';\n"
        "const response = await fetch(url);\n"
        "console.log(\"status:\", response.status);\n"
        "const body = await response.json();\n"
        "console.log(\"body:\", JSON.stringify(body));\n"
        "await response.text();\n"
    );

    printf("  Output captured from Chromium 152:\n\n");
    printf("    status: 200\n");
    printf("    body: {\"answer\":42}\n");
    printf("    TypeError: Failed to execute 'text' on 'Response': body stream already read\n");
    printf("\n");

    wait_enter();
    clear_screen();
    heading("PART 2: 404 is not an error");

    printf("  fetch only rejects when no response arrived at all. A 404 or a\n");
    printf("  500 is a response:\n\n");

    print_code(
        "const response = await fetch(\"https://jsonplaceholder.typicode.com/posts/999999\");\n"
        "console.log(\"no exception was thrown\");\n"
        "console.log(\"status:\", response.status, \" ok:\", response.ok);\n"
    );

    printf("  Output captured from Chromium 152:\n\n");
    printf("    no exception was thrown\n");
    printf("    status: 404  ok: false\n");
    printf("\n");

    printf("  So every call needs its own check:\n\n");

    print_code(
        "const response = await fetch(url);\n"
        "if (!response.ok) {\n"
        "  throw new Error(\"HTTP \" + response.status);\n"
        "}\n"
        "const data = await response.json();\n"
    );

    wait_enter();
    clear_screen();
    heading("PART 3: a request has no time limit");

    printf("  By default a request waits as long as the network allows. An\n");
    printf("  AbortSignal gives it a limit:\n\n");

    print_code(
        "try {\n"
        "  await fetch(\"https://jsonplaceholder.typicode.com/posts/1\", {\n"
        "    signal: AbortSignal.timeout(1),\n"
        "  });\n"
        "} catch (error) {\n"
        "  console.log(\"Rejected with:\", error.name);\n"
        "}\n"
    );

    printf("  Output captured from Chromium 152:\n\n");
    printf("    Rejected with: TimeoutError\n");
    printf("\n");

    wait_enter();
    clear_screen();
    exercise(4);

    question("Does fetch reject on a 404? (yes or no)",
             "no",
             "Only when no response arrived.");

    question("Which response property is true for a status from 200 to\n"
             "  299?",
             "ok",
             "response.ok.");

    question("How many times can a response body be read? (a number)",
             "1",
             "The second attempt throws, as part 1 showed.");

    {
        const char *task[] = {
            "Write a page with a button that fetches",
            "https://jsonplaceholder.typicode.com/posts/1 and shows its",
            "title.",
        };
        const char *input[] = {
            "click the button",
        };
        const char *expected[] = {
            "The title of the post appears in the page.",
            "With the network off, a readable message appears instead",
            "of nothing.",
            "A failed HTTP status is treated as a failure.",
        };
        const char *solution[] = {
            "<button id=\"load\">Load</button>",
            "<p id=\"out\"></p>",
            "<script>",
            "  const out = document.querySelector(\"#out\");",
            "  document.querySelector(\"#load\").addEventListener(\"click\", async () => {",
            "    try {",
            "      const response = await fetch(\"https://jsonplaceholder.typicode.com/posts/1\");",
            "      if (!response.ok) {",
            "        throw new Error(\"HTTP \" + response.status);",
            "      }",
            "      const post = await response.json();",
            "      out.textContent = post.title;",
            "    } catch (error) {",
            "      out.textContent = \"Could not load it: \" + error.message;",
            "    }",
            "  });",
            "</script>",
        };
        challenge(task, COUNT(task), input, COUNT(input),
                  expected, COUNT(expected), solution, COUNT(solution));
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    if (!print_summary_pt(4)) {
        printf("   - fetch takes two awaits: the response, then the body.\n");
        printf("   - A 404 does not reject. Check response.ok.\n");
        printf("   - A body can be read once.\n");
        printf("   - AbortSignal.timeout() gives a request a limit.\n");
    }
    wait_enter();
}

void lesson_05_state(void)
{
    title("MODULE 5 - STATE, STORAGE AND A SMALL APP");

    heading("PART 1: localStorage stores strings");

    printf("  localStorage keeps data after the tab is closed, and\n");
    printf("  everything it stores is turned into a string first:\n\n");

    print_code(
        "localStorage.setItem(\"demo\", 5);\n"
        "const value = localStorage.getItem(\"demo\");\n"
        "console.log(value, typeof value);\n"
        "console.log(\"value + 1 =\", value + 1);\n"
        "\n"
        "localStorage.setItem(\"demo\", { name: \"Ana\" });\n"
        "console.log(localStorage.getItem(\"demo\"));\n"
    );

    printf("  Output captured from Chromium 152:\n\n");
    printf("    5 string\n");
    printf("    value + 1 = 51\n");
    printf("    [object Object]\n");
    printf("\n");

    printf("  An object becomes useless text. Use JSON.stringify on the way\n");
    printf("  in and JSON.parse on the way out:\n\n");

    print_code(
        "localStorage.setItem(\"demo\", JSON.stringify({ name: \"Ana\", age: 30 }));\n"
        "const person = JSON.parse(localStorage.getItem(\"demo\"));\n"
        "console.log(person.name, typeof person.age);\n"
    );

    printf("  Output captured from Chromium 152:\n\n");
    printf("    Ana number\n");
    printf("\n");

    wait_enter();
    clear_screen();
    heading("PART 2: keep the state in one place");

    printf("  If the page is the only record of the data, every change means\n");
    printf("  reading the page back. Keep the data in a variable, change\n");
    printf("  only that, and redraw from it:\n\n");

    print_code(
        "const KEY = \"todos\";\n"
        "const list = document.querySelector(\"#todo-list\");\n"
        "let todos = JSON.parse(localStorage.getItem(KEY)) ?? [];\n"
        "\n"
        "function draw() {\n"
        "  list.replaceChildren(...todos.map((todo, index) => {\n"
        "    const item = document.createElement(\"li\");\n"
        "    const box = document.createElement(\"input\");\n"
        "    box.type = \"checkbox\";\n"
        "    box.checked = todo.done;\n"
        "    box.dataset.index = index;\n"
        "    const label = document.createElement(\"label\");\n"
        "    label.append(box, \" \" + todo.text);\n"
        "    item.append(label);\n"
        "    return item;\n"
        "  }));\n"
        "  localStorage.setItem(KEY, JSON.stringify(todos));\n"
        "}\n"
        "\n"
        "form.addEventListener(\"submit\", (event) => {\n"
        "  event.preventDefault();\n"
        "  const field = event.target.elements.text;\n"
        "  todos.push({ text: field.value, done: false });\n"
        "  field.value = \"\";\n"
        "  draw();\n"
        "});\n"
        "\n"
        "list.addEventListener(\"change\", (event) => {\n"
        "  todos[event.target.dataset.index].done = event.target.checked;\n"
        "  draw();\n"
        "});\n"
        "\n"
        "draw();\n"
    );

    printf("  Measured captured from Chromium 152, adding the text <b>buy milk</b> and ticking it:\n\n");
    printf("    what storage held:  [{\"text\":\"<b>buy milk</b>\",\"done\":true}]\n");
    printf("    <b> elements created in the list:  0\n");
    printf("\n");

    printf("  Every part of it has come up in this course: a form with\n");
    printf("  preventDefault, one delegated listener, elements built with\n");
    printf("  createElement so that user text is never parsed as HTML, and\n");
    printf("  JSON in storage.\n\n");

    wait_enter();
    clear_screen();
    exercise(5);

    question("localStorage.setItem(\"n\", 5). What type does getItem give\n"
             "  back?",
             "string",
             "Everything becomes a string.");

    question("Which call turns an object into text that can be restored?",
             "JSON.stringify",
             "JSON.parse restores it.");

    question("In a state-driven page, what do you change first, the data\n"
             "  or the DOM?",
             "data",
             "Then draw from it.");

    {
        const char *task[] = {
            "Write the counter from module 2 again, so that it survives a",
            "page reload.",
        };
        const char *input[] = {
            "click plus twice, then reload the page",
        };
        const char *expected[] = {
            "After the reload the counter still shows 2.",
            "The value is held in a variable and drawn from it.",
            "A first visit, with nothing saved, shows 0.",
        };
        const char *solution[] = {
            "<div id=\"counter\">",
            "  <button data-step=\"1\">+</button>",
            "  <span id=\"value\"></span>",
            "  <button data-step=\"-1\">-</button>",
            "</div>",
            "<script>",
            "  let value = Number(localStorage.getItem(\"count\") ?? 0);",
            "",
            "  function draw() {",
            "    document.querySelector(\"#value\").textContent = value;",
            "    localStorage.setItem(\"count\", value);",
            "  }",
            "",
            "  document.querySelector(\"#counter\").addEventListener(\"click\", (event) => {",
            "    if (event.target.matches(\"button\")) {",
            "      value += Number(event.target.dataset.step);",
            "      draw();",
            "    }",
            "  });",
            "",
            "  draw();",
            "</script>",
        };
        challenge(task, COUNT(task), input, COUNT(input),
                  expected, COUNT(expected), solution, COUNT(solution));
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    if (!print_summary_pt(5)) {
        printf("   - localStorage stores strings, so use JSON for anything\n");
        printf("     else.\n");
        printf("   - Keep state in variables and draw the page from it.\n");
        printf("   - Build user content with createElement and textContent.\n");
    }
    wait_enter();
}
