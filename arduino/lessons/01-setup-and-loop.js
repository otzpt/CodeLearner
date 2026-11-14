module.exports = {
  id: "setup-and-loop",
  title: "setup and loop",
  titlePt: "setup e loop",
  screens: [
    [
      "An Arduino is a small computer on a board, made to control lights, motors and sensors. Its programs are called sketches, written in C++ with the Arduino library.",
      "This course runs sketches on your PC with a stand-in for the board: no hardware needed. Pins become variables, and time is simulated, so a one-second wait takes no real time. Where the PC and a real board differ, the lesson says so.",
    ],
    [
      "Every sketch has two functions. setup() runs once, when the board powers on. loop() then runs again and again, until the power is cut. Serial sends text to the computer it is plugged into:",
      { code: 'void setup() {\n  Serial.begin(9600);\n  Serial.println("starting");\n}\n\nvoid loop() {\n  Serial.println("loop runs");\n}' },
      "On a board, loop() would never stop. Here it stops after 3 times, so the examples finish. Serial.begin(9600) sets the speed of the connection; 9600 is a common choice.",
    ],
    [
      "Serial.print does not end the line; Serial.println does. Text goes in double quotes, numbers do not, and every statement ends with a semicolon:",
      { code: 'void setup() {\n  Serial.begin(9600);\n  Serial.print("two plus three is ");\n  Serial.println(2 + 3);\n}\n\nvoid loop() {\n}' },
      "An empty loop() is fine: the board just does nothing forever. Both functions must exist, even if empty.",
      { quiz: "How many times does setup() run?", choices: ["once", "forever"], answer: 1, why: "setup runs once at power-on; loop runs forever after it." },
    ],
    [
      { tip: "setup() once, loop() forever. Serial.begin before printing." },
      {
        exercise: "hello",
        task: "Write a sketch that prints \"Ana\" once in setup, then prints \"tick\" every time loop runs (3 times here).",
        expected: ["Ana", "tick", "tick", "tick"],
        hints: ['In setup: Serial.begin(9600); Serial.println("Ana");', 'In loop: Serial.println("tick");'],
        solution: 'void setup() {\n  Serial.begin(9600);\n  Serial.println("Ana");\n}\n\nvoid loop() {\n  Serial.println("tick");\n}',
      },
    ],
  ],
  recapPt: [
    "Um sketch tem setup() (corre uma vez) e loop() (corre sempre).",
    "Aqui o loop() para ao fim de 3 vezes; numa placa nunca para.",
    "Serial.begin(9600) e depois Serial.print / Serial.println mostram texto.",
  ],
  recap: [
    "A sketch has setup() (runs once) and loop() (runs forever).",
    "Here loop() stops after 3 times; on a board it never stops.",
    "Serial.begin(9600), then Serial.print / Serial.println show text.",
  ],
};
