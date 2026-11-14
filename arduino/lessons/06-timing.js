module.exports = {
  id: "timing",
  title: "Doing two things at once: millis",
  titlePt: "Fazer duas coisas ao mesmo tempo: millis",
  screens: [
    [
      "delay stops the whole board. While it waits, nothing else happens: a button press during a delay(1000) is missed. The way around it is to never wait. Each loop, check the clock, and act only when enough time has passed:",
      { code: 'unsigned long lastBlink = 0;\nbool on = false;\n\nvoid setup() {\n  Serial.begin(9600);\n}\n\nvoid loop() {\n  if (millis() - lastBlink >= 1000) {\n    lastBlink = millis();\n    on = !on;\n    Serial.print(millis());\n    Serial.println(on ? " on" : " off");\n  }\n  delay(400);   // PC only: stands in for the rest of the work\n}' },
      "The delay(400) here only moves the simulated clock forward between loops; a real sketch would be busy with other checks instead.",
    ],
    [
      "Why millis() - lastBlink, and not millis() >= lastBlink + 1000? millis() is an unsigned long that wraps back to 0 after about 49 days. The subtraction still gives the right answer across the wrap; the addition does not.",
      "on ? \" on\" : \" off\" picks the first value if on is true, the second otherwise. !on flips true and false.",
      { quiz: "Why avoid delay in a sketch that also reads a button?", choices: ["delay is slow to compile", "the board does nothing else while delay waits"], answer: 2, why: "A press during the wait is missed." },
    ],
    [
      { tip: "Store the time you last acted; act again when millis() - last >= interval." },
      {
        exercise: "timing",
        task: 'With unsigned long last = 0, print "beep at " and millis() whenever at least 1500 ms have passed since the last beep (and set last). End loop() with delay(1000) to move the PC clock on. Here loop runs 3 times: at 0, 1000 and 2000 ms.',
        expected: ["beep at 2000"],
        hints: ["if (millis() - last >= 1500) { last = millis(); ... }", "At 0 and 1000 ms not enough time has passed; at 2000 it has."],
        solution: 'unsigned long last = 0;\n\nvoid setup() {\n  Serial.begin(9600);\n}\n\nvoid loop() {\n  if (millis() - last >= 1500) {\n    last = millis();\n    Serial.print("beep at ");\n    Serial.println(millis());\n  }\n  delay(1000);\n}',
      },
    ],
  ],
  recapPt: [
    "delay para a placa toda; um botão carregado durante a espera perde-se.",
    "Guarda quando agiste e age de novo quando millis() - último >= intervalo.",
    "A subtração continua certa quando o millis() dá a volta, ao fim de 49 dias.",
  ],
  recap: [
    "delay stops the whole board; a button press during the wait is lost.",
    "Store when you last acted, and act again when millis() - last >= interval.",
    "The subtraction stays right when millis() wraps, after about 49 days.",
  ],
};
