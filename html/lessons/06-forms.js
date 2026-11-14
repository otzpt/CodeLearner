module.exports = {
  id: "forms",
  title: "Forms: asking the visitor",
  titlePt: "Formulários: perguntar ao visitante",
  screens: [
    [
      "A form collects what the visitor types. input is a box to type in; type says what kind: text, email, password, number, checkbox. A label says what the box is for, and its for attribute names the input's id:",
      { code: '<form>\n  <label for="email">Email</label>\n  <input id="email" type="email" name="email">\n  <button type="submit">Sign up</button>\n</form>' },
      "Clicking a label puts the cursor in its box, and screen readers read the label aloud for it. Every input needs one.",
    ],
    [
      "name is what the value is called when the form is sent. placeholder is grey hint text inside an empty box; it is not a label, because it disappears as soon as the visitor types:",
      { code: '<label for="age">Age</label>\n<input id="age" type="number" name="age" placeholder="e.g. 20">' },
      "type=\"email\" or \"number\" makes the browser check the format, and phones show the right keyboard.",
      { quiz: "How is a label tied to its input?", choices: ["it must come right before it", "its for matches the input's id"], answer: 2, why: "for=\"x\" points at the element with id=\"x\"." },
    ],
    [
      { tip: "Every input gets a label with a matching for and id." },
      {
        exercise: "forms",
        task: 'Make a form with a label "Name" for a text input with the id and name "name", and a submit button "Send".',
        expected: ["form", '  label [for="name"]: Name', '  input#name [type="text" name="name"]', '  button [type="submit"]: Send'],
        starter: "<!-- write your HTML below -->",
        hints: ['<label for="name">Name</label>', '<input id="name" type="text" name="name"> and <button type="submit">Send</button>, all inside <form>'],
        solution: '<form>\n  <label for="name">Name</label>\n  <input id="name" type="text" name="name">\n  <button type="submit">Send</button>\n</form>',
      },
    ],
  ],
  recapPt: [
    "<form> junta campos; <input type=\"...\"> é uma caixa para escrever.",
    "Cada input precisa de um <label for=\"id\">.",
    "name é o nome do valor enviado; placeholder é só uma dica.",
    "<button type=\"submit\"> envia o formulário.",
  ],
  recap: [
    "<form> groups fields; <input type=\"...\"> is a box to type in.",
    "Every input needs a <label for=\"id\">.",
    "name is what the sent value is called; placeholder is only a hint.",
    "<button type=\"submit\"> sends the form.",
  ],
};
