// LessonsMore.java - modules 6-10.

import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

class LessonsMore {

    static void lesson06ArraysStrings() {
        Ui.title("MODULE 6 - ARRAYS AND STRINGS");

        Ui.heading("PART 1: arrays are fixed-size, and .length is a FIELD");

        System.out.println("    int[] scores = {90, 85, 77};");
        System.out.println("    System.out.println(scores.length);   // no parentheses");
        System.out.println();
        int[] scores = {90, 85, 77};
        System.out.println("  Running: " + scores.length);
        System.out.println();
        System.out.println("  Once created with size 3, scores can never hold 4 -- there");
        System.out.println("  is no append. .length is a plain field, not a method call;");
        System.out.println("  Part 3 shows the exact opposite choice for String, which is");
        System.out.println("  a real, common mix-up in both directions.");

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.heading("PART 2: String is immutable, and == compares identity, not content");

        System.out.println("    String s1 = \"hi\";");
        System.out.println("    String s2 = \"hi\";");
        System.out.println("    String s3 = new String(\"hi\");");
        System.out.println("    System.out.println(s1 == s2);");
        System.out.println("    System.out.println(s1 == s3);");
        System.out.println("    System.out.println(s1.equals(s3));");
        System.out.println();

        String s1 = "hi";
        String s2 = "hi";
        String s3 = new String("hi");
        System.out.println("  Running:");
        System.out.println("    s1 == s2 (both literals)   -> " + (s1 == s2));
        System.out.println("    s1 == s3 (s3 via new)      -> " + (s1 == s3));
        System.out.println("    s1.equals(s3)               -> " + s1.equals(s3));
        System.out.println();
        System.out.println("  Two identical string LITERALS share one object from Java's");
        System.out.println("  string pool, so == happens to return true for s1 and s2 --");
        System.out.println("  but that is identity, not content, the same trap module 2's");
        System.out.println("  boxed Integer had. new String(\"hi\") deliberately makes a");
        System.out.println("  second, separate object with the same content, and == sees");
        System.out.println("  straight through the coincidence. .equals() is always the");
        System.out.println("  correct comparison for content -- unlike JavaScript, where");
        System.out.println("  == does compare string content directly.");

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.heading("PART 3: String.length() is a METHOD");

        System.out.println("    System.out.println(s1.length());   // parentheses required");
        System.out.println();
        System.out.println("  Running: " + s1.length());
        System.out.println();
        System.out.println("  scores.length has no parentheses; s1.length() needs them.");
        System.out.println("  Array size is a field because an array is not a full object");
        System.out.println("  with methods -- it is a lower-level construct baked into the");
        System.out.println("  language. String is an ordinary class, so its size comes from");
        System.out.println("  calling a method like any other. Mixing the two up is a");
        System.out.println("  compile error either way, at least -- never a silent bug.");

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.exercise(6);

        Ui.question(
                "int[] arr = {1, 2, 3};   Is arr.length written\n"
                        + "  with parentheses or without?  (answer: with or without)",
                "without",
                "Array length is a field, accessed directly -- arr.length, never arr.length().");
        Ui.question(
                "String s = \"hi\";   Is s.length() written\n"
                        + "  with parentheses or without?  (answer: with or without)",
                "with",
                "String is a class; length() is a method call, unlike an array's .length field.");
        Ui.question(
                "String a = new String(\"x\"); String b = new String(\"x\");\n"
                        + "  Does a == b?  (answer: yes or no)",
                "no",
                "Two separate objects with equal content -- == compares identity; .equals() compares content.");

        Ui.challenge(
                List.of(
                        "int[] nums = {4, 8, 15, 16, 23, 42}; -- print how many",
                        "elements it has, then print the value of the last one."),
                List.of(),
                List.of("6", "42"),
                List.of(
                        "int[] nums = {4, 8, 15, 16, 23, 42};",
                        "System.out.println(nums.length);",
                        "System.out.println(nums[nums.length - 1]);"));

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.heading("SUMMARY");

        System.out.println("   - arrays are fixed-size; .length is a field, no parentheses");
        System.out.println("   - String is immutable; .length() is a method, parentheses required");
        System.out.println("   - == compares identity, not content -- .equals() for content, always");
        Ui.waitEnter();
    }

