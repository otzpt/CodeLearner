let age = 25; // faz a var ser alteravel e n estatica
const height = 1.75; // const n permite alteraçao do valor
const name = "jonh";
// js n tem um tipo diefrente para float, double e int
// 1.75 e nummber e 1 tbm e number
// usa-se typeof para saber o tipo do valor ou var:
typeof age; // -> outputs number se eu estivesse a usar console.log()
typeof height; // tbm daria output number para js n existe float ou double
console.log(typeof age);
console.log(typeof height);
console.log(typeof name);
// all of the above outputs:
// number
// number
// string
1 == "1"; // da verdadeiro; == coverte e dps compara
1 === "1"; // da falso; === compara tipo e valor sem conversao
// usa-se sempre === ao inves de == n por escolha mas para
// evitar erros e problemas
// n dependo do design nem preferencia === e correto e conciso
// == pode ter surpresas que ngm espera
// mesma coisa para != e !== usa-se sempre !== pelo mesmo motivo

const check1 = NaN === NaN; // false
console.log(check1);
const check = Number.isNaN(NaN); // true
console.log(check);
// Nan num check normal nunca e igual a si mesmo
// Number.isNan(NaN) e o c
// heck correto
// === com NaN e sempre falso

age = 25;
age = "now a string";
// funciona; let permite mudar a var de tipos e valores
//
// IMPORTANTE
// const so se pode adicionar ou remover valores
// E apenas caso seja uma array ou objeto
// nunca mudar o valor diretamente
// let permite livremente mudar para str para int etc
// e mudar valores livremente

console.log(typeof age);

// para floats e mesma coisa que todas as outras linguagens
// mesmo binario para centimos usa o dinheiro como um todo
// 2.00; 0.00
// 0.1 + 0.2 = 0.30000000000000004
let result = 0.1 + 0.2;
console.log(result);

// EXERCISE
console.log("");
console.log("Exercise result:");
console.log("")

const text = "5";
const number = 5;
console.log(typeof text, typeof number);

const x1 = text == number;
const x2 = text === number;
console.log(x1, x2);

//summary
// sempre === nunca ==; == converte com regras
// que ngm se lembra
//
// NaN !== NaN; check com Number.isNaN(NaN)
// let/const controlam reassignment, n tipo ou mutabilidade
