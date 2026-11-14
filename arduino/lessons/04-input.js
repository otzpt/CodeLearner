module.exports = {
  id: "input",
  title: "Buttons: digital input",
  titlePt: "Botões: entrada digital",
  screens: [
    [
      "digitalRead(pin) reads a pin as HIGH or LOW. A button is usually wired between the pin and ground (0 volts), with INPUT_PULLUP: the board holds the pin HIGH, and pressing the button pulls it LOW. So pressed reads LOW:",
      { code: 'void setup() {\n  Serial.begin(9600);\n  pinMode(2, INPUT_PULLUP);\n  Serial.println(digitalRead(2));\n  sim_set_external(2, LOW);   // PC only: press the button\n  Serial.println(digitalRead(2));\n}\n\nvoid loop() {\n}' },
      "sim_set_external is not part of Arduino. It plays the part of your finger, so a sketch can be tried on a PC. Anything starting with sim_ is PC-only.",
    ],
    [
      "if makes a decision. == compares, and else runs when the condition is false. Here the LED follows the button:",
      { code: 'void setup() {\n  Serial.begin(9600);\n  pinMode(2, INPUT_PULLUP);\n  pinMode(LED_BUILTIN, OUTPUT);\n}\n\nvoid loop() {\n  if (digitalRead(2) == LOW) {\n    digitalWrite(LED_BUILTIN, HIGH);\n    Serial.println("pressed, LED on");\n  } else {\n    digitalWrite(LED_BUILTIN, LOW);\n    Serial.println("released, LED off");\n  }\n  sim_set_external(2, LOW);   // PC only: press it for the next loop\n}' },
      { quiz: "With INPUT_PULLUP, what does a pressed button read?", choices: ["HIGH", "LOW"], answer: 2, why: "The button connects the pin to ground, pulling it LOW." },
    ],
    [
      "Real buttons bounce: the metal contacts touch and part several times in a few milliseconds, so one press can look like many. The simple cure is to wait about 50 ms after a change and read again before believing it.",
      { tip: "INPUT_PULLUP: released is HIGH, pressed is LOW. sim_ functions are PC-only." },
      {
        exercise: "button",
        task: 'Read a button on pin 4 with INPUT_PULLUP. In each loop, print "up" if it reads HIGH, else "down". At the end of the first loop only, press it with sim_set_external(4, LOW) (use a global counter).',
        expected: ["up", "down", "down"],
        hints: ["int loops = 0; as a global; loops = loops + 1; at the end of loop.", "if (loops == 0) { sim_set_external(4, LOW); } before counting."],
        solution: 'int loops = 0;\n\nvoid setup() {\n  Serial.begin(9600);\n  pinMode(4, INPUT_PULLUP);\n}\n\nvoid loop() {\n  if (digitalRead(4) == HIGH) {\n    Serial.println("up");\n  } else {\n    Serial.println("down");\n  }\n  if (loops == 0) {\n    sim_set_external(4, LOW);\n  }\n  loops = loops + 1;\n}',
      },
    ],
  ],
  recapPt: [
    "digitalRead(pino) dá HIGH ou LOW.",
    "Com INPUT_PULLUP, um botão solto lê HIGH e carregado lê LOW.",
    "Os botões reais saltitam: espera uns 50 ms e lê outra vez.",
    "sim_set_external só existe no PC: faz de dedo no botão.",
  ],
  recap: [
    "digitalRead(pin) gives HIGH or LOW.",
    "With INPUT_PULLUP, a released button reads HIGH and a pressed one LOW.",
    "Real buttons bounce: wait about 50 ms and read again.",
    "sim_set_external only exists on the PC: it plays your finger.",
  ],
};
