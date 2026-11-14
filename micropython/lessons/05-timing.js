module.exports = {
  id: "timing",
  title: "Doing two things at once",
  titlePt: "Fazer duas coisas ao mesmo tempo",
  screens: [
    [
      "sleep stops the whole program. While it waits, nothing else happens: a button press during a sleep(1) is missed. The way around it: never wait long. Each time round the loop, check the clock and act only when enough time has passed:",
      { code: 'import time\n\nlast_blink = time.ticks_ms()\non = False\nfor step in range(8):\n    now = time.ticks_ms()\n    if time.ticks_diff(now, last_blink) >= 1000:\n        last_blink = now\n        on = not on\n        print(now, "on" if on else "off")\n    time.sleep_ms(300)   # stands in for the other work' },
      "The loop could read buttons and sensors between blinks, because nothing ever waits more than a moment.",
    ],
    [
      "\"on\" if on else \"off\" picks the first value when on is True, the second otherwise. not flips True and False.",
      { quiz: "Why avoid long sleeps in a program that also reads a button?", choices: ["sleep wears out the board", "the program does nothing else while it sleeps"], answer: 2, why: "A press during the sleep is missed." },
    ],
    [
      { tip: "Store when you last acted; act again when ticks_diff(now, last) >= interval." },
      {
        exercise: "timing",
        task: 'Run 6 steps of 1000 ms each (use time.sleep_ms(1000) at the end of each step). Print "beep at" and the time whenever at least 2500 ms have passed since the last beep, starting with last = the time before the first step.',
        expected: ["beep at 3000"],
        hints: ["last = time.ticks_ms() before the loop.", "if time.ticks_diff(now, last) >= 2500: last = now; print(...)"],
        solution: 'import time\n\nlast = time.ticks_ms()\nfor step in range(6):\n    now = time.ticks_ms()\n    if time.ticks_diff(now, last) >= 2500:\n        last = now\n        print("beep at", now)\n    time.sleep_ms(1000)',
      },
    ],
  ],
  recapPt: [
    "sleep para tudo; um botão carregado durante a espera perde-se.",
    "Guarda quando agiste e age de novo quando ticks_diff(agora, último) >= intervalo.",
    "\"a\" if condição else \"b\" escolhe entre dois valores.",
  ],
  recap: [
    "sleep stops everything; a button press during the wait is lost.",
    "Store when you last acted, and act again when ticks_diff(now, last) >= interval.",
    "\"a\" if condition else \"b\" picks one of two values.",
  ],
};
