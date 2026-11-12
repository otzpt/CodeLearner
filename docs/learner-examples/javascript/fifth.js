console.log("Mesma linha:");
for (let i = 0; i <= 5; i++) {
  process.stdout.write(i + " ");
  // escrever na mesma linha usa-se
  // process.stdout.write(var + "o que queres");
  // 1 2 3 4 ...
  // para escrever em linhas separadas segue o exemplo abaixo
}
console.log(" ");
console.log("");
console.log("Linhas separadas: ");
for (let i = 0; i <= 5; i++) {
  console.log(`${i} `);
  // assim escreves em linhas separadas
  // 1
  // 2
  // 3
  // ...
}
console.log("");
// loops for em js e basicamente a mesma coisa que em c

const fruits = ["apple", "banana", "cherry"];
console.log(" VALORES ");
for (const f of fruits) console.log(f); // valores
console.log("");
console.log(" CHAVES EM FORMATO STRING ")
for (const i in fruits) console.log(i); // chaves em formato strings

// for in da como output os indices da array "1", "2", ...
// nao os elemtos;
// usar for in numa array e quase sempre um erro
// lembra disto como
// of para receber os valores: "apple", "banana", "cherry"
// in para receber os indices como str: "1", "2", "3"
console.log(" ");
console.log(" ");
// WHILE'S
let n = 3;
while (n > 0) {
  process.stdout.write(n + " ");
  n--;
}

// EXERCISE
console.log(" ");
console.log(" ");
console.log("EXERCISE");

function EXERCISE() {
  let n = 1;
  const array = ["apple", "banana", "cherry"];
  while (n <= 5) {
    console.log(n);
    n++;
  }

  for (let i of array) console.log(`- ${i}`);
}

EXERCISE();
