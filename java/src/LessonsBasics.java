// LessonsBasics.java - modules 1-5.
//
// Package-private (no `public` on the class) so Main.java, in the same
// default package, can call these directly without the file needing to be
// named after this class the way a public class would.

import java.util.List;

class LessonsBasics {

    static void lesson01Running() {
        Ui.title("MODULE 1 - COMPILING AND PRINTING");

        Ui.heading("PART 1: two steps, not one");

        System.out.println("    public class Hello {");
        System.out.println("        public static void main(String[] args) {");
        System.out.println("            System.out.println(\"Hello, World!\");");
        System.out.println("        }");
        System.out.println("    }");
        System.out.println();
        System.out.println("  Running:");
        System.out.println("    javac Hello.java   <- compiles to Hello.class (bytecode)");
        System.out.println("    java Hello          <- the JVM runs that bytecode");
        System.out.println();
        System.out.println("    Hello, World!");
        System.out.println();
        System.out.println("  Two steps, not one. C compiles straight to a machine-code");
        System.out.println("  binary; Python and JavaScript run the source directly, no");
        System.out.println("  separate step at all. Java compiles to bytecode first, and");
        System.out.println("  a virtual machine -- the JVM -- runs that bytecode. Neither");
        System.out.println("  the .java source nor the .class file is directly executable");
        System.out.println("  by the OS the way a C binary is.");
        System.out.println();
        System.out.println("  A newer shortcut: `java Hello.java` alone compiles and runs");
        System.out.println("  it in one command, no .class file left behind -- this course");
        System.out.println("  is launched that exact way. It is still the same two steps");
        System.out.println("  underneath, just without a visible pause between them.");

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.heading("PART 2: the class name IS the file name");

        System.out.println("    public class Bar { ... }     // saved as Foo.java");
        System.out.println();
        System.out.println("  Running: javac Foo.java");
        System.out.println();
        System.out.println("    Foo.java:1: error: class Bar is public, should be declared");
        System.out.println("    in a file named Bar.java");
        System.out.println();
        System.out.println("  Not a style complaint -- a compile error, always, for any");
        System.out.println("  public class. This file is LessonsBasics.java and nothing in");
        System.out.println("  it is declared public for exactly that reason. Main.java, a");
        System.out.println("  few lessons from now, is where public first shows up here.");

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.heading("PART 3: println vs print");

        System.out.println("    System.out.print(\"a\");");
        System.out.println("    System.out.print(\"b\");");
        System.out.println("    System.out.println(\"c\");");
        System.out.println();
        System.out.print("  Running: ");
        System.out.print("a");
        System.out.print("b");
        System.out.println("c");
        System.out.println();
        System.out.println("  print writes nothing extra; println adds the line break.");
        System.out.println("  Same split as Python's print(end=\"\") vs the default, C's");
        System.out.println("  printf needing its own \\n written in by hand.");

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.exercise(1);

        Ui.question(
                "Does javac produce a binary the OS can run directly,\n"
                        + "  the way gcc does for C?  (answer: yes or no)",
                "no",
                "javac produces bytecode (a .class file); the JVM (java) runs that -- two steps, not one.");
        Ui.question(
                "public class Bar sits in a file called Foo.java.\n"
                        + "  Does javac Foo.java compile?  (answer: yes or no)",
                "no",
                "A public class name must match its file name exactly -- a compile error, not a style rule.");

        Ui.challenge(
                List.of("Write a program that prints exactly:", "Learning Java"),
                List.of(),
                List.of("Learning Java"),
                List.of(
                        "public class Test {",
                        "    public static void main(String[] args) {",
                        "        System.out.println(\"Learning Java\");",
                        "    }",
                        "}"));

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.heading("SUMMARY");

        System.out.println("   - javac compiles to bytecode; java runs it on the JVM -- two steps");
        System.out.println("   - a public class's name must match its file name, exactly");
        System.out.println("   - print writes nothing extra; println adds the line break");
        Ui.waitEnter();
    }

