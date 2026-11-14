module.exports = {
  id: "analog",
  title: "Sensors and dimming: analog pins",
  titlePt: "Sensores e regular: pinos analógicos",
  screens: [
    [
      "Many sensors give a voltage between 0 and 5 volts instead of just on or off: a light sensor, a temperature sensor, a knob. analogRead turns the voltage on pins A0 to A5 into a number from 0 to 1023:",
      { code: 'void setup() {\n  Serial.begin(9600);\n  sim_set_analog(0, 512);   // PC only: set A0 to half of 5 V\n  int raw = analogRead(A0);\n  Serial.println(raw);\n  Serial.println(raw * 5.0 / 1023);\n}\n\nvoid loop() {\n}' },
      "raw * 5.0 / 1023 turns the number back into volts; Serial.println shows two decimals. sim_set_analog plays the sensor on the PC.",
    ],
    [
      "map(value, fromLow, fromHigh, toLow, toHigh) rescales a number from one range to another, for example a 0-1023 reading into a 0-100 percentage. It works in whole numbers:",
      { code: 'void setup() {\n  Serial.begin(9600);\n  int readings[] = {0, 256, 1023};\n  for (int i = 0; i < 3; i++) {\n    Serial.println(map(readings[i], 0, 1023, 0, 100));\n  }\n}\n\nvoid loop() {\n}' },
      "for repeats the indented block with i = 0, 1, 2. readings[i] is the value at position i, counting from 0.",
    ],
    [
      "analogWrite(pin, 0 to 255) does the opposite in a way: it switches a pin on and off very fast, so an LED looks dimmer or a motor turns slower. 255 is fully on. Only pins marked ~ on the board can do it (3, 5, 6, 9, 10 and 11 on an Uno).",
      { quiz: "analogRead gives numbers in which range?", choices: ["0 to 255", "0 to 1023", "0 to 5"], answer: 2, why: "analogRead is 0 to 1023; analogWrite takes 0 to 255." },
      {
        exercise: "analog",
        task: "Set A1 to 900 with sim_set_analog(1, 900). Read it, print the reading, then print it mapped to 0-255, then print \"bright\" if the reading is above 800, else \"dim\".",
        expected: ["900", "224", "bright"],
        hints: ["int raw = analogRead(A1);", "map(raw, 0, 1023, 0, 255)"],
        solution: 'void setup() {\n  Serial.begin(9600);\n  sim_set_analog(1, 900);\n  int raw = analogRead(A1);\n  Serial.println(raw);\n  Serial.println(map(raw, 0, 1023, 0, 255));\n  if (raw > 800) {\n    Serial.println("bright");\n  } else {\n    Serial.println("dim");\n  }\n}\n\nvoid loop() {\n}',
      },
    ],
  ],
  recapPt: [
    "analogRead(A0) dá 0 a 1023 para 0 a 5 V.",
    "map(v, 0, 1023, 0, 100) muda a escala, com números inteiros.",
    "analogWrite(pino, 0 a 255) regula a intensidade nos pinos com ~.",
    "sim_set_analog só existe no PC: faz de sensor.",
  ],
  recap: [
    "analogRead(A0) gives 0 to 1023 for 0 to 5 V.",
    "map(v, 0, 1023, 0, 100) rescales, in whole numbers.",
    "analogWrite(pin, 0 to 255) dims on the pins marked ~.",
    "sim_set_analog only exists on the PC: it plays the sensor.",
  ],
};
