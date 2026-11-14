// How the HTML course runs a page: there is no browser in a terminal, so
// learn/web.js builds the page the way a browser would and prints its
// element tree, with a warning for every tag closed wrongly.

const path = require("node:path");
const javascript = require("../javascript/course.js");

const WEB = path.join(__dirname, "..", "learn", "web.js");
const NODE = process.execPath;

module.exports = {
  id: "html",
  title: "HTML FROM ZERO",
  titlePt: "HTML DO ZERO",
  file: "page.html",
  starter: "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n  <title>Practice</title>\n</head>\n<body>\n  <!-- your code here -->\n</body>\n</html>",
  steps: (file) => [
    [NODE, WEB, "check", file],
    [NODE, WEB, "outline", file],
  ],
  howToRun: (name) => `Open ${name} in any browser to see it drawn.`,
  warnings: (text) => text.split("\n").filter((l) => l.startsWith("warning: ")).map((l) => l.slice(9)),
  // The line is the page's, not the line inside learn/web.js.
  parseError(stderr) {
    const where = stderr.match(/\.html:(\d+)/);
    return { ...javascript.parseError(stderr), line: where ? Number(where[1]) : null };
  },
  errors: javascript.errors,
};
