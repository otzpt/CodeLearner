const { page } = require("./01-rules.js");

module.exports = {
  id: "class-and-id",
  title: "Picking elements: class and id",
  titlePt: "Escolher elementos: class e id",
  screens: [
    [
      "Styling every p the same is rarely what you want. Give elements a class in the HTML, and select it in CSS with a dot:",
      { code: page(".warning {\n  color: red;\n}", '<p>Normal text</p>\n<p class="warning">Careful!</p>\n<div class="warning">Also careful</div>') },
      "A class can be on any number of elements, of any kind.",
    ],
    [
      "An element can have several classes, separated by spaces, and get the styles of each. p.note picks only p elements that have the class note:",
      { code: page(".big {\n  font-size: 24px;\n}\np.note {\n  font-style: italic;\n}", '<p class="note big">A big note</p>\n<div class="note">Not a p</div>') },
    ],
    [
      "An id is a name only one element may have. In CSS it is selected with #:",
      { code: page("#logo {\n  font-weight: bold;\n}", '<h1 id="logo">Shop</h1>\n<p>Welcome</p>') },
      "Prefer classes for styling. ids are better kept for links to a part of a page, and for scripts.",
      { quiz: "How is the class card selected in CSS?", choices: ["#card", ".card", "card"], answer: 2, why: "A dot for classes, # for ids, a plain name for elements." },
    ],
    [
      {
        exercise: "classes",
        task: 'Give the second li the class "done", and style .done with color: gray. The list is ul with li "Wash", "Cook", "Eat".',
        expected: ["ul", "  li", "  li.done", "    color: gray   (from .done)", "  li"],
        starter: page("  /* your CSS here */", "<ul>\n  <li>Wash</li>\n  <li>Cook</li>\n  <li>Eat</li>\n</ul>"),
        hints: ['<li class="done">Cook</li>', ".done { color: gray; }"],
        solution: page(".done { color: gray; }", '<ul>\n  <li>Wash</li>\n  <li class="done">Cook</li>\n  <li>Eat</li>\n</ul>'),
      },
    ],
  ],
  recapPt: [
    ".nome escolhe elementos com essa class; podem ser muitos.",
    "Um elemento pode ter várias classes; p.note escolhe só os p com note.",
    "#nome escolhe o único elemento com esse id.",
  ],
  recap: [
    ".name picks elements with that class; there can be many.",
    "An element can have several classes; p.note picks only p with note.",
    "#name picks the one element with that id.",
  ],
};