    static void lesson07Methods() {
        Ui.title("MODULE 7 - METHODS AND OVERLOADING");

        Ui.heading("PART 1: a method, called");

        System.out.println("    static int square(int n) {");
        System.out.println("        return n * n;");
        System.out.println("    }");
        System.out.println();
        System.out.println("  Running: square(6) -> " + square(6));

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.heading("PART 2: no default arguments -- Java answers with overloading instead");

        System.out.println("    static String greet(String name) {");
        System.out.println("        return greet(name, \"Hello\");");
        System.out.println("    }");
        System.out.println("    static String greet(String name, String salutation) {");
        System.out.println("        return salutation + \", \" + name + \"!\";");
        System.out.println("    }");
        System.out.println();
        System.out.println("  Running:");
        System.out.println("    greet(\"Ana\")            -> " + greet("Ana"));
        System.out.println("    greet(\"Ana\", \"Hi\")     -> " + greet("Ana", "Hi"));
        System.out.println();
        System.out.println("  Python and JavaScript both let one function declare a default");
        System.out.println("  value right in its parameter list. Java has no such syntax --");
        System.out.println("  two methods sharing a name but differing in parameter list is");
        System.out.println("  how the same effect is reached. The compiler picks the method");
        System.out.println("  whose parameter list matches the call, at compile time. That");
        System.out.println("  also sidesteps the Python course's whole module 7 trap --");
        System.out.println("  a default value shared and mutated across calls -- since each");
        System.out.println("  overload is a distinct method with nothing to share.");

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.heading("PART 3: overloads are chosen by parameter TYPE, not by name");

        System.out.println("    static String describe(int n) { return n + \" (int)\"; }");
        System.out.println("    static String describe(double n) { return n + \" (double)\"; }");
        System.out.println();
        System.out.println("  Running:");
        System.out.println("    describe(5)      -> " + describe(5));
        System.out.println("    describe(5.0)     -> " + describe(5.0));
        System.out.println();
        System.out.println("  Same call syntax, different method run -- decided by which");
        System.out.println("  parameter list the argument's type matches, resolved before");
        System.out.println("  the program ever runs.");

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.exercise(7);

        Ui.question(
                "Does Java let a method declare a default value\n"
                        + "  directly in its parameter list, the way Python does?  (answer: yes or no)",
                "no",
                "Java has no default-argument syntax -- overloading (multiple methods, same name) is the answer.");
        Ui.question(
                "Two overloads of describe() exist: one for int, one\n"
                        + "  for double. Is the one that runs picked at compile time\n"
                        + "  or at runtime?  (answer: compile time or runtime)",
                "compile time",
                "Overload resolution happens at compile time, based on the argument types the compiler sees.");

        Ui.challenge(
                List.of(
                        "Write an overloaded method max(): one version taking two",
                        "ints, one taking three ints, each returning the largest.",
                        "Call both and print the results."),
                List.of(),
                List.of("7", "9"),
                List.of(
                        "static int max(int a, int b) {",
                        "    return a > b ? a : b;",
                        "}",
                        "static int max(int a, int b, int c) {",
                        "    return max(max(a, b), c);",
                        "}",
                        "",
                        "public static void main(String[] args) {",
                        "    System.out.println(max(3, 7));",
                        "    System.out.println(max(9, 2, 5));",
                        "}"));

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.heading("SUMMARY");

        System.out.println("   - no default-argument syntax -- overloading fills the same need");
        System.out.println("   - overloads differ by parameter list, chosen at compile time");
        System.out.println("   - each overload is a separate method -- nothing shared or mutated between calls");
        Ui.waitEnter();
    }

    private static int square(int n) {
        return n * n;
    }

