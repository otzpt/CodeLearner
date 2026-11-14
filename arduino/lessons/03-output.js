module.exports = {
  id: "output",
  title: "Switching things on: digital output",
  titlePt: "Ligar coisas: saída digital",
  screens: [
    [
      "Pins are the metal legs a board uses to talk to the world. A digital pin is either HIGH (on, voltage) or LOW (off). pinMode(pin, OUTPUT) makes a pin drive something; digitalWrite sets it. LED_BUILTIN is the light on the board itself, pin 13 on an Uno:",
      { code: 'void setup() {\n  Serial.begin(9600);\n  pinMode(LED_BUILTIN, OUTPUT);\n  digitalWrite(LED_BUILTIN, HIGH);\n  Serial.println(digitalRead(LED_BUILTIN));\n}\n\nvoid loop() {\n}' },
      "On a board, the LED lights up. Here, digitalRead shows the pin's state: 1 for HIGH.",
    ],
    [
      "The classic first sketch: blink. delay(ms) waits that many milliseconds; millis() is the time since power-on:",
      { code: 'void setup() {\n  Serial.begin(9600);\n  pinMode(LED_BUILTIN, OUTPUT);\n}\n\nvoid loop() {\n  digitalWrite(LED_BUILTIN, HIGH);\n  Serial.print(millis());\n  Serial.println(" on");\n  delay(500);\n  digitalWrite(LED_BUILTIN, LOW);\n  Serial.print(millis());\n  Serial.println(" off");\n  delay(500);\n}' },
      "Here the waits are simulated, so the times are exact. On a board they are real, and delay freezes everything else while it waits; lesson 6 shows the way around that.",
    ],
    [
      "Forgetting pinMode is a classic bug: the pin stays an input, and writing to it does not drive the LED on a real board.",
      { quiz: "What must happen before digitalWrite can light an LED?", choices: ["Serial.begin", "pinMode(pin, OUTPUT)"], answer: 2, why: "Pins start as inputs; pinMode makes them outputs." },
      {
        exercise: "blink",
        task: 'Make pin 8 an output. In each loop, set it HIGH, print "on at " and millis(), wait 200 ms, set it LOW, wait 800 ms.',
        expected: ["on at 0", "on at 1000", "on at 2000"],
        hints: ["pinMode(8, OUTPUT); in setup.", 'Serial.print("on at "); Serial.println(millis()); then the two delays.'],
        solution: 'void setup() {\n  Serial.begin(9600);\n  pinMode(8, OUTPUT);\n}\n\nvoid loop() {\n  digitalWrite(8, HIGH);\n  Serial.print("on at ");\n  Serial.println(millis());\n  delay(200);\n  digitalWrite(8, LOW);\n  delay(800);\n}',
      },
    ],
  ],
  recapPt: [
    "pinMode(pino, OUTPUT) e depois digitalWrite(pino, HIGH ou LOW).",
    "LED_BUILTIN é o LED da própria placa.",
    "delay(ms) espera; millis() é o tempo desde que ligou.",
    "Sem pinMode, o pino continua uma entrada.",
  ],
  recap: [
    "pinMode(pin, OUTPUT), then digitalWrite(pin, HIGH or LOW).",
    "LED_BUILTIN is the board's own LED.",
    "delay(ms) waits; millis() is the time since power-on.",
    "Without pinMode, the pin stays an input.",
  ],
};
