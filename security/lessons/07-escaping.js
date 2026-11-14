module.exports = {
  id: "escaping",
  title: "Putting user text into a web page",
  titlePt: "Pôr texto do utilizador numa página web",
  screens: [
    [
      "A web page is text in a language called HTML. Tags like <b>bold</b> mark it up, and <script> holds code the browser runs. A site that puts a visitor's comment straight into its page lets the comment add tags:",
      { code: 'comment = "<script>steal(document.cookie)</script>"\npage = f"<p>{comment}</p>"\nprint(page)' },
      "Every visitor's browser would run that script as if the site wrote it. This is called cross-site scripting, or XSS.",
    ],
    [
      "The fix is to escape the text: turn the characters that mean something in HTML into codes that just show them. html.escape does it:",
      { code: 'import html\n\ncomment = "<script>steal(document.cookie)</script>"\npage = f"<p>{html.escape(comment)}</p>"\nprint(page)' },
      "&lt; shows as < on screen, but the browser never treats it as the start of a tag.",
      { quiz: "When should user text be escaped?", choices: ["only if it looks dangerous", "every time it goes into a page"], answer: 2, why: "You cannot reliably spot every attack; escape everything." },
    ],
    [
      { tip: "The same rule as SQL and commands: user text is data, never code. Keep it apart with placeholders, lists or escaping." },
      {
        exercise: "escaping",
        task: 'Write comment_html(author, text) that returns <li><b>AUTHOR</b>: TEXT</li> with both values escaped. Print it for ("Ana", "hi!") and for ("<i>Rui</i>", "a & b").',
        expected: ["<li><b>Ana</b>: hi!</li>", "<li><b>&lt;i&gt;Rui&lt;/i&gt;</b>: a &amp; b</li>"],
        hints: ['return f"<li><b>{html.escape(author)}</b>: {html.escape(text)}</li>"', "& becomes &amp; too."],
        solution: 'import html\n\ndef comment_html(author, text):\n    return f"<li><b>{html.escape(author)}</b>: {html.escape(text)}</li>"\n\nprint(comment_html("Ana", "hi!"))\nprint(comment_html("<i>Rui</i>", "a & b"))',
      },
    ],
  ],
  recapPt: [
    "Texto do utilizador posto numa página sem cuidado pode juntar tags e scripts (XSS).",
    "html.escape transforma < > & \" em códigos que só se mostram.",
    "Regra geral: texto do utilizador é dados, nunca código.",
  ],
  recap: [
    "User text put into a page carelessly can add tags and scripts (XSS).",
    "html.escape turns < > & \" into codes that only display.",
    "General rule: user text is data, never code.",
  ],
};
