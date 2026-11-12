// skiped 8 and 9 cause its objects and i dont want objects
// its my use of free will

const { error } = require("console");

function divide(a, b) {
  if (b === 0) throw new Error("division by zero");
  return a / b;
}

try {
  console.log(divide(10, 2));
  console.log(divide(1, 0));
} catch (e) {
  console.log(`caught: ${e.message}`); // se apanhar erro print apanhou: error.message
} finally {
  console.log("always runs");
}
// a logica e simples
// try tenta o comando neste caso tenta fazer output de 10 / 2 e 1 / 0
// a funçao diz se numero dividido for 0 atira um erro novo
// ent o try tenta o console.log divisao
// e se apanhar um erro com catch (e) e sendo error
// faz print da menssagem de erro que nos atiramos caso tente divisao por 0
// e finally diz finalmente caso tenha apanhado o erro continua a correr
// ou caso n tenha tbm continua
// e bastante similar com o try expect do py
//
// output:
// 5
// caught: division by zero
// always runs

// TypeError significa que chamaste algo que n esta na funçao or leste uma propriadade NULL/nao defenida
// RangeError um numero fora de um alcançe permitido
// SyntaxError codigo invalido ou json invalido
//
// Running:
// TypeError: Cannot read properties of null (reading 'foo')
// SyntaxError: Expected property name or '}' in JSON at position 1 (line 1 column 2)

class ValidationError extends Error { // podes criar os teus proprios erros
  constructor(msg) {
    super(msg);
    this.name = "ValidationError";
  }
}
// output:
// ValidationError: invalid age, instanceof Error -> true
// extende erro mais super(msg) para fazer .message, e o shape standerd
// a mesma mecanica do modulo 9 que pulei

//catch (e) {
//  if (e instanceof ValidationError) {}
//  else throw e; // nao e meu passa para o proximo
//}

//try {
//  risky();
//} finally {
//  cleanup();
//}

// exercise

function Exercise() {
  function division(a, b) {
    if (b === 0) {
      throw new Error("division by 0");
    }
    return a / b;
  }
  try {
    console.log(division(10, 2));
    console.log(division(1, 0));
  } catch (e) {
    console.log(`error: ${e.message}`);
  } finally {
    console.log("continues");
  }
}

Exercise();
