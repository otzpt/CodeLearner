// How the CSS course runs a page: there is no browser in a terminal, so
// learn/web.js applies the page's CSS and prints each element's final
// style, with the rule that won each property.

const path = require("node:path");
const javascript = require("../javascript/course.js");

const WEB = path.join(__dirname, "..", "learn", "web.js");
const NODE = process.execPath;

module.exports = {
  id: "css",
  title: "CSS FROM ZERO",
  titlePt: "CSS DO ZERO",
  file: "page.html",
  starter: "<!DOCTYPE html>\n<html>\n<head>\n  <style>\n    /* your CSS here */\n  </style>\n</head>\n<body>\n  <p>Practice</p>\n</body>\n</html>",
  steps: (file) => [
    [NODE, WEB, "check", file],
    [NODE, WEB, "styles", file],
  ],
  howToRun: (name) => `Open ${name} in any browser to see it drawn.`,
  warnings: (text) => text.split("\n").filter((l) => l.startsWith("warning: ")).map((l) => l.slice(9)),
  parseError: javascript.parseError,
  errors: javascript.errors,
};