    static void lesson02Variables() {
        Ui.title("MODULE 2 - VARIABLES AND TYPES");

        Ui.heading("PART 1: primitives vs objects -- two tiers, not one");

        System.out.println("    int age = 25;              // primitive: a raw value");
        System.out.println("    double height = 1.75;      // primitive");
        System.out.println("    Integer boxedAge = 25;     // object: a value wrapped up");
        System.out.println();

        int age = 25;
        double height = 1.75;
        Integer boxedAge = 25;
        System.out.println("  Running:");
        System.out.println("    age    = " + age);
        System.out.println("    height = " + height);
        System.out.println("    boxedAge = " + boxedAge);
        System.out.println();
        System.out.println("  Every other course in this collection has one kind of value.");
        System.out.println("  Java has two: eight primitive types (int, double, boolean,");
        System.out.println("  char, ...) that hold a raw value directly, and everything");
        System.out.println("  else -- String, Integer, any class -- which is an object,");
        System.out.println("  held by reference. int and Integer look interchangeable.");
        System.out.println("  Part 3 shows exactly where they stop being interchangeable.");

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.heading("PART 2: integer division truncates, same as C");

        System.out.println("    System.out.println(5 / 2);");
        System.out.println();
        System.out.println("  Running: " + (5 / 2));
        System.out.println();
        System.out.println("  int / int gives int -- the .5 is gone, not rounded. Casting");
        System.out.println("  one side after the division is too late; it has to happen");
        System.out.println("  before:");
        System.out.println();
        System.out.println("    (double) 5 / 2   -> " + ((double) 5 / 2));
        System.out.println("    5 / (double) 2    -> " + (5 / (double) 2));

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.heading("PART 3: the Integer caching trap");

        System.out.println("    Integer a = 127, b = 127;");
        System.out.println("    Integer c = 200, d = 200;");
        System.out.println("    System.out.println(a == b);");
        System.out.println("    System.out.println(c == d);");
        System.out.println();

        Integer a = 127, b = 127;
        Integer c = 200, d = 200;
        System.out.println("  Running:");
        System.out.println("    a == b (both 127) -> " + (a == b));
        System.out.println("    c == d (both 200) -> " + (c == d));
        System.out.println();
        System.out.println("  Both pairs hold equal values. The result still differs,");
        System.out.println("  because == on two objects compares REFERENCES, not values --");
        System.out.println("  same rule as C's == on a char*. Java caches boxed Integer");
        System.out.println("  objects for -128..127 and reuses them, so a and b happen to");
        System.out.println("  be the same object. 200 falls outside that range, so c and d");
        System.out.println("  are two separate objects with equal values and unequal");
        System.out.println("  identity. This is a real, well-known trap -- caching is an");
        System.out.println("  implementation detail of Integer specifically, not a language");
        System.out.println("  guarantee, and .equals() is the correct comparison either way:");
        System.out.println("    c.equals(d) -> " + c.equals(d));

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.exercise(2);

        Ui.question(
                "System.out.println(5 / 2);   What prints?",
                "2",
                "int / int truncates toward zero; the .5 is discarded, not rounded.");
        Ui.question(
                "Integer x = 50, y = 50; then x == y.\n"
                        + "  50 is inside the cached range. Equal or not equal?  (answer: equal or not equal)",
                "equal",
                "Both x and y are the same cached object for values -128..127, so == is true here.");
        Ui.question(
                "Integer x = 500, y = 500; then x == y.\n"
                        + "  500 is outside the cached range.  (answer: equal or not equal)",
                "not equal",
                "Two separate objects with equal values -- == compares identity, not value, for boxed types.");

        Ui.challenge(
                List.of(
                        "Declare int total = 7; and double half = total / 2.0;,",
                        "then print half."),
                List.of(),
                List.of("3.5"),
                List.of(
                        "int total = 7;",
                        "double half = total / 2.0;",
                        "System.out.println(half);"));

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.heading("SUMMARY");

        System.out.println("   - primitives (int, double, boolean, ...) hold values directly");
        System.out.println("   - int / int truncates toward zero -- cast before dividing, not after");
        System.out.println("   - boxed Integer == compares identity, not value -- cached only for -128..127");
        Ui.waitEnter();
    }

