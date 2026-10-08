/*
 * Modules 10 to 13 - memory and registers, interrupts, assembly, and a
 * final project.
 */

/* A number right-aligned in `width` columns, so a table lines up. */
void print_padded(int value, int width)
{
    int digits = 1;
    for (int rest = value / 10; rest > 0; rest = rest / 10) {
        digits++;
    }
    for (int i = digits; i < width; i++) {
        Serial.print(' ');
    }
    Serial.print(value);
}

/* Eight binary digits with the zeros kept: Serial.println(x, BIN) drops them. */
void print_bits(uint8_t value)
{
    for (int i = 7; i >= 0; i--) {
        Serial.print(bitRead(value, i));
    }
}

/* Read from "flash" by hand, the way PROGMEM data is read on a real AVR. */
const uint8_t SPEED_TABLE[4] PROGMEM = {0, 90, 160, 255};

void lesson_10_memory()
{
    title("MODULE 10 - MEMORY, BITS AND PORTS");

    heading("PART 1: three memories, and where your text goes");

    Serial.println("  The Uno's chip, the ATmega328P, has three separate memories");
    Serial.println("  (Arduino documentation):");
    Serial.println();
    Serial.println("    flash    32 KB   the program itself; survives power-off");
    Serial.println("    SRAM      2 KB   variables, the stack, the heap: working");
    Serial.println("                     memory, wiped at power-off");
    Serial.println("    EEPROM    1 KB   a few settings that survive power-off");
    Serial.println();
    Serial.println("  Variables compete for 2 KB. On an AVR, a string literal like");
    Serial.println("  Serial.print(\"hello\") is copied into SRAM at startup, so a");
    Serial.println("  sketch full of messages runs out of memory with no error.");
    Serial.println("  The fix is to keep it in flash:");
    Serial.println();
    Serial.println("    Serial.print(F(\"hello\"));          // text stays in flash");
    Serial.println("    const uint8_t table[] PROGMEM = {...};   // data stays in flash");
    Serial.println("    pgm_read_byte(&table[i])                // read one byte of it");
    Serial.println();
    Serial.print("  Running:  ");
    for (int i = 0; i < 4; i++) {
        Serial.print(pgm_read_byte(&SPEED_TABLE[i]));
        Serial.print(' ');
    }
    Serial.println();
    Serial.println();
    Serial.println("  Honest note: on this PC, F() and PROGMEM do nothing, because");
    Serial.println("  a PC has no separate flash. The lines above run, and the");
    Serial.println("  saving only exists on the board. This very course would not");
    Serial.println("  fit an Uno's 2 KB; it does not need to.");

    wait_enter();
    clear_screen();
    heading("PART 2: bits");

    Serial.println("  A byte is eight on/off flags. Four operators work on them:");
    Serial.println("  & (AND)  | (OR)  ^ (XOR)  ~ (NOT), and << >> shift them.");
    Serial.println();
    Serial.println("    flags = 0b00000101");
    uint8_t flags = 0b00000101;
    Serial.print("  Running:  start       ");
    print_bits(flags);
    Serial.println();
    flags |= (1 << 3);
    Serial.println("    flags |= (1 << 3);      // SET bit 3");
    Serial.print("  Running:  after set   ");
    print_bits(flags);
    Serial.println();
    flags &= ~(1 << 0);
    Serial.println("    flags &= ~(1 << 0);     // CLEAR bit 0");
    Serial.print("  Running:  after clear ");
    print_bits(flags);
    Serial.println();
    flags ^= (1 << 7);
    Serial.println("    flags ^= (1 << 7);      // TOGGLE bit 7");
    Serial.print("  Running:  after flip  ");
    print_bits(flags);
    Serial.println();
    Serial.println();
    Serial.println("    (flags >> 3) & 1        // TEST bit 3");
    Serial.print("  Running:  ");
    Serial.println((flags >> 3) & 1);
    Serial.println();
    Serial.println("  Arduino names these bitSet, bitClear and bitRead, and _BV(n)");
    Serial.println("  is (1 << n).");
    Serial.println();
    Serial.println("  The trap is precedence. == binds tighter than &, so");
    Serial.println();
    Serial.println("    if (x & 2 == 2)    means    x & (2 == 2)    ->  x & 1");
    Serial.println();
    Serial.println("  and the compiler says so:");
    Serial.println();
    Serial.println("    warning: suggest parentheses around comparison in operand");
    Serial.println("    of '&' [-Wparentheses]");
    Serial.println();
    Serial.println("  Always write if ((x & 2) == 2).");

    wait_enter();
    clear_screen();
    heading("PART 3: ports, the chip's own pin registers");

    Serial.println("  digitalWrite(13, HIGH) looks up which register and bit pin");
    Serial.println("  13 belongs to, then sets it. You can do that yourself.");
    Serial.println("  The Uno's pins live in three ports:");
    Serial.println();
    Serial.println("    pins 0-7    PORTD  bits 0-7     (0 and 1 are Serial!)");
    Serial.println("    pins 8-13   PORTB  bits 0-5     (13 is bit 5)");
    Serial.println("    pins A0-A5  PORTC  bits 0-5");
    Serial.println();
    Serial.println("  Each port has three registers: DDRx (direction, 1 = output),");
    Serial.println("  PORTx (output level, or pull-up on an input) and PINx (read).");
    Serial.println();
    Serial.println("    DDRB  |= (1 << 5);     // pin 13 becomes an OUTPUT");
    Serial.println("    PORTB |= (1 << 5);     // pin 13 goes HIGH");
    DDRB |= (1 << 5);
    PORTB |= (1 << 5);
    Serial.print("  Running:  digitalRead(13) = ");
    Serial.println(digitalRead(13));
    Serial.print("            PORTB = ");
    print_bits(PORTB);
    Serial.println();
    Serial.println();
    Serial.println("  Same pin, no function call. Writing a register is a single");
    Serial.println("  instruction and changes several pins at once, which is why");
    Serial.println("  fast code uses it. The trap: PORTD = 0xFF overwrites ALL");
    Serial.println("  eight bits, including pins 0 and 1, the Serial lines. Use");
    Serial.println("  |= and &= to touch only your own bits.");

    wait_enter();
    clear_screen();
    exercise(10);

    question("Which memory holds your variables while the program runs:\n"
             "  flash, SRAM or EEPROM?",
             "SRAM", "Flash holds the program; SRAM is the working memory.");
    question("uint8_t flags = 0b0101;  flags |= (1 << 1);\n"
             "  What is flags, in decimal?",
             "7", "0101 with bit 1 set is 0111, which is 7.");

    {
        const char *task[] = {
            "Using only DDRB and PORTB (no pinMode, no digitalWrite), make pin 12",
            "an output and drive it HIGH. Then print digitalRead(12).",
        };
        const char *expected[] = {"1"};
        const char *solution[] = {
            "void setup() {",
            "  Serial.begin(9600);",
            "  DDRB |= (1 << 4);     // pin 12 is bit 4 of PORTB (pin 8 is bit 0)",
            "  PORTB |= (1 << 4);",
            "  Serial.println(digitalRead(12));",
            "}",
            "",
            "void loop() { }",
        };
        challenge(task, 2, NULL, 0, expected, 1, solution, 8);
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    Serial.println("   - flash holds the program, SRAM the variables (only 2 KB),");
    Serial.println("     EEPROM a few saved settings");
    Serial.println("   - F() and PROGMEM keep text and tables out of SRAM");
    Serial.println("   - |= sets, &= ~ clears, ^= flips, (x >> n) & 1 tests a bit;");
    Serial.println("     put parentheses around & and | tests");
    Serial.println("   - DDRx, PORTx, PINx are the pins; use |= and &= on them");
    Serial.println();
    Serial.println("  Module 11: code that runs the instant something happens.");
    wait_enter();
}

volatile uint8_t press_count = 0;

void on_press()
{
    press_count++;
}

void lesson_11_interrupts()
{
    title("MODULE 11 - INTERRUPTS AND volatile");

    heading("PART 1: an interrupt service routine");

    Serial.println("  Polling checks a pin each time round loop(); a short pulse");
    Serial.println("  that comes and goes between two checks is lost. An interrupt");
    Serial.println("  makes the chip stop what it is doing, run a small function");
    Serial.println("  right now, and carry on afterwards.");
    Serial.println();
    Serial.println("    attachInterrupt(digitalPinToInterrupt(2), on_press, FALLING);");
    Serial.println();
    Serial.println("  On an Uno only pins 2 and 3 can do this. FALLING means a");
    Serial.println("  change from HIGH to LOW; RISING and CHANGE exist too.");
    Serial.println();

    press_count = 0;
    pinMode(2, INPUT_PULLUP);
    sim_set_external(2, HIGH);
    attachInterrupt(digitalPinToInterrupt(2), on_press, FALLING);
    Serial.println("    void on_press() { press_count++; }");
    Serial.println("    ...then three presses (pin goes LOW, then back HIGH):");
    for (int i = 0; i < 3; i++) {
        sim_set_external(2, LOW);
        sim_set_external(2, HIGH);
    }
    Serial.print("  Running:  press_count = ");
    Serial.println(press_count);
    Serial.println();
    Serial.println("  The rules for the function itself: keep it very short; no");
    Serial.println("  delay(); millis() does not advance inside it; Serial.print");
    Serial.println("  is unreliable there (all from the Arduino reference). Set a");
    Serial.println("  flag or a counter, and let loop() do the slow work.");

    wait_enter();
    clear_screen();
    heading("PART 2: volatile");

    Serial.println("  Look at the counter's declaration:");
    Serial.println();
    Serial.println("    volatile uint8_t press_count = 0;");
    Serial.println();
    Serial.println("  The compiler assumes nothing changes a variable except the");
    Serial.println("  code it is looking at. So in");
    Serial.println();
    Serial.println("    while (!pressed) { }      // wait for the interrupt");
    Serial.println();
    Serial.println("  it may read `pressed` ONCE, see false, and compile the loop");
    Serial.println("  into one that never ends, since nothing in the loop writes");
    Serial.println("  it. volatile tells it: this can change at any moment from");
    Serial.println("  outside, read it from memory every time. Any variable shared");
    Serial.println("  between an ISR and the rest of the program needs it.");

    wait_enter();
    clear_screen();
    heading("PART 3: shared variables, and what a pending interrupt is");

    Serial.println("  An AVR is an 8-bit chip. Reading a 16-bit variable takes two");
    Serial.println("  separate byte reads, and an interrupt can land BETWEEN them,");
    Serial.println("  giving half old value and half new. So copy a shared");
    Serial.println("  multi-byte variable with interrupts off:");
    Serial.println();
    Serial.println("    noInterrupts();");
    Serial.println("    uint16_t copy = total;");
    Serial.println("    interrupts();");
    Serial.println();
    Serial.println("  While interrupts are off the event is not lost, it waits.");
    Serial.println("  But the chip keeps ONE waiting flag per interrupt:");
    Serial.println();

    press_count = 0;
    noInterrupts();
    for (int i = 0; i < 3; i++) {
        sim_set_external(2, LOW);
        sim_set_external(2, HIGH);
    }
    Serial.println("    noInterrupts();   ...three presses...");
    Serial.print("  Running:  press_count while off = ");
    Serial.println(press_count);
    interrupts();
    Serial.println("    interrupts();");
    Serial.print("  Running:  press_count after on  = ");
    Serial.println(press_count);
    Serial.println();
    Serial.println("  Three presses, counted as one. The second and third found the");
    Serial.println("  flag already raised. Keep interrupt-off stretches short.");
    detachInterrupt(digitalPinToInterrupt(2));

    wait_enter();
    clear_screen();
    exercise(11);

    question("Which keyword marks a variable an ISR changes behind the\n"
             "  compiler's back?",
             "volatile", "It forces a fresh read from memory every time.");
    question("Should an interrupt service routine call delay()? (yes or no)",
             "no", "ISRs must be short; delay() depends on time that stands still there.");

    {
        const char *task[] = {
            "Count falling edges on pin 2 with an interrupt. Simulate three presses",
            "with sim_set_external(2, LOW) then sim_set_external(2, HIGH), three",
            "times, then print the count.",
        };
        const char *expected[] = {"3"};
        const char *solution[] = {
            "volatile uint8_t presses = 0;",
            "",
            "void on_press() { presses++; }",
            "",
            "void setup() {",
            "  Serial.begin(9600);",
            "  pinMode(2, INPUT_PULLUP);",
            "  attachInterrupt(digitalPinToInterrupt(2), on_press, FALLING);",
            "  for (int i = 0; i < 3; i++) {",
            "    sim_set_external(2, LOW);",
            "    sim_set_external(2, HIGH);",
            "  }",
            "  Serial.println(presses);",
            "}",
            "",
            "void loop() { }",
        };
        challenge(task, 3, NULL, 0, expected, 1, solution, 16);
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    Serial.println("   - an ISR runs at once on a pin event; keep it tiny");
    Serial.println("   - variables shared with an ISR must be volatile");
    Serial.println("   - copy multi-byte shared variables with interrupts off");
    Serial.println("   - events during noInterrupts() collapse into one");
    Serial.println();
    Serial.println("  Module 12: writing instructions by hand inside C++.");
    wait_enter();
}

/* Lesson 12's function. On a real AVR it is one instruction; here, where
 * there is no AVR to run it on, the #else branch does the same job in C. */
uint8_t swap_nibbles(uint8_t x)
{
#ifdef __AVR__
    asm("swap %0" : "+r"(x));
#else
    x = (uint8_t) ((x << 4) | (x >> 4));
#endif
    return x;
}

void lesson_12_assembly()
{
    title("MODULE 12 - INLINE ASSEMBLY ON THE AVR");

    heading("PART 1: look at what the compiler already writes");

    Serial.println("  Assembly is the chip's own instruction set. Three honest");
    Serial.println("  reasons to write any: exact timing, an instruction C has no");
    Serial.println("  way to say (reading the status register, switching");
    Serial.println("  interrupts off), or a hot loop the compiler handled badly.");
    Serial.println("  Before writing a line, see what the compiler already did.");
    Serial.println();
    Serial.println("  The listings in this module were produced by the AVR");
    Serial.println("  compiler (avr-gcc 14.3, -mmcu=atmega328p -Os) from the files");
    Serial.println("  in avr/, and check-avr.py recompiles them to keep them true.");
    Serial.println("  run-avr.py goes further: it EXECUTES them on a simulated");
    Serial.println("  ATmega328P (simavr) and reads the results out of its memory,");
    Serial.println("  so what the lesson says each one does is observed, not read");
    Serial.println("  off the listing.");
    Serial.println();
    Serial.println("    void set_led(void) { PORTB |= (1 << 5); }");
    Serial.println();
    Serial.println("    sbi   0x05, 5");
    Serial.println("    ret");
    Serial.println();
    Serial.println("  The whole statement became ONE instruction, sbi (set bit in");
    Serial.println("  an I/O register). Hand-written assembly cannot beat that.");
    Serial.println();
    Serial.println("  The simplest inline assembly there is:");
    Serial.println();
    Serial.println("    asm volatile(\"nop\");");
    Serial.println();
    Serial.println("  nop does nothing for one clock cycle (AVR instruction set");
    Serial.println("  manual). At 16 MHz a cycle is 1 / 16,000,000 s = 62.5 ns, so");
    Serial.println("  16 of them make exactly 1 microsecond: a delay no function");
    Serial.println("  call can give, because the call costs cycles too. Eight of");
    Serial.println("  them compile to eight 2-byte nops, 16 bytes of zeros that");
    Serial.println("  avr-objdump prints as '...'.");
    Serial.println();
    Serial.println("  volatile stops the compiler from deleting or moving it.");

    wait_enter();
    clear_screen();
    heading("PART 2: extended asm, with operands");

    Serial.println("  Plain asm cannot touch your variables. Extended asm can:");
    Serial.println();
    Serial.println("    asm volatile( \"template\"");
    Serial.println("        : outputs      // \"=r\"(v)  written by the asm");
    Serial.println("        : inputs       // \"r\"(v)   read by the asm");
    Serial.println("        : clobbers );  // what it changes behind the compiler");
    Serial.println();
    Serial.println("  %0, %1, ... in the template stand for the operands, in order.");
    Serial.println("  The letter picks which registers the compiler may use");
    Serial.println("  (avr-gcc): r = any register, d = r16-r31 only (needed by ldi,");
    Serial.println("  subi, andi), I = a constant 0-63, M = a constant 0-255.");
    Serial.println("  = means written, + means read AND written.");
    Serial.println();
    Serial.println("    uint8_t swap_asm(uint8_t x) {");
    Serial.println("        asm(\"swap %0\" : \"+r\"(x));");
    Serial.println("        return x;");
    Serial.println("    }");
    Serial.println("    uint8_t swap_c(uint8_t x) {");
    Serial.println("        return (uint8_t)((x << 4) | (x >> 4));");
    Serial.println("    }");
    Serial.println();
    Serial.println("  swap exchanges the two halves of a byte. What avr-gcc makes");
    Serial.println("  of each:");
    Serial.println();
    Serial.println("    swap_asm:  swap r24     swap_c:  swap r24");
    Serial.println("               ret                   ret");
    Serial.println();
    Serial.println("  Identical. The compiler recognised the C and used the same");
    Serial.println("  instruction. So: write C first, read the listing, and reach");
    Serial.println("  for asm only where the listing shows a real miss.");
    Serial.println();
    Serial.println("  On this PC there is no AVR, so swap_nibbles() in this course");
    Serial.println("  has the asm under #ifdef __AVR__ and the C version in #else.");
    Serial.print("  Running the C branch:  swap_nibbles(0xA5) = 0x");
    Serial.println(swap_nibbles(0xA5), HEX);

    wait_enter();
    clear_screen();
    heading("PART 3: a critical section, and a whole .S file");

    Serial.println("  Module 11 turned interrupts off with noInterrupts(). If they");
    Serial.println("  were already off, interrupts() would wrongly turn them back");
    Serial.println("  on. The careful way saves the status register SREG, whose top");
    Serial.println("  bit is the interrupt flag, and puts it back:");
    Serial.println();
    Serial.println("    asm volatile(\"in %0, __SREG__ \\n\\t cli\" : \"=r\"(sreg) :: \"memory\");");
    Serial.println("    v = *p;");
    Serial.println("    asm volatile(\"out __SREG__, %0\" :: \"r\"(sreg) : \"memory\");");
    Serial.println();
    Serial.println("  compiled by avr-gcc to:");
    Serial.println();
    Serial.println("    in   r25, 0x3f        ; save SREG");
    Serial.println("    cli                   ; interrupts off");
    Serial.println("    ld   r24, Z           ; the protected read");
    Serial.println("    out  0x3f, r25        ; restore SREG");
    Serial.println();
    Serial.println("  The \"memory\" clobber tells the compiler the asm may touch");
    Serial.println("  memory, so it must not move the read across it. Leave out a");
    Serial.println("  clobber you needed and the compiler keeps a stale copy in a");
    Serial.println("  register: a bug that depends on the optimiser.");
    Serial.println();
    Serial.println("  A whole function can be a file. The Arduino sketch");
    Serial.println("  specification lets a sketch folder contain .S assembly files");
    Serial.println("  next to the .ino ones. avr/swap_nibbles.S is:");
    Serial.println();
    Serial.println("    .global swap_nibbles");
    Serial.println("    swap_nibbles:");
    Serial.println("        swap r24");
    Serial.println("        ret");
    Serial.println();
    Serial.println("  avr-gcc passes the first 8-bit argument in r24 and expects an");
    Serial.println("  8-bit result there, so the function needs no setup. Linked");
    Serial.println("  with C++ code that calls it, main() becomes:");
    Serial.println();
    Serial.println("    ldi  r24, 0xA5");
    Serial.println("    call swap_nibbles");
    Serial.println();
    Serial.println("  About NASM: it assembles x86 and x86-64, not AVR, so it has");
    Serial.println("  no place in this sketch. The AVR assembler is avr-gcc's own");
    Serial.println("  (GNU as); NASM comes up in the C, C++ and Rust courses, for");
    Serial.println("  the PC.");

    wait_enter();
    clear_screen();
    exercise(12);

    question("PORTB |= (1 << 5) compiles to which single AVR instruction?",
             "sbi", "Set Bit in I/O register: the compiler already writes the best one.");
    question("In the constraint \"+r\"(x) is x read only, write only, or both?\n"
             "  (one word)",
             "both", "= is write only, + is read and written, a bare r is read only.");

    {
        const char *task[] = {
            "Write uint8_t swap_nibbles(uint8_t x) that swaps the high and low four",
            "bits. Put the swap instruction under #ifdef __AVR__ and plain C in the",
            "#else. Print swap_nibbles(0xA5) in hexadecimal.",
        };
        const char *expected[] = {"5A"};
        const char *solution[] = {
            "uint8_t swap_nibbles(uint8_t x) {",
            "#ifdef __AVR__",
            "  asm(\"swap %0\" : \"+r\"(x));",
            "#else",
            "  x = (uint8_t)((x << 4) | (x >> 4));",
            "#endif",
            "  return x;",
            "}",
            "",
            "void setup() {",
            "  Serial.begin(9600);",
            "  Serial.println(swap_nibbles(0xA5), HEX);",
            "}",
            "",
            "void loop() { }",
        };
        challenge(task, 3, NULL, 0, expected, 1, solution, 15);
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    Serial.println("   - look at the compiler's listing before writing assembly");
    Serial.println("   - asm volatile(\"nop\") is one cycle: 62.5 ns at 16 MHz");
    Serial.println("   - extended asm: template, outputs, inputs, clobbers; r, d, I, M");
    Serial.println("   - save and restore SREG around a critical section");
    Serial.println("   - a .S file in the sketch folder is a whole assembly function");
    Serial.println();
    Serial.println("  Module 13: everything together, in a robot.");
    wait_enter();
}

/* ---- Lesson 13's robot --------------------------------------------------- */

const uint8_t TRIG_PIN = 7;
const uint8_t ECHO_PIN = 8;
const uint8_t LEFT_PWM = 5;
const uint8_t RIGHT_PWM = 6;

const uint8_t MODE_FORWARD = 0;
const uint8_t MODE_TURN = 1;
const uint8_t MODE_STOP = 2;

uint8_t robot_mode = MODE_FORWARD;
uint32_t robot_mode_since = 0;
uint32_t robot_last_check = 0;

/* An HC-SR04: a 10 microsecond pulse on TRIG starts a measurement, and ECHO
 * stays HIGH for as long as the sound took to go and come back. The
 * datasheet's rule of thumb: microseconds divided by 58 is centimetres. */
int read_distance_cm()
{
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    uint32_t echo_us = pulseIn(ECHO_PIN, HIGH, 30000);
    if (echo_us == 0) {
        return 400; /* no echo within 30 ms: nothing in range */
    }
    return (int) (echo_us / 58);
}

int choose_speed(int distance_cm)
{
    if (distance_cm < 10) {
        return 0;
    }
    if (distance_cm < 30) {
        return 120;
    }
    return 200;
}

void robot_update()
{
    if (millis() - robot_last_check < 100) {
        return; /* not time to look yet; the caller is free to do other work */
    }
    robot_last_check += 100;

    int distance = read_distance_cm();

    if (robot_mode == MODE_FORWARD) {
        if (distance < 10) {
            robot_mode = MODE_STOP;
        } else if (distance < 30) {
            robot_mode = MODE_TURN;
            robot_mode_since = millis();
        }
    } else if (robot_mode == MODE_TURN) {
        if (millis() - robot_mode_since >= 400) {
            robot_mode = MODE_FORWARD;
        }
    }

    if (robot_mode == MODE_FORWARD) {
        int speed = choose_speed(distance);
        analogWrite(LEFT_PWM, speed);
        analogWrite(RIGHT_PWM, speed);
    } else if (robot_mode == MODE_TURN) {
        analogWrite(LEFT_PWM, 150);
        analogWrite(RIGHT_PWM, 0);
    } else {
        analogWrite(LEFT_PWM, 0);
        analogWrite(RIGHT_PWM, 0);
    }
}

void lesson_13_robot()
{
    title("MODULE 13 - FINAL PROJECT: AN OBSTACLE ROBOT");

    heading("PART 1: the sensor");

    Serial.println("  Two motors on PWM pins 5 and 6, and an HC-SR04 ultrasonic");
    Serial.println("  sensor: TRIG on pin 7, ECHO on pin 8. This module uses");
    Serial.println("  almost everything so far: pins, PWM, pulseIn, integer");
    Serial.println("  division, constants, functions, millis().");
    Serial.println();
    Serial.println("    digitalWrite(TRIG_PIN, HIGH);");
    Serial.println("    delayMicroseconds(10);          // the start pulse");
    Serial.println("    digitalWrite(TRIG_PIN, LOW);");
    Serial.println("    uint32_t echo_us = pulseIn(ECHO_PIN, HIGH, 30000);");
    Serial.println("    return echo_us / 58;            // microseconds to cm");
    Serial.println();
    Serial.println("  pulseIn measures how long ECHO stays HIGH, in microseconds,");
    Serial.println("  and gives up after 30000 (30 ms), returning 0. That 0 must");
    Serial.println("  become 'nothing there', not 'touching': 0 cm would stop the");
    Serial.println("  robot for no reason. Here it returns 400.");
    Serial.println();

    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    int examples[3] = {20, 55, 0};
    for (int i = 0; i < 3; i++) {
        sim_set_pulse(ECHO_PIN, (uint32_t) examples[i] * 58); /* PC only */
        Serial.print("  Running:  a wall ");
        Serial.print(examples[i]);
        Serial.print(" cm away (echo ");
        Serial.print((uint32_t) examples[i] * 58);
        Serial.print(" us) reads ");
        Serial.print(read_distance_cm());
        Serial.println(" cm");
    }
    Serial.println("  (the last row is the no-echo case)");

    wait_enter();
    clear_screen();
    heading("PART 2: the decisions");

    Serial.println("  Three modes, a variable saying which one is active, and the");
    Serial.println("  clock deciding when to leave the timed one:");
    Serial.println();
    Serial.println("    FORWARD  drive; closer than 30 cm -> TURN; closer than 10 -> STOP");
    Serial.println("    TURN     pivot for 400 ms, then back to FORWARD");
    Serial.println("    STOP     both motors off");
    Serial.println();
    Serial.println("  robot_update() looks at the sensor every 100 ms and returns");
    Serial.println("  at once otherwise:");
    Serial.println();
    Serial.println("    if (millis() - robot_last_check < 100) return;");
    Serial.println("    robot_last_check += 100;");
    Serial.println();
    Serial.println("  Module 8's pattern, and the reason loop() stays free for");
    Serial.println("  button reads, serial commands, anything else. A version");
    Serial.println("  built on delay(100) would be deaf between looks.");
    Serial.println();
    Serial.println("  And speed comes from a function worth keeping separate and");
    Serial.println("  testable on its own:");
    Serial.println();
    Serial.println("    int choose_speed(int distance_cm) {");
    Serial.println("      if (distance_cm < 10) return 0;");
    Serial.println("      if (distance_cm < 30) return 120;");
    Serial.println("      return 200;");
    Serial.println("    }");

    wait_enter();
    clear_screen();
    heading("PART 3: running it against a simulated room");

    Serial.println("  The world is a distance that shrinks while the robot drives");
    Serial.println("  and grows while it turns. At 2000 ms a hand appears 6 cm in");
    Serial.println("  front. The robot code is exactly robot_update() above.");
    Serial.println();
    Serial.println("    time    distance  mode     left right");

    sim_set_time(0);
    robot_mode = MODE_FORWARD;
    robot_last_check = 0;
    robot_mode_since = 0;
    pinMode(LEFT_PWM, OUTPUT);
    pinMode(RIGHT_PWM, OUTPUT);
    int world_cm = 80;
    const char *mode_names[3] = {"FORWARD", "TURN   ", "STOP   "};

    for (int tick = 1; tick <= 26; tick++) {
        if (tick >= 20) {
            world_cm = 6;
        }
        sim_set_pulse(ECHO_PIN, (uint32_t) world_cm * 58);
        delay(100);
        robot_update();

        if (tick % 2 == 0) {
            Serial.print("    ");
            if (millis() < 1000) {
                Serial.print(' ');
            }
            Serial.print(millis());
            Serial.print(" ms   ");
            if (world_cm < 100) {
                Serial.print(' ');
            }
            if (world_cm < 10) {
                Serial.print(' ');
            }
            Serial.print(world_cm);
            Serial.print(" cm   ");
            Serial.print(mode_names[robot_mode]);
            Serial.print("  ");
            print_padded(sim_pwm[LEFT_PWM], 3);
            Serial.print("   ");
            print_padded(sim_pwm[RIGHT_PWM], 3);
            Serial.println();
        }

        if (robot_mode == MODE_FORWARD && tick < 20) {
            world_cm = world_cm - sim_pwm[LEFT_PWM] / 40;
        } else if (robot_mode == MODE_TURN) {
            world_cm = world_cm + 12;
        }
    }
    sim_set_time(0);

    Serial.println();
    Serial.println("  Read it top to bottom: drive, the wall gets close, it");
    Serial.println("  pivots, the way clears, it drives again, and when the hand");
    Serial.println("  appears it stops and stays stopped.");

    wait_enter();
    clear_screen();
    exercise(13);

    question("An echo pulse of 1160 microseconds. How many centimetres?\n"
             "  (divide by 58)",
             "20", "1160 / 58 = 20.");
    question("To check the sensor every 100 ms and still read a button\n"
             "  in between, use delay() or millis()?",
             "millis", "delay() freezes everything; millis() just asks what time it is.");

    {
        const char *task[] = {
            "Write int choose_speed(int distance_cm): 0 below 10 cm, 120 below 30,",
            "otherwise 200. In setup(), print choose_speed for 5, 20 and 50.",
        };
        const char *expected[] = {"0", "120", "200"};
        const char *solution[] = {
            "int choose_speed(int distance_cm) {",
            "  if (distance_cm < 10) { return 0; }",
            "  if (distance_cm < 30) { return 120; }",
            "  return 200;",
            "}",
            "",
            "void setup() {",
            "  Serial.begin(9600);",
            "  Serial.println(choose_speed(5));",
            "  Serial.println(choose_speed(20));",
            "  Serial.println(choose_speed(50));",
            "}",
            "",
            "void loop() { }",
        };
        challenge(task, 2, NULL, 0, expected, 3, solution, 14);
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    Serial.println("   - split the problem: read the sensor, decide, drive");
    Serial.println("   - a mode variable plus millis() is a state machine that never");
    Serial.println("     blocks");
    Serial.println("   - treat 'no echo' as far away, not as zero");
    Serial.println("   - small pure functions like choose_speed are easy to test");
    Serial.println();
    Serial.println("  That is the course. The MicroPython course covers the same");
    Serial.println("  ideas in Python for the Raspberry Pi Pico.");
    wait_enter();
}
