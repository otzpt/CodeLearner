#!/usr/bin/env node
// A small stand-in for a browser, for the HTML, CSS and Web JavaScript
// courses, which run in a terminal where no page can be drawn.
//
//   node learn/web.js check   page.html   warnings about tags closed wrongly
//   node learn/web.js outline page.html   the element tree, as a browser builds it
//   node learn/web.js styles  page.html   each element's final CSS, and which rule won
//   node learn/web.js run     page.html   runs the page's <script>s against a DOM
//
// SIMPLIFICATION: this covers what the lessons use, not the web platform.
// HTML: tags, attributes, void and raw-text elements, comments, common
// entities; no implied end tags or error recovery beyond reporting.
// CSS: type, .class, #id, * selectors, compounds, descendant and child
// combinators, comma lists, !important, inline style, inheritance of text
// properties; no media queries, pseudo-classes or shorthands. DOM: the
// methods listed in Element below. Anything else in a lesson needs adding
// here first.

const fs = require("node:fs");
const util = require("node:util");
const vm = require("node:vm");

// ---------------------------------------------------------------- HTML

const VOID = new Set(["area", "base", "br", "col", "embed", "hr", "img", "input", "link", "meta", "source", "track", "wbr"]);
const RAW = new Set(["script", "style"]);
const ENTITIES = { lt: "<", gt: ">", amp: "&", quot: '"', apos: "'", nbsp: " " };

function decode(text) {
  return text.replace(/&(#x?[0-9a-f]+|\w+);/gi, (all, name) => {
    if (name[0] === "#") {
      return String.fromCodePoint(name[1].toLowerCase() === "x" ? parseInt(name.slice(2), 16) : parseInt(name.slice(1), 10));
    }
    return ENTITIES[name] !== undefined ? ENTITIES[name] : all;
  });
}

class Element {
  constructor(tag, attrs = {}) {
    this.tagName = tag.toUpperCase();
    this.tag = tag.toLowerCase();
    this.attrs = { ...attrs };
    this.childNodes = [];
    this.parentElement = null;
    this.listeners = {};
    this.style = {};
    this.line = 0;
  }

  get id() { return this.attrs.id || ""; }
  set id(v) { this.attrs.id = v; }
  get className() { return this.attrs.class || ""; }
  set className(v) { this.attrs.class = v; }
  get children() { return this.childNodes.filter((n) => n instanceof Element); }
  get value() { return this.attrs.value || ""; }
  set value(v) { this.attrs.value = String(v); }

  get classList() {
    const el = this;
    const list = () => el.className.split(/\s+/).filter(Boolean);
    return {
      contains: (c) => list().includes(c),
      add: (...cs) => { el.className = [...new Set([...list(), ...cs])].join(" "); },
      remove: (...cs) => { el.className = list().filter((c) => !cs.includes(c)).join(" "); },
      toggle: (c) => { const on = !list().includes(c); on ? this.classList.add(c) : this.classList.remove(c); return on; },
    };
  }

  getAttribute(name) { return this.attrs[name] !== undefined ? this.attrs[name] : null; }
  setAttribute(name, value) { this.attrs[name] = String(value); }
  removeAttribute(name) { delete this.attrs[name]; }

  get textContent() {
    return this.childNodes.map((n) => (typeof n === "string" ? n : n.textContent)).join("");
  }
  set textContent(text) {
    this.childNodes = [String(text)];
  }

  get innerHTML() {
    return this.childNodes.map(serialize).join("");
  }
  set innerHTML(html) {
    const holder = parse(String(html)).root;
    this.childNodes = [];
    for (const n of holder.childNodes) this.appendChild(n);
  }

  appendChild(node) {
    if (node instanceof Element) {
      if (node.parentElement) node.remove();
      node.parentElement = this;
    }
    this.childNodes.push(node);
    return node;
  }

  remove() {
    if (this.parentElement) {
      this.parentElement.childNodes = this.parentElement.childNodes.filter((n) => n !== this);
      this.parentElement = null;
    }
  }

  querySelectorAll(selector) {
    const found = [];
    const walk = (el) => {
      for (const child of el.children) {
        if (matches(child, selector)) found.push(child);
        walk(child);
      }
    };
    walk(this);
    return found;
  }