    static void lesson03Input() {
        Ui.title("MODULE 3 - READING INPUT");

        Ui.heading("PART 1: Scanner");

        System.out.println("    Scanner sc = new Scanner(System.in);");
        System.out.println("    System.out.print(\"Your name: \");");
        System.out.println("    String name = sc.nextLine();");
        System.out.println();
        System.out.print("  Your name: ");
        String name = Ui.IN.nextLine();
        System.out.println("  Running: Hello, " + name + "!");

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.heading("PART 2: nextInt() leaves the newline behind");

        System.out.println("    System.out.print(\"Age: \");");
        System.out.println("    int age = sc.nextInt();");
        System.out.println("    System.out.print(\"Name: \");");
        System.out.println("    String name2 = sc.nextLine();     // <- reads an EMPTY line");
        System.out.println();
        System.out.println("  nextInt() reads only the digits -- the newline you pressed");
        System.out.println("  ENTER to send stays sitting in the buffer. The very next");
        System.out.println("  nextLine() reads up to that leftover newline, gets nothing,");
        System.out.println("  and returns an empty string. Type an age, then a name --");
        System.out.println("  watch the name come back blank:");
        System.out.println();
        System.out.print("  Age: ");
        int age = Ui.IN.nextInt();
        System.out.print("  Name: ");
        String brokenName = Ui.IN.nextLine();
        System.out.println("  Running: age=" + age + " name=[" + brokenName + "]");
        System.out.println();
        System.out.println("  Same bug shape as C++'s cin >> then getline -- a formatted");
        System.out.println("  read leaves the line's newline behind for the next line-read");
        System.out.println("  to trip over. Different API, identical trap.");

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.heading("PART 3: the fix -- consume the leftover newline");

        System.out.println("    int age = sc.nextInt();");
        System.out.println("    sc.nextLine();     // throws away the leftover newline");
        System.out.println("    String name = sc.nextLine();");
        System.out.println();
        System.out.print("  Age: ");
        int age2 = Ui.IN.nextInt();
        Ui.IN.nextLine();
        System.out.print("  Name: ");
        String fixedName = Ui.IN.nextLine();
        System.out.println("  Running: age=" + age2 + " name=[" + fixedName + "]");
        System.out.println();
        System.out.println("  An extra nextLine() with nothing done with its result -- its");
        System.out.println("  only job is consuming that leftover newline before the real");
        System.out.println("  nextLine() call runs.");

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.exercise(3);

        Ui.question(
                "sc.nextInt() reads a number. Does it also consume\n"
                        + "  the newline after it?  (answer: yes or no)",
                "no",
                "nextInt() stops at the digits; the newline is left in the buffer for the next read.");
        Ui.question(
                "int x = sc.nextInt(); followed directly by\n"
                        + "  String s = sc.nextLine();   What does s hold?",
                "empty string",
                "That nextLine() reads up to the leftover newline from nextInt() and finds nothing there.");

        Ui.challenge(
                List.of(
                        "Read an int with nextInt(), consume the leftover newline,",
                        "then read a line with nextLine() and print it."),
                List.of("3", "hello"),
                List.of("hello"),
                List.of(
                        "Scanner sc = new Scanner(System.in);",
                        "int n = sc.nextInt();",
                        "sc.nextLine();",
                        "String line = sc.nextLine();",
                        "System.out.println(line);"));

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.heading("SUMMARY");

        System.out.println("   - Scanner reads input; nextLine() reads a whole line, nextInt() a number");
        System.out.println("   - nextInt() leaves the newline behind -- the next nextLine() reads it as empty");
        System.out.println("   - an extra sc.nextLine() to consume that newline is the fix");
        Ui.waitEnter();
    }

    static void lesson04Conditions() {
        Ui.title("MODULE 4 - CONDITIONS");

        Ui.heading("PART 1: if / else if / else");

        System.out.println("    int n = 7;");
        System.out.println("    if (n % 2 == 0) {");
        System.out.println("        System.out.println(\"even\");");
        System.out.println("    } else {");
        System.out.println("        System.out.println(\"odd\");");
        System.out.println("    }");
        System.out.println();
        int n = 7;
        System.out.print("  Running: ");
        if (n % 2 == 0) {
            System.out.println("even");
        } else {
            System.out.println("odd");
        }

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.heading("PART 2: a condition MUST be boolean -- nothing else compiles");

        System.out.println("    int x = 5;");
        System.out.println("    if (x) { ... }          // does NOT compile");
        System.out.println();
        System.out.println("    Test.java:3: error: incompatible types: int cannot be");
        System.out.println("    converted to boolean");
        System.out.println();
        System.out.println("  C treats any nonzero int as true. Python and JavaScript both");
        System.out.println("  have truthy/falsy rules for values that are not literally");
        System.out.println("  true or false. Java has none of that: if's condition must be");
        System.out.println("  a boolean expression, or it is a compile error, full stop.");

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.heading("PART 3: = vs == inside a condition -- a compile error, not a warning");

        System.out.println("    int x = 5;");
        System.out.println("    if (x = 5) { ... }      // does NOT compile");
        System.out.println();
        System.out.println("    Test.java:3: error: incompatible types: int cannot be");
        System.out.println("    converted to boolean");
        System.out.println();
        System.out.println("  x = 5 is itself an expression -- it assigns 5 and evaluates");
        System.out.println("  to 5, an int. C and C++ both accept that int inside if() and");
        System.out.println("  only warn, if anything is said at all -- the classic typed-=");
        System.out.println("  -instead-of-== bug those two courses both cover. Because a");
        System.out.println("  Java condition must be boolean, and int is never boolean,");
        System.out.println("  the exact same typo is a compile error here instead of a");
        System.out.println("  runtime surprise.");

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.exercise(4);

        Ui.question(
                "int x = 3; if (x) { ... }   Does this compile?  (answer: yes or no)",
                "no",
                "A condition must be boolean. int is never accepted, unlike C.");
        Ui.question(
                "int x = 5; if (x = 5) { ... }   Compile error, or a\n"
                        + "  silently-true condition like C?  (answer: error or silent)",
                "error",
                "x = 5 evaluates to an int, and int cannot be used as a boolean condition here.");

        Ui.challenge(
                List.of(
                        "Read an int with Scanner. Print \"positive\" if it is > 0,",
                        "\"negative\" if it is < 0, otherwise print \"zero\"."),
                List.of("-4"),
                List.of("negative"),
                List.of(
                        "Scanner sc = new Scanner(System.in);",
                        "int n = sc.nextInt();",
                        "if (n > 0) {",
                        "    System.out.println(\"positive\");",
                        "} else if (n < 0) {",
                        "    System.out.println(\"negative\");",
                        "} else {",
                        "    System.out.println(\"zero\");",
                        "}"));

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.heading("SUMMARY");

        System.out.println("   - if / else if / else, same shape as every C-family language");
        System.out.println("   - a condition must be boolean -- no truthy ints, no exceptions");
        System.out.println("   - if (x = 5) is a compile error here, not a runtime typo like C/C++");
        Ui.waitEnter();
    }

