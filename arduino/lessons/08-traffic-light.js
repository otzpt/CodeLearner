module.exports = {
  id: "traffic-light",
  title: "Project: a traffic light",
  titlePt: "Projeto: um semáforo",
  screens: [
    [
      "Time to put it together: a traffic light with three LEDs, on pins 10 (red), 11 (yellow) and 12 (green).",
      "Plan it as states. The light is always in one state, and moves to the next when its time is up:\n1. GREEN for 3000 ms, then\n2. YELLOW for 1000 ms, then\n3. RED for 3000 ms, then back to GREEN.",
      "A variable holds the current state, and each loop checks whether its time is up, with millis(), as in lesson 6. This pattern, called a state machine, runs most real devices.",
    ],
    [
      "An enum gives names to the states, so the code says GREEN instead of a number to remember:",
      { code: 'enum Light { GREEN, YELLOW, RED };\nLight state = GREEN;\n\nvoid setup() {\n  Serial.begin(9600);\n  Serial.println(state == GREEN);\n  state = RED;\n  Serial.println(state == RED);\n}\n\nvoid loop() {\n}' },
    ],
    [
      {
        exercise: "traffic",
        task: 'Write the traffic light. Make pins 10, 11, 12 outputs. Start GREEN at time 0. In loop, when the current state\'s time has passed, move to the next state, set the LEDs (only the current one HIGH), and print the time, a space and the state name. Print the starting state in setup. End loop() with delay(1000). For this exercise loop runs 9 times, from 0 to 8000 ms.',
        expected: ["0 GREEN", "3000 YELLOW", "4000 RED", "7000 GREEN"],
        env: { SIM_LOOPS: "9" },
        hints: ["Keep unsigned long changedAt = 0 and a duration for the current state (3000 for GREEN).", "A function show(state) that does the three digitalWrites and the print keeps loop short."],
        solution: 'enum Light { GREEN, YELLOW, RED };\nLight state = GREEN;\nunsigned long changedAt = 0;\n\nunsigned long duration(Light s) {\n  if (s == YELLOW) return 1000;\n  return 3000;\n}\n\nvoid show(Light s) {\n  digitalWrite(10, s == RED ? HIGH : LOW);\n  digitalWrite(11, s == YELLOW ? HIGH : LOW);\n  digitalWrite(12, s == GREEN ? HIGH : LOW);\n  Serial.print(millis());\n  Serial.print(" ");\n  if (s == GREEN) Serial.println("GREEN");\n  if (s == YELLOW) Serial.println("YELLOW");\n  if (s == RED) Serial.println("RED");\n}\n\nvoid setup() {\n  Serial.begin(9600);\n  pinMode(10, OUTPUT);\n  pinMode(11, OUTPUT);\n  pinMode(12, OUTPUT);\n  show(state);\n}\n\nvoid loop() {\n  if (millis() - changedAt >= duration(state)) {\n    if (state == GREEN) state = YELLOW;\n    else if (state == YELLOW) state = RED;\n    else state = GREEN;\n    changedAt = millis();\n    show(state);\n  }\n  delay(1000);\n}',
      },
    ],
  ],
  recapPt: [
    "Pensa no aparelho como estados, cada um com a sua duração.",
    "enum dá nomes aos estados.",
    "Em cada loop, se o tempo do estado acabou, passa ao seguinte: é uma máquina de estados.",
  ],
  recap: [
    "Think of the device as states, each with its own duration.",
    "enum gives the states names.",
    "Each loop, if the state's time is up, move to the next: a state machine.",
  ],
};