    private static String greet(String name) {
        return greet(name, "Hello");
    }

    private static String greet(String name, String salutation) {
        return salutation + ", " + name + "!";
    }

    private static String describe(int n) {
        return n + " (int)";
    }

    private static String describe(double n) {
        return n + " (double)";
    }

    static void lesson08ArrayList() {
        Ui.title("MODULE 8 - ARRAYLIST AND BOXING");

        Ui.heading("PART 1: ArrayList grows -- an array cannot");

        System.out.println("    ArrayList<String> names = new ArrayList<>();");
        System.out.println("    names.add(\"Ana\");");
        System.out.println("    names.add(\"Bea\");");
        System.out.println("    System.out.println(names.size());");
        System.out.println("    System.out.println(names.get(0));");
        System.out.println();

        ArrayList<String> names = new ArrayList<>();
        names.add("Ana");
        names.add("Bea");
        System.out.println("  Running:");
        System.out.println("    size()  -> " + names.size());
        System.out.println("    get(0)  -> " + names.get(0));
        System.out.println();
        System.out.println("  Module 6's array is fixed the moment it is created; add()");
        System.out.println("  here grows the list one element at a time, size() replaces");
        System.out.println("  .length, and there is no [] -- get(i) and set(i, value)");
        System.out.println("  instead.");

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.heading("PART 2: ArrayList<int> does not compile -- it needs the boxed type");

        System.out.println("    ArrayList<int> nums = new ArrayList<int>();   // does NOT compile");
        System.out.println();
        System.out.println("    Test.java:1: error: unexpected type");
        System.out.println("    required: reference");
        System.out.println("    found:    int");
        System.out.println();
        System.out.println("  A generic type parameter -- the <...> part -- must be a real");
        System.out.println("  object type, and int is a primitive, module 2's raw-value");
        System.out.println("  tier, not the object tier. The fix is the boxed wrapper from");
        System.out.println("  that same module:");
        System.out.println();
        System.out.println("    ArrayList<Integer> nums = new ArrayList<>();");
        System.out.println("    nums.add(5);          // auto-boxed to Integer for you");
        System.out.println("    int first = nums.get(0);   // auto-unboxed back to int");
        System.out.println();

        ArrayList<Integer> nums = new ArrayList<>();
        nums.add(5);
        nums.add(12);
        int first = nums.get(0);
        System.out.println("  Running: nums = " + nums + ", first (unboxed) = " + first);
        System.out.println();
        System.out.println("  add(5) autoboxes the int into an Integer behind the scenes;");
        System.out.println("  get(0) autounboxes it back. The conversion is automatic --");
        System.out.println("  the requirement that the type itself be Integer, not int, is");
        System.out.println("  not.");

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.heading("PART 3: looping over an ArrayList");

        System.out.println("    for (int n : nums) {");
        System.out.println("        System.out.println(n);");
        System.out.println("    }");
        System.out.println();
        System.out.println("  Running:");
        for (int n : nums) {
            System.out.println("    " + n);
        }
        System.out.println();
        System.out.println("  The same enhanced for from module 5, over an ArrayList");
        System.out.println("  instead of an array -- it works on anything Java calls");
        System.out.println("  Iterable, which ArrayList is and a plain array also counts as.");

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.exercise(8);

        Ui.question(
                "Can an ArrayList grow after it is created, the way\n"
                        + "  a plain array cannot?  (answer: yes or no)",
                "yes",
                "add() grows an ArrayList one element at a time; an array's size is fixed forever.");
        Ui.question(
                "ArrayList<int> nums = new ArrayList<int>();\n"
                        + "  Does this compile?  (answer: yes or no)",
                "no",
                "A generic type parameter must be an object type -- ArrayList<Integer>, not ArrayList<int>.");

        Ui.challenge(
                List.of(
                        "Build an ArrayList<Integer> from 1 to 5 (add each with a",
                        "loop), then print the sum of its values."),
                List.of(),
                List.of("15"),
                List.of(
                        "ArrayList<Integer> nums = new ArrayList<>();",
                        "for (int i = 1; i <= 5; i++) {",
                        "    nums.add(i);",
                        "}",
                        "int sum = 0;",
                        "for (int n : nums) {",
                        "    sum += n;",
                        "}",
                        "System.out.println(sum);"));

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.heading("SUMMARY");

        System.out.println("   - ArrayList grows; add/get/size replace []/.length");
        System.out.println("   - a generic type parameter must be an object type -- Integer, not int");
        System.out.println("   - autoboxing/unboxing convert automatically; the type itself still must be boxed");
        Ui.waitEnter();
    }

