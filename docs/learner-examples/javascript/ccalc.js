const prompt = require('prompt-sync')();

function calc(a, b, operator) {
  let result;
  if (operator === "+") {
    result = a + b;
  } else if (operator === "-") {
    result = a - b;
  } else if (operator === "*") {
    result = a * b;
  } else if (operator === "/") {
    result = a / b;
  } else {
    console.log("invalid operator(use only +, -, *, /)");
  }

  return result;
}

let a = Number(prompt("insert the first number: "));
let b = Number(prompt("insert the second number: "));
let operator = prompt("insert the operator: ");

console.log(calc(a, b, operator));
