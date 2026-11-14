module.exports = {
  id: "traffic-light",
  title: "Project: a traffic light",
  titlePt: "Projeto: um semáforo",
  screens: [
    [
      "Time to put it together: a traffic light with three LEDs, on pins 10 (red), 11 (yellow) and 12 (green).",
      "Plan it as states. The light is always in one state, and moves to the next when its time is up:\n1. green for 3000 ms, then\n2. yellow for 1000 ms, then\n3. red for 3000 ms, then back to green.",
      "A dictionary can hold each state's time and the next state, so the loop stays short. This pattern, called a state machine, runs most real devices:",
      { code: 'TIMES = {"green": 3000, "yellow": 1000, "red": 3000}\nNEXT = {"green": "yellow", "yellow": "red", "red": "green"}\nstate = "green"\nfor _ in range(4):\n    print(state, TIMES[state])\n    state = NEXT[state]' },
    ],
    [
      {
        exercise: "traffic",
        task: 'Write the traffic light. Make pins 10, 11, 12 outputs. Start green at time 0 and print the time and state. Then run 8 steps of time.sleep_ms(1000); in each, when the current state\'s time has passed, move to the next state, light only its LED, and print the time and the state.',
        expected: ["0 green", "3000 yellow", "4000 red", "7000 green"],
        hints: ["Keep changed = time.ticks_ms() when the state changes.", "In each step: sleep first, then check ticks_diff(now, changed) >= TIMES[state]."],
        solution: 'from machine import Pin\nimport time\n\nleds = {"red": Pin(10, Pin.OUT), "yellow": Pin(11, Pin.OUT), "green": Pin(12, Pin.OUT)}\nTIMES = {"green": 3000, "yellow": 1000, "red": 3000}\nNEXT = {"green": "yellow", "yellow": "red", "red": "green"}\n\ndef show(state):\n    for name, pin in leds.items():\n        pin.value(1 if name == state else 0)\n    print(time.ticks_ms(), state)\n\nstate = "green"\nchanged = time.ticks_ms()\nshow(state)\nfor step in range(8):\n    time.sleep_ms(1000)\n    now = time.ticks_ms()\n    if time.ticks_diff(now, changed) >= TIMES[state]:\n        state = NEXT[state]\n        changed = now\n        show(state)',
      },
    ],
  ],
  recapPt: [
    "Pensa no aparelho como estados, cada um com a sua duração.",
    "Dicionários guardam o tempo e o estado seguinte de cada estado.",
    "Em cada passo, se o tempo do estado acabou, passa ao seguinte.",
  ],
  recap: [
    "Think of the device as states, each with its own duration.",
    "Dictionaries hold each state's time and the next state.",
    "Each step, if the state's time is up, move to the next.",
  ],
};
