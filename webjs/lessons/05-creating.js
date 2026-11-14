const { page } = require("./01-scripts.js");

module.exports = {
  id: "creating",
  title: "Adding and removing elements",
  titlePt: "Juntar e tirar elementos",
  screens: [
    [
      "document.createElement makes a new element, not yet on the page. appendChild puts it inside another element, at the end:",
      { code: page('<ul id="list"></ul>', 'const list = document.querySelector("#list");\nfor (const fruit of ["apple", "pear"]) {\n  const li = document.createElement("li");\n  li.textContent = fruit;\n  list.appendChild(li);\n}\nconsole.log(list.children.length);\nconsole.log(list.innerHTML);') },
      "innerHTML gives the HTML inside an element, as text.",
    ],
    [
      ".remove() takes an element off the page:",
      { code: page('<ul id="list">\n  <li>keep</li>\n  <li class="old">delete me</li>\n</ul>', 'document.querySelector(".old").remove();\nconsole.log(document.querySelectorAll("li").length);') },
      "Setting innerHTML can also add elements, but never with text a visitor typed: if it contains tags, they become real elements, scripts included. Use createElement and textContent for anything from the visitor.",
      { quiz: "Where does appendChild put the new element?", choices: ["at the start", "at the end, inside the parent"], answer: 2, why: "It becomes the last child." },
    ],
    [
      {
        exercise: "creating",
        task: 'The page has an input "#item", a button "#add" and an empty ul "#cart". On click, add an li with the typed text to #cart, then clear the input (value = ""). Add "milk" then "tea", then print the number of items and the cart\'s innerHTML.',
        expected: ["2", "<li>milk</li><li>tea</li>"],
        starter: page('<input id="item">\n<button id="add">Add</button>\n<ul id="cart"></ul>', "// your code here"),
        hints: ['const li = document.createElement("li"); li.textContent = input.value; cart.appendChild(li);', "Then input.value = \"\"; after the clicks, print cart.children.length and cart.innerHTML."],
        solution: page('<input id="item">\n<button id="add">Add</button>\n<ul id="cart"></ul>', 'const input = document.querySelector("#item");\nconst cart = document.querySelector("#cart");\ndocument.querySelector("#add").addEventListener("click", () => {\n  const li = document.createElement("li");\n  li.textContent = input.value;\n  cart.appendChild(li);\n  input.value = "";\n});\nsim.type("#item", "milk");\nsim.click("#add");\nsim.type("#item", "tea");\nsim.click("#add");\nconsole.log(cart.children.length);\nconsole.log(cart.innerHTML);'),
      },
    ],
  ],
  recapPt: [
    "document.createElement(\"li\") cria um elemento; appendChild põe-no na página.",
    ".remove() tira um elemento.",
    "Nunca ponhas texto do visitante em innerHTML: usa textContent.",
  ],
  recap: [
    "document.createElement(\"li\") makes an element; appendChild puts it on the page.",
    ".remove() takes an element off.",
    "Never put the visitor's text in innerHTML: use textContent.",
  ],
};