    static void lesson05Loops() {
        Ui.title("MODULE 5 - LOOPS");

        Ui.heading("PART 1: while and for");

        System.out.println("    int i = 0;");
        System.out.println("    while (i < 3) {");
        System.out.println("        System.out.println(i);");
        System.out.println("        i++;");
        System.out.println("    }");
        System.out.println();
        System.out.println("  Running:");
        int i = 0;
        while (i < 3) {
            System.out.println("    " + i);
            i++;
        }
        System.out.println();
        System.out.println("    for (int j = 0; j < 3; j++) {");
        System.out.println("        System.out.println(j);");
        System.out.println("    }");
        System.out.println();
        System.out.println("  Running:");
        for (int j = 0; j < 3; j++) {
            System.out.println("    " + j);
        }
        System.out.println();
        System.out.println("  j exists only inside the for -- unlike i above, it is not");
        System.out.println("  usable after the loop ends. Scoped to the loop, on purpose.");

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.heading("PART 2: the enhanced for -- values, no index");

        System.out.println("    int[] nums = {10, 20, 30};");
        System.out.println("    for (int x : nums) {");
        System.out.println("        System.out.println(x);");
        System.out.println("    }");
        System.out.println();
        int[] nums = {10, 20, 30};
        System.out.println("  Running:");
        for (int x : nums) {
            System.out.println("    " + x);
        }
        System.out.println();
        System.out.println("  Arrays are properly taught next module -- for now, {10, 20,");
        System.out.println("  30} is just a fixed list of ints. for (int x : nums) reads");
        System.out.println("  \"for each x in nums\": x is a value, not a position, and there");
        System.out.println("  is no index variable to manage or get off-by-one on.");

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.heading("PART 3: break and continue");

        System.out.println("    for (int k = 0; k < 10; k++) {");
        System.out.println("        if (k == 3) continue;");
        System.out.println("        if (k == 6) break;");
        System.out.println("        System.out.println(k);");
        System.out.println("    }");
        System.out.println();
        System.out.println("  Running:");
        for (int k = 0; k < 10; k++) {
            if (k == 3) {
                continue;
            }
            if (k == 6) {
                break;
            }
            System.out.println("    " + k);
        }
        System.out.println();
        System.out.println("  continue skips straight to the next iteration; break leaves");
        System.out.println("  the loop entirely. 3 is missing above, and nothing past 6");
        System.out.println("  ever prints.");

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.exercise(5);

        Ui.question(
                "for (int j = 0; j < 3; j++) { ... }   Is j usable\n"
                        + "  in code right after this loop?  (answer: yes or no)",
                "no",
                "A for loop's counter is scoped to the loop -- it does not exist outside it.");
        Ui.question(
                "for (int x : nums)   Does x give you the index\n"
                        + "  or the value?  (answer: index or value)",
                "value",
                "The enhanced for gives values directly -- there is no index variable in this form.");

        Ui.challenge(
                List.of(
                        "int[] nums = {2, 4, 6, 8}; -- print the sum of its values",
                        "using an enhanced for loop."),
                List.of(),
                List.of("20"),
                List.of(
                        "int[] nums = {2, 4, 6, 8};",
                        "int sum = 0;",
                        "for (int x : nums) {",
                        "    sum += x;",
                        "}",
                        "System.out.println(sum);"));

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.heading("SUMMARY");

        System.out.println("   - while and for work the same as every C-family language here");
        System.out.println("   - a for loop's counter is scoped to the loop, not usable after it");
        System.out.println("   - for (T x : collection) gives values, no index to manage");
        Ui.waitEnter();
    }
}
