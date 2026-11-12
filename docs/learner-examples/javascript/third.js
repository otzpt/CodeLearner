// js n tem scanf nem input ele n espera pelo input
// ent ler uma linha é asynchronous
// e tu esperas por ela

const readline = require("node:readline");
const rl = readline.createInterface({ input: process.stdin });
const it = rl[Symbol.asyncIterator]();

async function ask(prompt) {
  process.stdout.write(prompt);
  const { value } = await it.next();
  return value;
}
// todas as funçoes que pedirem uma funçao async neste caso
// a funçao ask tem de ser tbm async
// async e contagioso por toda a correia de chamadas
// da funçao ask, ate ao codigo de alto-nivel que começa tudo

// se tu testar um script a mandar input com:
// printf "a\nb\n" | node script.js
// e o script pergunta 2 perguntas com o
// readline ou promises library's rl.question()
// a segunda pergunta pode falhar silenciosamente
// e ficar pendurada infinitamente sem nenhum erro
// isso acontece porque printf fecha o momento que termina
// de escrever e uma vez que stdin ja chegou ao
// end-of-input a segunda pergunta nunca e resolvida
//
// um terminaç de verdade nunca termina stdin enquanto
// tu o tens aberto ent isso nunca afeta o programa em si
// ou o decorrer do mesmo,
// MAS afeta testing se tiveres a testar o script com input
// artificial ao inves de interagires com ele diretamente
// se tiveres a testar algo com input n direto
// e o programa ficar parado na segunda prompt
// esse e o motivo

async function main() {
  process.stdout.write("What is your name? ");
  const { value: name } = await it.next();
  console.log(`Hello, ${name}`);
  rl.close();
}
// never forget ;
main();
