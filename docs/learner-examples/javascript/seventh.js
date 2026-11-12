// outro exemplo de uma funçao mais curta
let name = "coelho"
function greet(name) { return `helo, ${name}`; }

const add = (a, b) => a + b; // forma mais curta de escrever uma funçao

// funçao de return basica e curta
let base = 3;
function power(base, exp = 2) { return base ** exp; }

const x = greet(name);
const y = power(base, 2);
console.log(x);
console.log(y);
console.log(add); // prints [function : add] diz o que e e o nome
const z = add(2, 3);//obvio
console.log(z);

// This and super basic object

const objecto = { // var = {} "{}" significa objecto em js
  name: "Ana", // this names the object
  // regular e arrow significa o metodo de funçao
  regular: function () { return this.name; }, // this = este objeto; este objeto retorna valor 'name'
  arrow: () => { return this.name; }, // n pode retorna undefined
};

// os dois sao chamados da mesma forma objecto.metodo(objecto.regular)
// a diferença esta em como cada tipo de funçao decide o que "this" significa
// uma funçao regular "this" e qualquer coisa que fica fora do ponto this.qualquer_coisa
// ja a "arrow:"(metodo de seta)isso usa qualquer coisa onde foi escrito n onde e chamado
// porque uma funçao de seta n tem o seu this aqui isto e fora de qualquer objeto
// ou seja this.name em arrow: e indefenido

//Rule of thumb: methods on an object use `function`. Arrow
// functions are for callbacks, where you usually WANT this to
// stay whatever it was outside -- map/filter callbacks, timers.

const fns = []; // array vazia
for (var i = 0; i < 3; i++) {
  fns.push(() => i);// arrow function dentro de for e .push
}
// output disso com var = [3, 3, 3] nao [0, 1, 2]
// var n esta projetada para o corpo do loop for
// tem apenas um i para o loop inteiro
// e no tempo em que cada funçao tenha acabado de correr i ja e 3
// cada iteraçao captura a mesma variavel
for (let j = 0; j < 3; j++) {
  fns.push(() => j);
}
// se usares let o output sera o pretendido [0, 1, 2]
// porque let cria uma ligaçao fresca a cada iteraçao
// ent cada enclosure captura o seu proprio j
// isso apenas e razao suficiente para nunca usar var num loop
// na qual o seu corpo cria uma funçao

// exercise
console.log("");
console.log("");
console.log("");

function exercise() {
  let pair = [1, 2];
  function swap(pair) {
    return [...pair].reverse();
  }
  return console.log(swap(pair))
}

exercise();
