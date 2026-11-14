module.exports = {
  id: "serial-input",
  title: "Commands from the computer: Serial input",
  titlePt: "Comandos do computador: entrada Serial",
  screens: [
    [
      "Serial works both ways: the computer can send text to the board. Serial.available() says how many characters are waiting; Serial.parseInt() reads a whole number from them:",
      { code: 'void setup() {\n  Serial.begin(9600);\n  Serial.print("Brightness (0-255)? ");\n}\n\nvoid loop() {\n  if (Serial.available() > 0) {\n    int level = Serial.parseInt();\n    Serial.print("set to ");\n    Serial.println(level);\n  }\n}', output: ["Brightness (0-255)? {180}", "set to 180"] },
      "The bold part is what was typed. On a board you would type it in the Serial Monitor of the Arduino IDE.",
    ],
    [
      "Serial.readStringUntil('\\n') reads a whole line of text, as a String. trim() removes the new-line and spaces around it, and == compares Strings:",
      { code: 'void setup() {\n  Serial.begin(9600);\n  pinMode(LED_BUILTIN, OUTPUT);\n}\n\nvoid loop() {\n  if (Serial.available() > 0) {\n    String command = Serial.readStringUntil(\'\\n\');\n    command.trim();\n    if (command == "on") {\n      digitalWrite(LED_BUILTIN, HIGH);\n      Serial.println("LED on");\n    } else {\n      Serial.println("unknown command");\n    }\n  }\n}', output: ["{on}", "LED on"] },
      { quiz: "What does Serial.available() tell you?", choices: ["whether the board is on", "how many characters are waiting to be read"], answer: 2, why: "Read only when it is more than 0." },
    ],
    [
      {
        exercise: "serial",
        task: 'In setup, print "Seconds? " (no new line). In loop, when characters are waiting, read a number with parseInt and print it times 1000 followed by " ms".',
        expected: ["Seconds? {3}", "3000 ms"],
        hints: ['Serial.print("Seconds? ");', "if (Serial.available() > 0) { long s = Serial.parseInt(); Serial.print(s * 1000); Serial.println(\" ms\"); }"],
        solution: 'void setup() {\n  Serial.begin(9600);\n  Serial.print("Seconds? ");\n}\n\nvoid loop() {\n  if (Serial.available() > 0) {\n    long seconds = Serial.parseInt();\n    Serial.print(seconds * 1000);\n    Serial.println(" ms");\n  }\n}',
      },
    ],
  ],
  recapPt: [
    "Serial.available() diz quantos caracteres estão à espera.",
    "Serial.parseInt() lê um número inteiro.",
    "Serial.readStringUntil('\\n') lê uma linha; .trim() limpa-a.",
  ],
  recap: [
    "Serial.available() says how many characters are waiting.",
    "Serial.parseInt() reads a whole number.",
    "Serial.readStringUntil('\\n') reads a line; .trim() cleans it.",
  ],
};
