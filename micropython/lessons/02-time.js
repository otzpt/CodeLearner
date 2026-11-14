module.exports = {
  id: "time",
  title: "Blinking: waiting and the clock",
  titlePt: "Piscar: esperar e o relógio",
  screens: [
    [
      "time.sleep(seconds) waits; time.sleep_ms(ms) waits in milliseconds. time.ticks_ms() is a clock in milliseconds. The classic first program, blinking:",
      { code: 'from machine import Pin\nimport time\n\nled = Pin(25, Pin.OUT)\nfor i in range(3):\n    led.on()\n    print(time.ticks_ms(), "on")\n    time.sleep_ms(500)\n    led.off()\n    time.sleep_ms(500)' },
      "On a board the waits are real. Here the clock is simulated, so the times are exact and the example finishes at once.",
    ],
    [
      "A real program on a board never ends: it loops forever with while True. Stopping it is the board's power switch, or Ctrl+C in an editor like Thonny. This course's examples count their loops instead, so they finish.",
      "ticks_ms() wraps back to 0 after a while. To measure time between two moments, use time.ticks_diff(later, earlier), which stays right across the wrap:",
      { code: 'import time\n\nstart = time.ticks_ms()\ntime.sleep(2)\nprint(time.ticks_diff(time.ticks_ms(), start), "ms passed")' },
      { quiz: "How do you measure how long something took?", choices: ["ticks_ms() - start", "time.ticks_diff(ticks_ms(), start)"], answer: 2, why: "ticks_diff stays correct when the clock wraps." },
    ],
    [
      {
        exercise: "blink",
        task: 'Make pin 25 an output. Four times: switch it on, print "on at" and time.ticks_ms(), wait 200 ms, switch it off, wait 300 ms.',
        expected: ["on at 0", "on at 500", "on at 1000", "on at 1500"],
        hints: ["for i in range(4):", 'print("on at", time.ticks_ms()), then sleep_ms(200), off, sleep_ms(300).'],
        solution: 'from machine import Pin\nimport time\n\nled = Pin(25, Pin.OUT)\nfor i in range(4):\n    led.on()\n    print("on at", time.ticks_ms())\n    time.sleep_ms(200)\n    led.off()\n    time.sleep_ms(300)',
      },
    ],
  ],
  recapPt: [
    "time.sleep(s) e time.sleep_ms(ms) esperam.",
    "time.ticks_ms() é um relógio em milissegundos.",
    "time.ticks_diff(depois, antes) mede tempo, mesmo quando o relógio dá a volta.",
    "Numa placa, o programa repete para sempre com while True.",
  ],
  recap: [
    "time.sleep(s) and time.sleep_ms(ms) wait.",
    "time.ticks_ms() is a clock in milliseconds.",
    "time.ticks_diff(later, earlier) measures time, even when the clock wraps.",
    "On a board, the program repeats forever with while True.",
  ],
};