    static void lesson09Classes() {
        Ui.title("MODULE 9 - CLASSES, INTERFACES, ENCAPSULATION");

        Ui.heading("PART 1: private is enforced -- a compile error, not a convention");

        System.out.println("    class Student {");
        System.out.println("        private int grade;");
        System.out.println("        Student(int grade) { this.grade = grade; }");
        System.out.println("        int getGrade() { return grade; }");
        System.out.println("    }");
        System.out.println();
        System.out.println("    Student s = new Student(15);");
        System.out.println("    s.grade = 20;         // does NOT compile");
        System.out.println();
        System.out.println("    Test.java:1: error: grade has private access in Student");
        System.out.println();
        System.out.println("  Python's leading underscore (self._grade) is a naming");
        System.out.println("  convention nobody enforces -- reading or writing it from");
        System.out.println("  outside the class just works. private here is a real access");
        System.out.println("  modifier: the compiler refuses code outside the class that");
        System.out.println("  touches the field directly, whatever it is trying to do with");
        System.out.println("  it. getGrade() below is the sanctioned way out:");

        Student a = new Student(15);
        System.out.println();
        System.out.println("  Running: s.getGrade() -> " + a.getGrade());

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.heading("PART 2: interfaces -- a contract, checked at compile time");

        System.out.println("    interface Shape {");
        System.out.println("        double area();");
        System.out.println("    }");
        System.out.println("    class Circle implements Shape {");
        System.out.println("        // area() left out entirely   // does NOT compile");
        System.out.println("    }");
        System.out.println();
        System.out.println("    Test.java:1: error: Circle is not abstract and does not");
        System.out.println("    override abstract method area() in Shape");
        System.out.println();
        System.out.println("  A class that implements an interface must provide every");
        System.out.println("  method that interface declares, or the compiler refuses it --");
        System.out.println("  there is no partial implementation the way Python's duck");
        System.out.println("  typing allows (call a method an object does not have, and");
        System.out.println("  Python only fails the moment that call actually runs).");
        System.out.println("  A real Circle, done properly:");
        System.out.println();

        Circle circle = new Circle(3.0);
        System.out.println("  Running: circle.area() -> " + circle.area());

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.heading("PART 3: garbage collected, and try-with-resources for deterministic cleanup");

        System.out.println("  No manual free like C, no destructor firing at a predictable");
        System.out.println("  point like C++'s RAII -- Java collects garbage on its own");
        System.out.println("  schedule, same as Python. For something that must close at a");
        System.out.println("  known moment -- a file, a connection -- Java's answer is");
        System.out.println("  try-with-resources, the same idea as Python's `with`:");
        System.out.println();
        System.out.println("    try (var writer = new java.io.FileWriter(\"out.txt\")) {");
        System.out.println("        writer.write(\"data\");");
        System.out.println("    }   // writer.close() runs here, guaranteed, even on an exception");
        System.out.println();
        System.out.println("  Any class implementing AutoCloseable can be used this way --");
        System.out.println("  the deterministic half of C++'s RAII, opted into per resource,");
        System.out.println("  exactly like Python's context managers rather than automatic");
        System.out.println("  for every object the way C++ makes it.");

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.exercise(9);

        Ui.question(
                "A field is declared private. Can code outside the\n"
                        + "  class read or write it directly?  (answer: yes or no)",
                "no",
                "private is a compiler-enforced access modifier here, not a naming convention like Python's _grade.");
        Ui.question(
                "A class implements an interface but leaves one of its\n"
                        + "  methods unwritten. Does this compile?  (answer: yes or no)",
                "no",
                "Every method an interface declares must be implemented, or the compiler refuses the class.");
        Ui.question(
                "Is a Java object destroyed at a specific, predictable\n"
                        + "  moment, the way a C++ destructor is?  (answer: yes or no)",
                "no",
                "Garbage collection timing is not guaranteed -- use try-with-resources for anything that must close on time.");

        Ui.challenge(
                List.of(
                        "Write a class Counter with a private int count starting",
                        "at 0, a method increment() that adds 1, and a method",
                        "value() that returns it. Create one, call increment()",
                        "three times, and print value()."),
                List.of(),
                List.of("3"),
                List.of(
                        "class Counter {",
                        "    private int count = 0;",
                        "    void increment() { count++; }",
                        "    int value() { return count; }",
                        "}",
                        "",
                        "Counter c = new Counter();",
                        "c.increment();",
                        "c.increment();",
                        "c.increment();",
                        "System.out.println(c.value());"));

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.heading("SUMMARY");

        System.out.println("   - private is enforced by the compiler -- not a convention like Python's _grade");
        System.out.println("   - implementing an interface means implementing every method it declares, or it will not compile");
        System.out.println("   - garbage collected like Python; try-with-resources gives C++'s RAII-style deterministic cleanup, opt-in");
        Ui.waitEnter();
    }

