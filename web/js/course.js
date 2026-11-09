// The helpers these pages use. Plain browser JavaScript, and by the end of
// the course every line here will be something you have been taught.

// Runs one demo at the moment the parser reaches it, because module 1 is about
// exactly that. print() adds a line to the box below the demo's code, and
// anything the demo throws is printed instead of being lost.
function demo(id, run) {
  const lines = [];

  // The box comes after the script in the page, so at first it does not exist.
  function show() {
    const box = document.getElementById(id);
    if (box !== null) {
      box.textContent = lines.join("\n");
    }
  }

  function print(...values) {
    lines.push(values.map((v) => (typeof v === "string" ? v : JSON.stringify(v))).join(" "));
    show();
  }

  document.addEventListener("DOMContentLoaded", show);
  run(print).catch((error) => print(error.name + ": " + error.message));
}

// One listener on the document handles every question on the page.
document.addEventListener("submit", (event) => {
  const form = event.target;
  if (!form.matches(".question")) {
    return;
  }
  event.preventDefault();

  const normalize = (text) => text.trim().toLowerCase();
  const right = normalize(form.elements.answer.value) === normalize(form.dataset.answer);
  const verdict = form.querySelector(".verdict");
  verdict.hidden = false;
  verdict.className = "verdict " + (right ? "right" : "wrong");
  verdict.textContent = right ? "Correct." : "The answer is: " + form.dataset.answer;
  form.querySelector(".why").hidden = false;
});
