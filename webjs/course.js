// How the Web JavaScript course runs a page: there is no browser in a
// terminal, so learn/web.js builds the page and runs its <script>s against
// a small stand-in for the browser's document. sim.click and sim.type play
// the user; they are PC-only.

const path = require("node:path");
const javascript = require("../javascript/course.js");

const WEB = path.join(__dirname, "..", "learn", "web.js");
const NODE = process.execPath;

module.exports = {
  id: "webjs",
  title: "WEB JAVASCRIPT FROM ZERO",
  titlePt: "JAVASCRIPT WEB DO ZERO",
  file: "page.html",
  starter: "<!DOCTYPE html>\n<html>\n<body>\n  <p id=\"out\">Practice</p>\n  <script>\n    // your code here\n  </script>\n</body>\n</html>",
  steps: (file) => [
    [NODE, WEB, "check", file],
    [NODE, WEB, "run", file],
  ],
  howToRun: (name) => `Open ${name} in any browser; console.log appears in the browser\'s developer tools (F12).`,
  warnings: (text) => text.split("\n").filter((l) => l.startsWith("warning: ")).map((l) => l.slice(9)),
  // The line is the page's, not the line inside learn/web.js.
  parseError(stderr) {
    const where = stderr.match(/\.html:(\d+)/);
    return { ...javascript.parseError(stderr), line: where ? Number(where[1]) : null };
  },
  errors: javascript.errors,
};
