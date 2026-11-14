module.exports = {
  id: "pins",
  title: "A board and its pins",
  titlePt: "Uma placa e os seus pinos",
  screens: [
    [
      "MicroPython is a small version of Python that runs on boards like the Raspberry Pi Pico, made to control lights, motors and sensors. You copy a file called main.py to the board, and it runs every time the board powers on.",
      "This course runs your code on your PC with a stand-in for the board: no hardware needed. Pins become variables, and time is simulated, so a one-second wait takes no real time. Anything from a module called sim is PC-only and is marked so.",
    ],
    [
      "Pins are the metal legs a board uses to talk to the world. The machine module controls them. Pin(number, Pin.OUT) makes a pin an output; on() and off() switch it; value() reads it back. On a Pico, pin 25 is the light on the board:",
      { code: 'from machine import Pin\n\nled = Pin(25, Pin.OUT)\nled.on()\nprint(led.value())\nled.off()\nprint(led.value())' },
      "1 means on (high voltage), 0 means off. On a board, the LED would light up and go dark.",
    ],
    [
      "toggle() flips a pin, and value(1) or value(0) sets it directly. print works as in any Python, and its text appears on the computer the board is plugged into:",
      { code: 'from machine import Pin\n\nled = Pin(25, Pin.OUT)\nfor i in range(3):\n    led.toggle()\n    print("lap", i, "led", led.value())' },
      { quiz: "What does Pin(15, Pin.OUT) do?", choices: ["reads pin 15", "makes pin 15 an output you can switch"], answer: 2, why: "Pin.OUT makes it drive something; Pin.IN would read it." },
    ],
    [
      {
        exercise: "pins",
        task: "Make pin 14 an output. Switch it on and print its value, then toggle it and print its value again.",
        expected: ["1", "0"],
        hints: ["from machine import Pin, then light = Pin(14, Pin.OUT)", "light.on(), print(light.value()), light.toggle(), print(light.value())"],
        solution: 'from machine import Pin\n\nlight = Pin(14, Pin.OUT)\nlight.on()\nprint(light.value())\nlight.toggle()\nprint(light.value())',
      },
    ],
  ],
  recapPt: [
    "MicroPython é um Python pequeno para placas; main.py corre quando a placa liga.",
    "Pin(n, Pin.OUT) cria uma saída; on(), off(), toggle() e value() controlam-na.",
    "1 é ligado, 0 é desligado.",
    "Aqui tudo corre no PC; o módulo sim é só do PC.",
  ],
  recap: [
    "MicroPython is a small Python for boards; main.py runs when the board powers on.",
    "Pin(n, Pin.OUT) makes an output; on(), off(), toggle() and value() control it.",
    "1 is on, 0 is off.",
    "Here everything runs on the PC; the sim module is PC-only.",
  ],
};
