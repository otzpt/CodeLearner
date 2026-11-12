const prompt = require('prompt-sync')();
const MAX = 100;
// generate the random number with a max of 100 and a min of 1
function random_number(MAX) {
  return Math.floor(Math.random() * MAX) + 1;
}

let number = random_number(MAX);

while (true) {
  let guess = Number(prompt("Guess a random number between 1 and 100: ")); // use Number() to receive a number and not a string

  if (guess === number) {
    console.log("Correct");
    break;
  } else if (guess <= number) {
    console.log("Too low");
  } else if (guess >= number) {
    console.log("Too high");
  }
}
