module.exports = {
  id: "analog",
  title: "Sensors and dimming",
  titlePt: "Sensores e regular",
  screens: [
    [
      "Many sensors give a voltage instead of just on or off: a light sensor, a knob. An ADC (analog-to-digital converter) turns it into a number. On a Pico, read_u16() gives 0 to 65535, for 0 to 3.3 volts:",
      { code: 'from machine import ADC\nimport sim\n\nsim.set_adc(0, 32768)   # PC only: put half of 3.3 V on ADC 0\nsensor = ADC(0)\nraw = sensor.read_u16()\nprint(raw)\nprint(round(raw * 3.3 / 65535, 2), "volts")' },
      "sim.set_adc plays the sensor on the PC.",
    ],
    [
      "PWM does a kind of opposite: it switches a pin on and off very fast, so an LED looks dimmer or a motor turns slower. duty_u16 sets how much of the time it is on, from 0 to 65535:",
      { code: 'from machine import Pin, PWM\n\nled = PWM(Pin(15))\nled.freq(1000)\nfor percent in [0, 25, 100]:\n    led.duty_u16(percent * 65535 // 100)\n    print(percent, "% ->", led.duty_u16())' },
      "freq(1000) switches 1000 times a second, too fast to see. // divides and drops the decimals.",
      { quiz: "read_u16() returns numbers in which range?", choices: ["0 to 1023", "0 to 65535", "0 to 3.3"], answer: 2, why: "u16 means a 16-bit unsigned number: 0 to 65535." },
    ],
    [
      {
        exercise: "analog",
        task: 'Put 49152 on ADC 1 with sim.set_adc (PC only). Read it, print the reading, then print it as a whole percentage of 65535, then set a PWM on pin 15 to that same duty and print its duty_u16().',
        expected: ["49152", "75", "49152"],
        hints: ["percent = raw * 100 // 65535", "pwm = PWM(Pin(15)); pwm.duty_u16(raw); print(pwm.duty_u16())"],
        solution: 'from machine import ADC, Pin, PWM\nimport sim\n\nsim.set_adc(1, 49152)\nraw = ADC(1).read_u16()\nprint(raw)\nprint(raw * 100 // 65535)\npwm = PWM(Pin(15))\npwm.duty_u16(raw)\nprint(pwm.duty_u16())',
      },
    ],
  ],
  recapPt: [
    "ADC(n).read_u16() lê uma tensão como 0 a 65535.",
    "PWM(Pin(n)) com duty_u16(0 a 65535) regula a intensidade.",
    "sim.set_adc só existe no PC: faz de sensor.",
  ],
  recap: [
    "ADC(n).read_u16() reads a voltage as 0 to 65535.",
    "PWM(Pin(n)) with duty_u16(0 to 65535) dims or slows.",
    "sim.set_adc only exists on the PC: it plays the sensor.",
  ],
};
