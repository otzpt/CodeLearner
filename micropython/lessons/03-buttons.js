module.exports = {
  id: "buttons",
  title: "Buttons: reading a pin",
  titlePt: "Botões: ler um pino",
  screens: [
    [
      "Pin(number, Pin.IN, Pin.PULL_UP) makes a pin an input that the board holds high (1). A button wired between that pin and ground pulls it to 0 when pressed. So pressed reads 0:",
      { code: 'from machine import Pin\nimport sim\n\nbutton = Pin(16, Pin.IN, Pin.PULL_UP)\nprint(button.value())\nsim.drive(16, 0)   # PC only: press the button\nprint(button.value())' },
      "sim.drive plays the part of your finger, so the code can be tried on a PC. On a board, leave that line out.",
    ],
    [
      "if makes a decision. Here the LED follows the button:",
      { code: 'from machine import Pin\nimport sim\n\nbutton = Pin(16, Pin.IN, Pin.PULL_UP)\nled = Pin(25, Pin.OUT)\nfor press in [1, 0, 1]:\n    sim.drive(16, press)   # PC only\n    if button.value() == 0:\n        led.on()\n        print("pressed, LED on")\n    else:\n        led.off()\n        print("released, LED off")' },
      { quiz: "With PULL_UP, what does a pressed button read?", choices: ["1", "0"], answer: 2, why: "The button connects the pin to ground." },
    ],
    [
      "Real buttons bounce: the contacts touch and part several times in a few milliseconds, so one press can look like many. The simple cure is to wait about 50 ms after a change and read again before believing it.",
      { tip: "PULL_UP: released is 1, pressed is 0. sim.drive is PC-only." },
      {
        exercise: "buttons",
        task: 'Make pin 5 an input with PULL_UP. For each of the levels 1, 1, 0, 1 in turn: drive it onto pin 5 with sim.drive (PC only), then print "up" or "down". Finally print how many times it read down.',
        expected: ["up", "up", "down", "up", "1"],
        hints: ["for level in [1, 1, 0, 1]: sim.drive(5, level) then read button.value()", "Keep downs = 0 before the loop and add 1 when it reads 0."],
        solution: 'from machine import Pin\nimport sim\n\nbutton = Pin(5, Pin.IN, Pin.PULL_UP)\ndowns = 0\nfor level in [1, 1, 0, 1]:\n    sim.drive(5, level)\n    if button.value() == 0:\n        print("down")\n        downs += 1\n    else:\n        print("up")\nprint(downs)',
      },
    ],
  ],
  recapPt: [
    "Pin(n, Pin.IN, Pin.PULL_UP) cria uma entrada que fica a 1.",
    "Um botão carregado lê 0.",
    "Os botões reais saltitam: espera uns 50 ms e lê outra vez.",
    "sim.drive só existe no PC: faz de dedo.",
  ],
  recap: [
    "Pin(n, Pin.IN, Pin.PULL_UP) makes an input that rests at 1.",
    "A pressed button reads 0.",
    "Real buttons bounce: wait about 50 ms and read again.",
    "sim.drive only exists on the PC: it plays your finger.",
  ],
};
