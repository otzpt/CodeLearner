#!/usr/bin/env node
// Static checks for the web courses, standard library only:
//
//   node web/check-course.js
//
// The pages are hand-maintained HTML, so a listing and the demo beside it can
// drift apart. This compares them, and checks each module has the usual shape
// and that its local links exist. What a demo does on screen needs a browser
// and is not checked here: see web/README.md.

"use strict";

const fs = require("fs");
const path = require("path");

const root = __dirname;
let problems = 0;
let checked = 0;

function fail(file, message) {
  console.log(`${path.relative(root, file)}: ${message}`);
  problems++;
}

function decode(text) {
  return text
    .replace(/&lt;/g, "<")
    .replace(/&gt;/g, ">")
    .replace(/&quot;/g, '"')
    .replace(/&amp;/g, "&");
}

function normalize(text) {
  return text.replace(/\r\n/g, "\n").trim();
}

function checkDemos(file, html) {
  for (const [figure] of html.matchAll(/<figure class="demo">[\s\S]*?<\/figure>/g)) {
    const frames = [...figure.matchAll(/srcdoc="([^"]*)"/g)];
    if (frames.length > 0) {
      const listing = figure.match(/<pre><code>([\s\S]*?)<\/code><\/pre>/);
      for (const frame of frames) {
        checked++;
        if (!listing || normalize(decode(listing[1])) !== normalize(decode(frame[1]))) {
          fail(file, "a frame does not match the listing above it");
        }
      }
    }

    const script = figure.match(/<script>\ndemo\("[^"]+", async function \(print\) \{\n([\s\S]*?)\n\}\);\n<\/script>/);
    if (script) {
      checked++;
      const listings = [...figure.matchAll(/<pre><code>([\s\S]*?)<\/code><\/pre>/g)].map((m) => normalize(decode(m[1])));
      if (!listings.includes(normalize(script[1]))) {
        fail(file, "a script does not match its listing");
      }
    }
  }

  for (const [, name, listing] of html.matchAll(/<pre data-file="([^"]+)"><code>([\s\S]*?)<\/code><\/pre>/g)) {
    checked++;
    const target = path.join(path.dirname(file), name);
    if (!fs.existsSync(target)) {
      fail(file, `listing names a missing file: ${name}`);
    } else if (normalize(fs.readFileSync(target, "utf8")) !== normalize(decode(listing))) {
      fail(file, `listing differs from ${name}`);
    }
  }
}

function checkLinks(file, html) {
  // Listings, srcdoc documents and inline scripts hold examples, not links.
  const live = html
    .replace(/srcdoc="[^"]*"/g, "")
    .replace(/<pre[\s\S]*?<\/pre>/g, "")
    .replace(/<script>[\s\S]*?<\/script>/g, "");
  for (const [, url] of live.matchAll(/\b(?:href|src)="([^"#]+)"/g)) {
    if (/^[a-z]+:/i.test(url) || url.startsWith("/")) {
      continue;
    }
    if (!fs.existsSync(path.join(path.dirname(file), url))) {
      fail(file, `broken local link: ${url}`);
    }
  }
}

function checkModule(file, html, number) {
  for (const needle of [`<h1>Module ${number}:`, "PART 1:", `<!-- exercise ${number} -->`, "<h3>Challenge</h3>", "<!-- summary -->", "<h2>SUMMARY</h2>"]) {
    if (!html.includes(needle)) {
      fail(file, `missing ${needle}`);
    }
  }
}

for (const course of ["html", "css", "js"]) {
  const dir = path.join(root, course);
  for (const name of fs.readdirSync(dir).filter((n) => n.endsWith(".html"))) {
    const file = path.join(dir, name);
    const html = fs.readFileSync(file, "utf8");
    if (html.includes("—")) {
      fail(file, "contains an em dash");
    }
    const module = name.match(/^(\d+)-/);
    if (module) {
      checkModule(file, html, Number(module[1]));
    }
    checkDemos(file, html);
    checkLinks(file, html);
  }
}

if (problems > 0) {
  console.log(`\n${problems} problem(s).`);
  process.exit(1);
}
console.log(`Web courses clean (${checked} demos compared with their listings).`);