  querySelector(selector) {
    return this.querySelectorAll(selector)[0] || null;
  }

  addEventListener(type, fn) {
    (this.listeners[type] = this.listeners[type] || []).push(fn);
  }

  dispatchEvent(event) {
    for (let el = this; el; el = el.parentElement) {
      event.currentTarget = el;
      for (const fn of el.listeners[event.type] || []) fn.call(el, event);
      if (event.stopped) break;
    }
  }

  click() {
    this.dispatchEvent({ type: "click", target: this, stopped: false, stopPropagation() { this.stopped = true; }, preventDefault() {} });
  }
}

function serialize(node) {
  if (typeof node === "string") return node.replace(/&/g, "&amp;").replace(/</g, "&lt;").replace(/>/g, "&gt;");
  const attrs = Object.entries(node.attrs).map(([k, v]) => (v === "" ? ` ${k}` : ` ${k}="${v}"`)).join("");
  if (VOID.has(node.tag)) return `<${node.tag}${attrs}>`;
  return `<${node.tag}${attrs}>${node.childNodes.map(serialize).join("")}</${node.tag}>`;
}

// Build the tree. Problems a browser would quietly paper over are collected
// as warnings, because noticing them is part of learning HTML.
function parse(html) {
  const root = new Element("#root");
  const stack = [root];
  const warnings = [];
  let line = 1;
  const re = /<!--[\s\S]*?-->|<!doctype[^>]*>|<\/([a-zA-Z][\w-]*)\s*>|<([a-zA-Z][\w-]*)((?:\s+[^\s=>\/]+(?:\s*=\s*(?:"[^"]*"|'[^']*'|[^\s>]+))?)*)\s*\/?>|[^<]+|</gi;
  let m;
  while ((m = re.exec(html))) {
    const token = m[0];
    const top = stack[stack.length - 1];
    if (m[2]) {
      const tag = m[2].toLowerCase();
      const attrs = {};
      for (const a of m[3].matchAll(/([^\s=>\/]+)(?:\s*=\s*("([^"]*)"|'([^']*)'|([^\s>]+)))?/g)) {
        attrs[a[1].toLowerCase()] = decode(a[3] ?? a[4] ?? a[5] ?? "");
      }
      const el = new Element(tag, attrs);
      el.line = line;
      top.appendChild(el);
      if (RAW.has(tag)) {
        const end = html.toLowerCase().indexOf(`</${tag}`, re.lastIndex);
        const stop = end < 0 ? html.length : end;
        el.childNodes.push(html.slice(re.lastIndex, stop));
        line += (html.slice(m.index, stop).match(/\n/g) || []).length;
        const close = html.slice(stop).match(/^<\/\w+\s*>/);
        re.lastIndex = stop + (close ? close[0].length : 0);
        continue;
      } else if (!VOID.has(tag)) {
        stack.push(el);
      }
    } else if (m[1]) {
      const tag = m[1].toLowerCase();
      const at = stack.map((e) => e.tag).lastIndexOf(tag);
      if (VOID.has(tag)) {
        warnings.push(`line ${line}: <${tag}> never has a closing tag; </${tag}> is ignored`);
      } else if (at <= 0) {
        warnings.push(`line ${line}: </${tag}> closes nothing that is open; it is ignored`);
      } else {
        for (let i = stack.length - 1; i > at; i--) {
          warnings.push(`line ${line}: <${stack[i].tag}> (line ${stack[i].line}) was not closed before </${tag}>`);
        }
        stack.length = at;
      }
    } else if (!token.startsWith("<!")) {
      top.childNodes.push(decode(token));
    }
    line += (token.match(/\n/g) || []).length;
  }
  for (let i = stack.length - 1; i > 0; i--) {
    warnings.push(`<${stack[i].tag}> (line ${stack[i].line}) was never closed`);
  }
  return { root, warnings };
}

function label(el) {
  let s = el.tag;
  if (el.id) s += "#" + el.id;
  for (const c of el.className.split(/\s+/).filter(Boolean)) s += "." + c;
  return s;
}

function directText(el) {
  return el.childNodes.filter((n) => typeof n === "string").join(" ").replace(/\s+/g, " ").trim();
}

const SHOWN_ATTRS = ["href", "src", "alt", "type", "name", "value", "placeholder", "for", "lang", "rel", "action", "method"];

function outline(el, depth = 0, out = []) {
  for (const child of el.children) {
    if (RAW.has(child.tag)) continue;
    const attrs = SHOWN_ATTRS.filter((a) => child.attrs[a] !== undefined).map((a) => `${a}="${child.attrs[a]}"`).join(" ");
    // An element with text of its own shows all of its text, children's
    // included, so "Mix and <strong>fry</strong>." reads as one sentence.
    const text = directText(child) ? child.textContent.replace(/\s+/g, " ").trim() : "";
    out.push("  ".repeat(depth) + label(child) + (attrs ? ` [${attrs}]` : "") + (text ? `: ${text}` : ""));
    outline(child, depth + 1, out);
  }
  return out;
}

// ---------------------------------------------------------------- CSS

// One simple selector: tag, #id, .classes, or *.
function matchesCompound(el, compound) {
  const tag = compound.match(/^[a-zA-Z][\w-]*|^\*/);
  if (tag && tag[0] !== "*" && tag[0].toLowerCase() !== el.tag) return false;
  for (const id of compound.match(/#[\w-]+/g) || []) if (el.id !== id.slice(1)) return false;
  for (const cls of compound.match(/\.[\w-]+/g) || []) if (!el.classList.contains(cls.slice(1))) return false;
  return true;
}

function matchesOne(el, selector) {
  const parts = selector.trim().replace(/\s*>\s*/g, " > ").split(/\s+/);
  const test = (node, i) => {
    if (!node || node.tag === "#root" || !matchesCompound(node, parts[i])) return false;
    if (i === 0) return true;
    if (parts[i - 1] === ">") return test(node.parentElement, i - 2);
    for (let up = node.parentElement; up && up.tag !== "#root"; up = up.parentElement) {
      if (test(up, i - 1)) return true;
    }
    return false;
  };
  return test(el, parts.length - 1);
}

function matches(el, selectorList) {
  return selectorList.split(",").some((s) => matchesOne(el, s));
}

function specificity(selector) {
  const ids = (selector.match(/#[\w-]+/g) || []).length;
  const classes = (selector.match(/\.[\w-]+/g) || []).length;
  const tags = (selector.replace(/[#.][\w-]+/g, "").match(/[a-zA-Z][\w-]*/g) || []).length;
  return [ids, classes, tags];
}

function rules(root) {
  const out = [];
  let order = 0;
  for (const style of root.querySelectorAll("style")) {
    const css = style.textContent.replace(/\/\*[\s\S]*?\*\//g, "");
    for (const r of css.matchAll(/([^{}@]+)\{([^{}]*)\}/g)) {
      const decls = [];
      for (const d of r[2].split(";")) {
        const kv = d.match(/^\s*([\w-]+)\s*:\s*(.+?)\s*$/);
        if (kv) {
          const important = /!important$/.test(kv[2]);
          decls.push({ prop: kv[1].toLowerCase(), value: kv[2].replace(/\s*!important$/, ""), important });
        }
      }
      for (const sel of r[1].split(",")) {
        out.push({ selector: sel.trim(), specificity: specificity(sel), order: order++, decls });
      }
    }
  }
  return out;
}

const INHERITED = new Set(["color", "font-size", "font-family", "font-weight", "font-style", "text-align", "line-height", "visibility", "letter-spacing"]);

function compare(a, b) {
  if (a.important !== b.important) return a.important ? 1 : -1;
  for (let i = 0; i < 3; i++) if (a.specificity[i] !== b.specificity[i]) return a.specificity[i] - b.specificity[i];
  return a.order - b.order;
}

// Every element's winning declarations, then inheritance from its parent.
function computeStyles(root) {
  const all = rules(root);
  const result = new Map();
  const visit = (el, inherited) => {
    const won = {};
    for (const r of all) {
      if (!matchesOne(el, r.selector)) continue;
      for (const d of r.decls) {
        const candidate = { ...d, specificity: r.specificity, order: r.order, from: r.selector };
        if (!won[d.prop] || compare(candidate, won[d.prop]) >= 0) won[d.prop] = candidate;
      }
    }
    for (const d of (el.attrs.style || "").split(";")) {
      const kv = d.match(/^\s*([\w-]+)\s*:\s*(.+?)\s*$/);
      if (kv) {
        const candidate = { prop: kv[1], value: kv[2], important: false, specificity: [9, 0, 0], order: 1e9, from: "style attribute" };
        if (!won[kv[1]] || compare(candidate, won[kv[1]]) >= 0) won[kv[1]] = candidate;
      }
    }
    const final = { ...inherited };
    for (const [prop, d] of Object.entries(won)) final[prop] = { value: d.value, from: d.from };
    result.set(el, final);
    const pass = {};
    for (const [prop, v] of Object.entries(final)) {
      if (INHERITED.has(prop)) pass[prop] = v.inheritedFrom ? v : { value: v.value, inheritedFrom: label(el) };
    }
    for (const child of el.children) {
      if (RAW.has(child.tag)) continue;
      // Browsers give links their own colour, so a link does not take its
      // parent's.
      const given = child.tag === "a" ? Object.fromEntries(Object.entries(pass).filter(([p]) => p !== "color")) : pass;
      visit(child, given);
    }
  };
  for (const child of root.children) visit(child, {});
  return result;
}

function styleReport(root) {
  const styles = computeStyles(root);
  const out = [];
  const body = root.querySelector("body") || root;
  const walk = (el, depth) => {
    for (const child of el.children) {
      if (RAW.has(child.tag)) continue;
      const s = styles.get(child) || {};
      out.push("  ".repeat(depth) + label(child));
      for (const p of Object.keys(s).sort()) {
        const v = s[p];
        const why = v.inheritedFrom ? `inherited from ${v.inheritedFrom}` : `from ${v.from}`;
        out.push("  ".repeat(depth + 1) + `${p}: ${v.value}   (${why})`);
      }
      walk(child, depth + 1);
    }
  };
  walk(body, 0);
  return out;
}

// ---------------------------------------------------------------- scripts

function runScripts(root, file) {
  const documentElement = root.querySelector("html") || root;
  const document = {
    documentElement,
    get body() { return root.querySelector("body"); },
    get title() { const t = root.querySelector("title"); return t ? t.textContent : ""; },
    querySelector: (s) => root.querySelector(s),
    querySelectorAll: (s) => root.querySelectorAll(s),
    getElementById: (id) => root.querySelectorAll("*").find((e) => e.id === id) || null,
    createElement: (tag) => new Element(tag),
  };
  const timers = [];
  const context = vm.createContext({
    document,
    console: { log: (...a) => process.stdout.write(util.format(...a) + "\n") },
    setTimeout: (fn, ms = 0) => timers.push({ fn, ms, seq: timers.length }),
    // PC only: play the user.
    sim: {
      click: (selector) => {
        const el = root.querySelector(selector);
        if (!el) throw new Error(`sim.click: nothing matches ${selector}`);
        el.click();
      },
      type: (selector, text) => {
        const el = root.querySelector(selector);
        if (!el) throw new Error(`sim.type: nothing matches ${selector}`);
        el.value = text;
        el.dispatchEvent({ type: "input", target: el, stopped: false, stopPropagation() {}, preventDefault() {} });
      },
    },
  });
  context.window = context;
  for (const script of root.querySelectorAll("script")) {
    new vm.Script(script.textContent, { filename: file, lineOffset: script.line - 1 }).runInContext(context);
  }
  timers.sort((a, b) => a.ms - b.ms || a.seq - b.seq).forEach((t) => t.fn());
}

// ---------------------------------------------------------------- main

function main() {
  const [mode, file] = process.argv.slice(2);
  const { root, warnings } = parse(fs.readFileSync(file, "utf8"));
  if (mode === "check") {
    for (const w of warnings) process.stderr.write(`warning: ${w}\n`);
  } else if (mode === "outline") {
    console.log(outline(root).join("\n"));
  } else if (mode === "styles") {
    console.log(styleReport(root).join("\n"));
  } else if (mode === "run") {
    runScripts(root, file);
  } else {
    console.error("usage: node web.js check|outline|styles|run page.html");
    process.exit(2);
  }
}

if (require.main === module) {
  main();
}

module.exports = { parse, outline, styleReport, matches };
