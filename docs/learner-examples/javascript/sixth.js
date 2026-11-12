const a = [1, 2, 3];
const b = a;
b.push(4);// var.push(o que queres) enfia um valor na array
console.log(a);
// da print de [1,2,3,4] pq b e a partilham o mesmo endereço
// ent quando alteras 1 alteras os 2
// este e um bug classico da programaçao
// e const proteje a alteraçao de valore n adiçoes

let x = [10, 9, 2];
x.sort();
// outputs [10, 2, 9] ao inves de [2, 9 ,10]
// isso e porque .sort() converte cada elemento de um array
// para uma str antes de organiza a array
// organizando-a por ordem alfabeta
// para organizar numeros tens de passar um comparador:
let y = [10, 9, 2];
y.sort((x, y) => x - y);
//outputs [2, 9, 10];
// .sort() tbm modifica a array e a retorna
// a maioria dos metodos de array faz um ou outro, n os dois
// slice() cria uma copia; splice() muta-

const nums = [1, 2, 3];
nums.map(x => x * 2); // outputs [2, 4, 6]
nums.filter(x => x % 2 === 0); // outputs [2]
nums.reduce((sum, x) => sum + x, 0); // outputs 6
// cada uma retorna uma nova array (map/filter)
// ou um valor unico(reduce)
// nenhuma das 3 toca na variavel original diratamente

//Exercise

console.log("");
console.log("Exercise");

function exercise() {
  let nums = [3, 1, 2];
  // use [...array] so the array keeps its value
  console.log([...nums].sort((x, y) => x - y));
  console.log(nums) // you dont need a for loop to print an array
  // for (let i of nums) console.log(i); -> this was a dumb try
}

exercise();
