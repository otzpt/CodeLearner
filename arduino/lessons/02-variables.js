module.exports = {
  id: "variables",
  title: "Variables and their sizes",
  titlePt: "Variáveis e os seus tamanhos",
  screens: [
    [
      "A variable is a named box for a value, with a type written before its name. A variable made outside both functions is global: it keeps its value between runs of loop():",
      { code: 'int count = 0;\n\nvoid setup() {\n  Serial.begin(9600);\n}\n\nvoid loop() {\n  count = count + 1;\n  Serial.print("loop number ");\n  Serial.println(count);\n}' },
      "A variable made inside loop() would start again from scratch every time.",
    ],
    [
      "A board has very little memory, so the size of each type matters. On the most common board, the Uno, an int is only 2 bytes: it holds -32768 to 32767. On a PC it is 4 bytes. Types with the size in their name are the same everywhere:",
      { code: 'void setup() {\n  Serial.begin(9600);\n  Serial.println(sizeof(uint8_t));\n  Serial.println(sizeof(int16_t));\n  Serial.println(sizeof(uint32_t));\n}\n\nvoid loop() {\n}' },
      "uint8_t is 1 byte, 0 to 255; int16_t is 2 bytes; uint32_t is 4 bytes, 0 to about 4 billion. The u means unsigned: no negative numbers.",
    ],
    [
      "Going past a type's range wraps around to the other end, without any error:",
      { code: 'uint8_t level = 250;\n\nvoid setup() {\n  Serial.begin(9600);\n}\n\nvoid loop() {\n  level = level + 3;\n  Serial.println(level);\n}' },
      "It prints 253, then 256 does not fit in a byte and becomes 0, then 3. Pick a type big enough for every value it can reach.",
      { quiz: "On an Uno, can an int hold 40000?", choices: ["yes", "no"], answer: 2, why: "An Uno int goes up to 32767. Use int32_t or uint16_t." },
    ],
    [
      {
        exercise: "variables",
        task: "Keep a global uint16_t total starting at 0. Each time loop runs, add 1000 to it and print it.",
        expected: ["1000", "2000", "3000"],
        hints: ["uint16_t total = 0; above setup.", "In loop: total = total + 1000; Serial.println(total);"],
        solution: 'uint16_t total = 0;\n\nvoid setup() {\n  Serial.begin(9600);\n}\n\nvoid loop() {\n  total = total + 1000;\n  Serial.println(total);\n}',
      },
    ],
  ],
  recapPt: [
    "Variáveis globais (fora das funções) guardam o valor entre voltas do loop().",
    "Num Uno um int tem 2 bytes (-32768 a 32767); num PC tem 4.",
    "uint8_t, int16_t, uint32_t têm o mesmo tamanho em todo o lado.",
    "Passar o limite dá a volta sem aviso.",
  ],
  recap: [
    "Global variables (outside the functions) keep their value between loops.",
    "On an Uno an int is 2 bytes (-32768 to 32767); on a PC it is 4.",
    "uint8_t, int16_t, uint32_t are the same size everywhere.",
    "Going past the range wraps around with no warning.",
  ],
};
