/*
 * Modules 7 to 9 - analog values, time, and text.
 */

void lesson_07_analog()
{
    title("MODULE 7 - ANALOG VALUES AND map()");

    heading("PART 1: analogRead, and arithmetic on a 16-bit chip");

    Serial.println("  digitalRead sees two states. analogRead measures a voltage");
    Serial.println("  on A0-A5 and returns a number from 0 (0 V) to 1023 (5 V):");
    Serial.println("  the Uno's converter has 10 bits, and 2^10 is 1024 levels.");
    Serial.println();

    sim_set_analog(0, 512);
    int raw = analogRead(A0);
    Serial.println("    sim_set_analog(0, 512);     // PC only: put a voltage on A0");
    Serial.println("    int raw = analogRead(A0);");
    Serial.print("  Running:  raw = ");
    Serial.println(raw);
    Serial.println();
    Serial.println("  Back to volts: raw * 5.0 / 1023.");
    Serial.print("  Running:  ");
    Serial.print(raw * 5.0 / 1023);
    Serial.println(" V");
    Serial.println();
    Serial.println("  Now millivolts, with whole numbers. The obvious line is");
    Serial.println("  raw * 5000 / 1023, and the product is the trap: 1023 * 5000");
    Serial.println("  is 5115000, far past what 16 bits hold (65535). On an Uno,");
    Serial.println("  where int is 16 bits, that overflows, and with plain int it");
    Serial.println("  is undefined behaviour, so it is not run. With unsigned 16-bit");
    Serial.println("  values it wraps, and the PC can show the same wrap by casting");
    Serial.println("  the product down to 16 bits:");
    Serial.println();

    uint16_t full = 1023;
    uint16_t wrapped = (uint16_t) (full * 5000);
    uint32_t widened = (uint32_t) full * 5000 / 1023;
    Serial.println("    uint16_t full = 1023;");
    Serial.println("    (uint16_t)(full * 5000) / 1023     // what a 16-bit chip computes");
    Serial.println("    (uint32_t)full * 5000 / 1023       // widen BEFORE multiplying");
    Serial.print("  Running:  ");
    Serial.print(wrapped / 1023);
    Serial.print(" mV the 16-bit way, ");
    Serial.print(widened);
    Serial.println(" mV the right way");
    Serial.println();
    Serial.println("  Fix: make one operand 32 bits before the multiplication.");
    Serial.println();
    Serial.println("  The PC only imitates the 16-bit wrap. The same two lines were");
    Serial.println("  also built for the ATmega328P and run on a simulated one");
    Serial.println("  (simavr, by avr/run-avr.py), which gave 3 and 5000.");

    wait_enter();
    clear_screen();
    heading("PART 2: analogWrite is PWM, not a voltage");

    Serial.println("  analogWrite(pin, 0..255) does not output 2.5 V. It switches");
    Serial.println("  the pin on and off very fast and varies how long it is on");
    Serial.println("  -- the duty cycle. A motor or LED averages that out. It is");
    Serial.println("  8 bits: 0 is always off, 255 always on.");
    Serial.println();
    Serial.println("    analogWrite(9, 64);");
    analogWrite(9, 64);
    Serial.print("  Running:  duty on pin 9 = ");
    Serial.print(sim_pwm[9] * 100 / 255);
    Serial.println("%");
    Serial.println();
    Serial.println("  Only some pins can do it. On an Uno: 3, 5, 6, 9, 10, 11.");
    Serial.println("  On any other pin analogWrite does not fail. It quietly");
    Serial.println("  falls back to digital: HIGH from 128 up, LOW below.");
    Serial.println();
    Serial.println("    analogWrite(7, 200);  analogWrite(7, 100);");
    analogWrite(7, 200);
    Serial.print("  Running:  pin 7 after 200 reads ");
    Serial.println(digitalRead(7) ? "HIGH" : "LOW");
    analogWrite(7, 100);
    Serial.print("            pin 7 after 100 reads ");
    Serial.println(digitalRead(7) ? "HIGH" : "LOW");

    wait_enter();
    clear_screen();
    heading("PART 3: map() and constrain()");

    Serial.println("  A sensor gives 0-1023 but analogWrite wants 0-255.");
    Serial.println("  map(value, from_low, from_high, to_low, to_high) rescales:");
    Serial.println();
    Serial.println("    map(x, 0, 1023, 0, 255)");
    Serial.println();
    int inputs[4] = {0, 512, 1023, 1200};
    for (int i = 0; i < 4; i++) {
        Serial.print("  Running:  x = ");
        Serial.print(inputs[i]);
        Serial.print("   ->  ");
        Serial.println(map(inputs[i], 0, 1023, 0, 255));
    }
    Serial.println();
    Serial.println("  Two things to see. 512 gives 127, not 127.5: map works in");
    Serial.println("  whole numbers and truncates. And 1200 gives a number past");
    Serial.println("  255: map does NOT clamp. constrain(x, low, high) does:");
    Serial.println();
    Serial.println("    constrain(map(1200, 0, 1023, 0, 255), 0, 255)");
    Serial.print("  Running:  ");
    Serial.println(constrain(map(1200, 0, 1023, 0, 255), 0, 255));

    wait_enter();
    clear_screen();
    exercise(7);

    question("analogRead returns a value from 0 up to what?", "1023",
             "The converter has 10 bits, so 1024 levels, 0 to 1023.");
    question("analogWrite(7, 200) on an Uno. Pin 7 is not a PWM pin.\n"
             "  What does the pin do? (HIGH or LOW)",
             "HIGH", "Off a PWM pin it falls back to digital: 128 or more is HIGH.");

    {
        const char *task[] = {
            "In setup(), call sim_set_analog(0, 700) to pretend A0 reads 700 (a",
            "PC-only call). Read A0 and print that value mapped from 0..1023",
            "onto 0..100.",
        };
        const char *expected[] = {"68"};
        const char *solution[] = {
            "void setup() {",
            "  Serial.begin(9600);",
            "  sim_set_analog(0, 700);",
            "  int raw = analogRead(A0);",
            "  Serial.println(map(raw, 0, 1023, 0, 100));",
            "}",
            "",
            "void loop() { }",
        };
        challenge(task, 3, NULL, 0, expected, 1, solution, 8);
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    Serial.println("   - analogRead is 0-1023; analogWrite is 0-255 and only on PWM pins");
    Serial.println("   - PWM is fast on/off, not a real voltage");
    Serial.println("   - on a 16-bit int chip, widen to 32 bits BEFORE multiplying");
    Serial.println("   - map() truncates and does not clamp; follow it with constrain()");
    Serial.println();
    Serial.println("  Module 8: waiting without freezing the board.");
    wait_enter();
}

/* A toy loop for lesson 8. Each pass costs `work_us` of simulated time, the
 * way real code takes real time. Returns how many times the pass ran. */
int count_checks_with_delay(uint32_t run_ms)
{
    uint32_t start = millis();
    int checks = 0;
    while (millis() - start < run_ms) {
        checks++;
        delayMicroseconds(100); /* the work of checking a button */
        delay(500);             /* the blocking wait */
    }
    return checks;
}

int count_checks_with_millis(uint32_t run_ms)
{
    uint32_t start = millis();
    uint32_t last_blink = start;
    int checks = 0;
    while (millis() - start < run_ms) {
        checks++;
        delayMicroseconds(100); /* the same work of checking a button */
        if (millis() - last_blink >= 500) {
            last_blink += 500;
        }
    }
    return checks;
}

void lesson_08_time()
{
    title("MODULE 8 - TIME WITHOUT FREEZING");

    heading("PART 1: what delay() costs");

    Serial.println("  A robot has to blink a light every 500 ms AND notice a");
    Serial.println("  button. With delay(500) the chip does nothing for half a");
    Serial.println("  second at a time. Counting how many times each version gets");
    Serial.println("  to check the button in 3 seconds (simulated time; each");
    Serial.println("  check takes 0.1 ms of work):");
    Serial.println();

    sim_set_time(0);
    int with_delay = count_checks_with_delay(3000);
    sim_set_time(0);
    int with_millis = count_checks_with_millis(3000);
    Serial.print("  Running:  with delay(500)    ");
    Serial.print(with_delay);
    Serial.println(" checks");
    Serial.print("            with millis()      ");
    Serial.print(with_millis);
    Serial.println(" checks");
    Serial.println();
    Serial.println("  Both blink at the same rate. One looks at the button");
    Serial.println("  every half second and the other every tenth of a");
    Serial.println("  millisecond. A button press shorter than 500 ms can fall");
    Serial.println("  entirely inside a delay() and never be seen.");

    wait_enter();
    clear_screen();
    heading("PART 2: the millis() pattern");

    Serial.println("  millis() is the time since power-on, in milliseconds.");
    Serial.println("  Instead of waiting, ask whether enough time has passed:");
    Serial.println();
    Serial.println("    if (millis() - last >= interval) {");
    Serial.println("      last += interval;");
    Serial.println("      ...do the periodic thing...");
    Serial.println("    }");
    Serial.println();

    sim_set_time(0);
    uint32_t last = 0;
    Serial.print("  Running, 1 ms per pass, interval 500, toggles at:");
    while (millis() <= 2000) {
        if (millis() - last >= 500) {
            last += 500;
            Serial.print(' ');
            Serial.print(millis());
        }
        delay(1);
    }
    Serial.println();
    Serial.println();
    Serial.println("  `last += interval` keeps the rhythm exact. The common");
    Serial.println("  `last = millis()` restarts the count at each toggle and");
    Serial.println("  adds the pass-time delay to every period, so it drifts:");
    Serial.println();

    sim_set_time(0);
    last = 0;
    Serial.print("  Running, 3 ms per pass, last = millis(), toggles at:");
    while (millis() <= 2000) {
        if (millis() - last >= 500) {
            last = millis();
            Serial.print(' ');
            Serial.print(millis());
        }
        delay(3);
    }
    Serial.println();

    wait_enter();
    clear_screen();
    heading("PART 3: when millis() wraps around");

    Serial.println("  millis() is a uint32_t, so it counts to 4294967295 and");
    Serial.println("  then goes back to 0. In days:");
    Serial.println();
    Serial.println("    4294967296.0 / 1000 / 60 / 60 / 24");
    Serial.print("  Running:  ");
    Serial.print(4294967296.0 / 1000 / 60 / 60 / 24);
    Serial.println(" days");
    Serial.println();
    Serial.println("  A robot that stays on for 50 days will meet it. Two ways to");
    Serial.println("  test for 'has 10 ms passed?', right at the wrap:");
    Serial.println();

    sim_set_time(4294967290u);
    uint32_t start = millis();
    uint32_t wait = 10;
    delay(1);
    uint32_t now = millis();
    Serial.println("    start = millis();       // 4294967290, 6 ms before the wrap");
    Serial.println("    ... 1 ms later ...");
    Serial.print("  Running:  now = ");
    Serial.print(now);
    Serial.print(", start + wait = ");
    Serial.println(start + wait);
    Serial.print("            now >= start + wait   is  ");
    Serial.println(now >= start + wait ? "true  (WRONG: only 1 ms passed)" : "false");
    Serial.print("            now - start >= wait   is  ");
    Serial.println(now - start >= wait ? "true" : "false (right: only 1 ms passed)");
    Serial.println();
    Serial.println("  start + wait wrapped to a small number, so `now >=` fires");
    Serial.println("  at once. Subtracting works because unsigned arithmetic");
    Serial.println("  wraps the same way: now - start is the true elapsed time");
    Serial.println("  even across the wrap. Always write millis() - start.");
    sim_set_time(0);

    wait_enter();
    clear_screen();
    exercise(8);

    question("millis() is a uint32_t that counts milliseconds. About how\n"
             "  many days until it wraps? (nearest whole number)",
             "50", "2^32 ms is about 49.7 days.");
    question("Which test survives the wrap?  1) now >= start + wait\n"
             "  2) now - start >= wait    (type 1 or 2)",
             "2", "Unsigned subtraction gives the true elapsed time across the wrap.");

    {
        const char *task[] = {
            "Print the word tick every 250 ms of simulated time, WITHOUT calling",
            "delay(250). Use millis(). To let time pass on the PC, end loop()",
            "with delay(50). Run it for 21 passes and it must print 4 ticks:",
            "make try FILE=test.ino LOOPS=21",
        };
        const char *expected[] = {"tick", "tick", "tick", "tick"};
        const char *solution[] = {
            "uint32_t last = 0;",
            "",
            "void setup() {",
            "  Serial.begin(9600);",
            "}",
            "",
            "void loop() {",
            "  if (millis() - last >= 250) {",
            "    last += 250;",
            "    Serial.println(\"tick\");",
            "  }",
            "  delay(50);",
            "}",
        };
        challenge(task, 4, NULL, 0, expected, 4, solution, 13);
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    Serial.println("   - delay() makes the chip deaf; millis() lets it keep working");
    Serial.println("   - if (millis() - last >= interval) { last += interval; ... }");
    Serial.println("   - millis() wraps after about 49.7 days");
    Serial.println("   - always subtract: millis() - start, never compare to a sum");
    Serial.println();
    Serial.println("  Module 9: text, and what it costs on a chip this small.");
    wait_enter();
}

void lesson_09_text()
{
    title("MODULE 9 - TEXT: CHAR ARRAYS AND String");

    heading("PART 1: C strings, and the extra byte");

    Serial.println("  Text in C and C++ is an array of char that ends with a zero");
    Serial.println("  byte, '\\0', the terminator. The functions that read text");
    Serial.println("  keep going until they find it.");
    Serial.println();

    char word[] = "robot";
    Serial.println("    char word[] = \"robot\";");
    Serial.print("  Running:  sizeof(word) = ");
    Serial.print(sizeof(word));
    Serial.print(",  strlen(word) = ");
    Serial.println(strlen(word));
    Serial.println();
    Serial.println("  Five letters, six bytes: the sixth is the terminator.");
    Serial.println("  strlen counts to the terminator; sizeof counts the bytes");
    Serial.println("  of the array. A buffer for N letters needs N + 1.");
    Serial.println();
    Serial.println("  Squeezing it into exactly five is refused:");
    Serial.println();
    Serial.println("    char word[5] = \"robot\";");
    Serial.println("    error: initializer-string for 'char [5]' is too long");

    wait_enter();
    clear_screen();
    heading("PART 2: the String class");

    Serial.println("  Arduino also has String, an object that grows by itself:");
    Serial.println();
    String label = "speed=";
    label += 42;
    Serial.println("    String label = \"speed=\";");
    Serial.println("    label += 42;");
    Serial.print("  Running:  ");
    Serial.print(label);
    Serial.print("   length ");
    Serial.println(label.length());
    Serial.println();
    String greeting = "hello robot";
    Serial.println("    String greeting = \"hello robot\";");
    Serial.print("  Running:  indexOf(' ') = ");
    Serial.print(greeting.indexOf(' '));
    Serial.print(",  substring(6) = ");
    Serial.println(greeting.substring(6));
    Serial.println();
    Serial.println("  It is far more comfortable, and it has a cost the PC");
    Serial.println("  cannot show: a String lives in the heap, and building and");
    Serial.println("  dropping Strings in a loop can fragment the Uno's 2 KB until");
    Serial.println("  an allocation fails and the sketch misbehaves, with no");
    Serial.println("  message. The Arduino documentation advises char arrays for");
    Serial.println("  anything that runs for a long time.");

    wait_enter();
    clear_screen();
    heading("PART 3: pulling numbers out of a command");

    Serial.println("  A robot gets 'M 120 -40': motor command, left, right. A whole");
    Serial.println("  line off Serial arrives as a String:");
    Serial.println();
    Serial.println("    String line = Serial.readStringUntil('\\n');   // up to ENTER");
    Serial.println();
    Serial.println("  Here the line is typed in directly, so this runs without you:");
    Serial.println();
    String command = "M 120 -40";
    int first = command.indexOf(' ');
    int second = command.indexOf(' ', first + 1);
    int left = (int) command.substring(first + 1, second).toInt();
    int right = (int) command.substring(second + 1).toInt();
    Serial.println("    int first  = command.indexOf(' ');");
    Serial.println("    int second = command.indexOf(' ', first + 1);");
    Serial.println("    left  = command.substring(first + 1, second).toInt();");
    Serial.println("    right = command.substring(second + 1).toInt();");
    Serial.print("  Running:  left = ");
    Serial.print(left);
    Serial.print(", right = ");
    Serial.println(right);
    Serial.println();
    Serial.println("  The same with a char array and sscanf, no String:");
    Serial.println();
    char buffer[] = "M 120 -40";
    int left2 = 0;
    int right2 = 0;
    sscanf(buffer, "M %d %d", &left2, &right2);
    Serial.println("    sscanf(buffer, \"M %d %d\", &left, &right);");
    Serial.print("  Running:  left = ");
    Serial.print(left2);
    Serial.print(", right = ");
    Serial.println(right2);
    Serial.println();
    Serial.println("  The & hands sscanf the address to write into. One catch on");
    Serial.println("  a real Uno: its sscanf does not read decimals (%f) by");
    Serial.println("  default, so whole numbers only.");

    wait_enter();
    clear_screen();
    exercise(9);

    question("char word[] = \"hi\";   What is sizeof(word)?", "3",
             "Two letters plus the terminating zero byte.");
    question("String s = \"ab\";  s += 12;   What is s.length()?", "4",
             "+= with a number appends its digits: ab12 is four characters.");

    {
        const char *task[] = {
            "Read one line from Serial shaped like  SPEED 75  and print",
            "  speed is 75  using the number from the line.",
        };
        const char *input[] = {"SPEED 75"};
        const char *expected[] = {"speed is 75"};
        const char *solution[] = {
            "void setup() {",
            "  Serial.begin(9600);",
            "  String line = Serial.readStringUntil('\\n');",
            "  int space = line.indexOf(' ');",
            "  int value = line.substring(space + 1).toInt();",
            "  Serial.print(\"speed is \");",
            "  Serial.println(value);",
            "}",
            "",
            "void loop() { }",
        };
        challenge(task, 2, input, 1, expected, 1, solution, 10);
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    Serial.println("   - text is a char array plus a terminating zero: N letters, N+1");
    Serial.println("   - strlen counts letters, sizeof counts the array");
    Serial.println("   - String is comfortable but lives in the heap: avoid it in");
    Serial.println("     code that runs for days");
    Serial.println("   - indexOf, substring and toInt, or sscanf, pull numbers out");
    Serial.println();
    Serial.println("  Module 10: memory, bits and the chip's own registers.");
    wait_enter();
}
