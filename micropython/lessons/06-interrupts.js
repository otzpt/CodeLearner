module.exports = {
  id: "interrupts",
  title: "Reacting at once: interrupts",
  titlePt: "Reagir logo: interrupções",
  screens: [
    [
      "Checking a button in a loop can miss a very short press. An interrupt is the other way round: the board itself calls a function of yours the moment a pin changes, whatever the program was doing:",
      { code: 'from machine import Pin\nimport sim\n\npresses = 0\n\ndef pressed(pin):\n    global presses\n    presses += 1\n\nbutton = Pin(16, Pin.IN, Pin.PULL_UP)\nbutton.irq(handler=pressed, trigger=Pin.IRQ_FALLING)\nfor level in [0, 1, 0, 1, 0]:\n    sim.drive(16, level)   # PC only\nprint(presses, "presses")' },
      "IRQ_FALLING means \"when the pin goes from 1 to 0\", which is a press. global lets the function change a variable made outside it.",
    ],
    [
      "A handler interrupts the program at any moment, so keep it tiny: set a flag or count, and let the main loop do the real work. Printing or waiting inside a handler can cause trouble on a real board.",
      { quiz: "What should an interrupt handler do?", choices: ["as much as possible", "something tiny, like counting or setting a flag"], answer: 2, why: "It interrupts everything else; the main loop does the rest." },
    ],
    [
      {
        exercise: "interrupts",
        task: "Count presses on pin 7 with an interrupt on IRQ_FALLING, using a handler that adds 1 to a global count. Drive the levels 0, 1, 0, 1, 0, 1, 0 onto pin 7 with sim.drive (PC only), then print the count.",
        expected: ["4"],
        hints: ["def on_press(pin): global count; count += 1", "button.irq(handler=on_press, trigger=Pin.IRQ_FALLING)"],
        solution: 'from machine import Pin\nimport sim\n\ncount = 0\n\ndef on_press(pin):\n    global count\n    count += 1\n\nbutton = Pin(7, Pin.IN, Pin.PULL_UP)\nbutton.irq(handler=on_press, trigger=Pin.IRQ_FALLING)\nfor level in [0, 1, 0, 1, 0, 1, 0]:\n    sim.drive(7, level)\nprint(count)',
      },
    ],
  ],
  recapPt: [
    "pin.irq(handler=f, trigger=Pin.IRQ_FALLING) chama f quando o pino desce de 1 para 0.",
    "A função de interrupção deve ser mínima: contar ou marcar, mais nada.",
    "global deixa uma função mudar uma variável de fora.",
  ],
  recap: [
    "pin.irq(handler=f, trigger=Pin.IRQ_FALLING) calls f when the pin falls from 1 to 0.",
    "An interrupt handler should be tiny: count or flag, nothing more.",
    "global lets a function change a variable made outside it.",
  ],
};
