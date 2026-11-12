const score = 85;

if (score >= 90) {
  console.log("A");
} else if (score >= 80) {
  console.log("B");
} else if (score >= 50) {
  console.log("C");
} else {
  console.log("F");
}
console.log("");

// basicamente C mas usas === ao inves de ==
// tal como C apenas a primeira condiçao verdadeira e ativa
// ent tens sempre de fazer por ordem
// ou meter varias codiçoes de && ou ||

// Falsy in JavaScript: false, 0, "", null, undefined, NaN.
// So far, similar to Python. Here is where they diverge:

//Boolean([]); = True
//Boolean({}); = True

// uma array vazia ou objeto vazio sao os dois verdade em js
// na qual em python isto e o oposto
// se sabes python "if (array)" nunca em lenght
// Em js usas "if (array.lenght)" ou "if (array.lenght === 0)"

let x = 3;
if (x = 5) { }

// muda valor de x para 5 e dps verifica se e 5
// isto executa sem erro nem aviso
// ao contrario de gcc ou g++
// node nem sequer avisa sobre isto por default

const n = -5;
console.log(Boolean(n));

console.log("");

// Exercise

const number = -5;

if (number < 0) {
  console.log("negative");
} else if (number > 0) {
  console.log("positive");
} else {
  console.log("0");
}