    private static class Student {
        private int grade;

        Student(int grade) {
            this.grade = grade;
        }

        int getGrade() {
            return grade;
        }
    }

    private interface Shape {
        double area();
    }

    private static class Circle implements Shape {
        private final double radius;

        Circle(double radius) {
            this.radius = radius;
        }

        @Override
        public double area() {
            return Math.PI * radius * radius;
        }
    }

    static void lesson10Exceptions() {
        Ui.title("MODULE 10 - EXCEPTIONS: CHECKED VS UNCHECKED");

        Ui.heading("PART 1: try / catch / finally");

        System.out.println("    static int divide(int a, int b) {");
        System.out.println("        if (b == 0) {");
        System.out.println("            throw new ArithmeticException(\"division by zero\");");
        System.out.println("        }");
        System.out.println("        return a / b;");
        System.out.println("    }");
        System.out.println();
        System.out.println("    try {");
        System.out.println("        System.out.println(divide(10, 2));");
        System.out.println("        System.out.println(divide(1, 0));");
        System.out.println("    } catch (ArithmeticException e) {");
        System.out.println("        System.out.println(\"caught: \" + e.getMessage());");
        System.out.println("    } finally {");
        System.out.println("        System.out.println(\"always runs\");");
        System.out.println("    }");
        System.out.println();
        System.out.println("  Running:");
        try {
            System.out.println("    " + divide(10, 2));
            System.out.println("    " + divide(1, 0));
        } catch (ArithmeticException e) {
            System.out.println("    caught: " + e.getMessage());
        } finally {
            System.out.println("    always runs");
        }
        System.out.println();
        System.out.println("  The second divide() never returns -- throw jumps straight to");
        System.out.println("  catch, so nothing after it in try runs. finally runs either");
        System.out.println("  way, same shape as every other course's try/catch/finally.");

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.heading("PART 2: checked exceptions -- enforced before the program ever runs");

        System.out.println("    static void risky() throws java.io.IOException {");
        System.out.println("        throw new java.io.IOException(\"disk on fire\");");
        System.out.println("    }");
        System.out.println();
        System.out.println("    public static void main(String[] args) {");
        System.out.println("        risky();          // does NOT compile");
        System.out.println("    }");
        System.out.println();
        System.out.println("    Test.java:1: error: unreported exception IOException;");
        System.out.println("    must be caught or declared to be thrown");
        System.out.println();
        System.out.println("  IOException is a CHECKED exception: throws IOException in");
        System.out.println("  risky()'s own signature is a promise the compiler enforces on");
        System.out.println("  every caller -- catch it, or declare throws IOException");
        System.out.println("  yourself and pass the obligation upward. C++, Python and");
        System.out.println("  JavaScript all let any exception fly straight past every");
        System.out.println("  caller with zero compile-time warning; Java refuses to build");
        System.out.println("  at all if a checked one is left unhandled anywhere on its way");
        System.out.println("  up. The opposite failure mode from C/C++'s if (x = 5) --");
        System.out.println("  there, real bugs pass silently; here, an unhandled path stops");
        System.out.println("  the build before it can ever run.");

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.heading("PART 3: unchecked exceptions -- no compile-time promise at all");

        System.out.println("    static int firstElement(int[] arr) {");
        System.out.println("        return arr[0];    // no throws clause anywhere");
        System.out.println("    }");
        System.out.println();
        System.out.println("  Running (called with an empty array):");
        try {
            firstElement(new int[0]);
        } catch (ArrayIndexOutOfBoundsException e) {
            System.out.println("    caught at runtime: " + e.getMessage());
        }
        System.out.println();
        System.out.println("  ArrayIndexOutOfBoundsException is RuntimeException, Java's");
        System.out.println("  unchecked family -- the compiler never requires a catch or a");
        System.out.println("  throws for it, exactly like every exception in C++, Python");
        System.out.println("  and JavaScript. Checked exceptions are for failures a caller");
        System.out.println("  can reasonably be expected to plan for (a missing file);");
        System.out.println("  unchecked ones are for programming mistakes an index out of");
        System.out.println("  range, a null reference used where an object was expected.");

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.exercise(10);

        Ui.question(
                "A method declares throws IOException. A caller does\n"
                        + "  not catch it and does not declare throws itself. Compile\n"
                        + "  error, or a runtime exception like Python?  (answer: error or runtime)",
                "error",
                "A checked exception left unhandled anywhere on its call path is a compile error, not a runtime surprise.");
        Ui.question(
                "ArrayIndexOutOfBoundsException is unchecked. Must a\n"
                        + "  caller declare throws for it?  (answer: yes or no)",
                "no",
                "RuntimeException and its subclasses are unchecked -- no compile-time obligation at all.");

        Ui.challenge(
                List.of(
                        "Write a method parseAge(String s) that returns an int by",
                        "calling Integer.parseInt(s), catching NumberFormatException",
                        "and returning -1 if it is not a valid number. Call it with",
                        "\"25\" and with \"abc\", printing both results."),
                List.of(),
                List.of("25", "-1"),
                List.of(
                        "static int parseAge(String s) {",
                        "    try {",
                        "        return Integer.parseInt(s);",
                        "    } catch (NumberFormatException e) {",
                        "        return -1;",
                        "    }",
                        "}",
                        "",
                        "System.out.println(parseAge(\"25\"));",
                        "System.out.println(parseAge(\"abc\"));"));

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.heading("SUMMARY");

        System.out.println("   - try/catch/finally: same shape as every other course here");
        System.out.println("   - checked exceptions must be caught or declared -- enforced at compile time");
        System.out.println("   - unchecked (RuntimeException) needs neither -- same as every exception in C++/Python/JS");
        System.out.println();
        System.out.println("  That is the core course. Garbage collected like Python and");
        System.out.println("  JavaScript, statically typed and compiled like C and C++, and");
        System.out.println("  the one course here where the compiler itself enforces both");
        System.out.println("  access control and exception handling before the program is");
        System.out.println("  ever allowed to run.");
        Ui.waitEnter();
    }

    private static int divide(int a, int b) {
        if (b == 0) {
            throw new ArithmeticException("division by zero");
        }
        return a / b;
    }

    private static int firstElement(int[] arr) {
        return arr[0];
    }
}
