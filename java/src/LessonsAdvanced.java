// LessonsAdvanced.java - module 11 onward.
//
// Module 8 already used ArrayList, one implementation of the List
// interface. Module 11 is the rest of the Collections Framework: Map and
// Set, and the trap each language in this repo hides somewhere different
// -- Java's own HashMap.get() returns null on a missing key rather than
// throwing or inserting, which is its own thing to get wrong.
//
// Module 12 is generics: the duplication and unsafe-cast pain of a
// container typed as plain Object, then the fix -- a generic class, a
// generic method, and a bounded type parameter -- and the one trap that
// is Java's own rather than a repeat of an earlier module's: type
// erasure, verified at runtime rather than just asserted.
//
// Module 13 is concurrency: Thread and Runnable, a real race condition --
// two threads incrementing a shared plain int with no protection, run
// live and shown to print a wrong, different-every-time total -- the fix
// (synchronized), and join(), without which a result read after start()
// may not be the finished one yet. Every Runnable here is an anonymous
// class, not a lambda: this course never introduces lambda syntax (see
// tools/check-teaching-order.py's own "never taught" list), and this is
// the first module where Runnable makes that restriction visible.

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

class LessonsAdvanced {

    static void lesson11Collections() {
        Ui.title("MODULE 11 - THE COLLECTIONS FRAMEWORK");

        Ui.heading("PART 1: List, Map, and Set are interfaces");

        System.out.println("  Module 8's ArrayList is one implementation of the List");
        System.out.println("  interface -- code written against List works with any of");
        System.out.println("  them:");
        System.out.println();
        System.out.println("    List<String> names = new ArrayList<>();");
        System.out.println("    names.add(\"Ana\");");
        System.out.println();

        List<String> names = new ArrayList<>();
        names.add("Ana");
        System.out.println("  Running: " + names);
        System.out.println();
        System.out.println("  Declaring the variable as List<String>, not");
        System.out.println("  ArrayList<String>, means the rest of the method never");
        System.out.println("  depends on which List you picked -- only `new ArrayList<>()`");
        System.out.println("  would need to change to swap it for a different one.");

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.heading("PART 2: Map -- key to value, and a missing key is null");

        System.out.println("  A Map holds values found by key instead of by index:");
        System.out.println();
        System.out.println("    Map<String, Integer> ages = new HashMap<>();");
        System.out.println("    ages.put(\"Ana\", 30);");
        System.out.println();

        Map<String, Integer> ages = new HashMap<>();
        ages.put("Ana", 30);
        System.out.println("  Running: ages.get(\"Ana\") is " + ages.get("Ana"));
        System.out.println();
        System.out.println("  A key that was never put is not an exception, and it is");
        System.out.println("  not inserted either -- .get() just returns null:");
        System.out.println();
        System.out.println("    System.out.println(ages.get(\"Bea\"));");
        System.out.println();

        System.out.println("  Running: " + ages.get("Bea"));
        System.out.println();
        System.out.println("  null silently propagating is its own trap: adding 1 to a");
        System.out.println("  null Integer throws NullPointerException at the point you");
        System.out.println("  use it, not at the .get() that actually produced it. The");
        System.out.println("  safe form gives a real default instead of null:");
        System.out.println();
        System.out.println("    int age = ages.getOrDefault(\"Bea\", 0);");
        System.out.println();

        int age = ages.getOrDefault("Bea", 0);
        System.out.println("  Running: " + age);

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.heading("PART 3: Set -- unique values, membership without an index");

        System.out.println("  A Set holds each value at most once. Adding a value already");
        System.out.println("  present changes nothing and does not throw:");
        System.out.println();
        System.out.println("    Set<Integer> seen = new HashSet<>();");
        System.out.println("    seen.add(5);");
        System.out.println("    seen.add(5);");
        System.out.println();

        Set<Integer> seen = new HashSet<>();
        seen.add(5);
        boolean addedAgain = seen.add(5);
        System.out.println("  Running: size is " + seen.size() + "; the second add() returned "
                + addedAgain + " (false means it was already there).");
        System.out.println();
        System.out.println("  .contains() is how you check membership:");
        System.out.println();
        System.out.println("    if (seen.contains(5)) { ... }");

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.exercise(11);

        Ui.question(
                "Map<String, Integer> m = new HashMap<>(); m.get(\"x\") was\n"
                        + "  never put. Does this throw, or return null?",
                "return null",
                "get() on a missing key returns null -- no exception, and nothing is inserted, "
                        + "unlike C++'s std::map operator[].");

        Ui.question(
                "Set<Integer> s = new HashSet<>(); s.add(7); s.add(7);\n"
                        + "  What does the second add() call return?  (answer: true or false)",
                "false",
                "add() returns whether the set actually changed -- false means the value was "
                        + "already present.");

        Ui.question(
                "ages.getOrDefault(\"Bea\", 0) is called on a Map with no\n"
                        + "  \"Bea\" key. What does it return?",
                "0",
                "getOrDefault gives the fallback you name instead of null, closing the "
                        + "NullPointerException-later trap .get() alone leaves open.");

        Ui.challenge(
                List.of(
                        "Read words until \"stop\". Using a HashSet<String>,",
                        "print how many DISTINCT words were entered (duplicates",
                        "should not be counted twice)."),
                List.of("cat", "dog", "cat", "stop"),
                List.of("2"),
                List.of(
                        "import java.util.HashSet;",
                        "import java.util.Scanner;",
                        "",
                        "public class Main {",
                        "    public static void main(String[] args) {",
                        "        Scanner sc = new Scanner(System.in);",
                        "        HashSet<String> words = new HashSet<>();",
                        "        String w;",
                        "        while (!(w = sc.next()).equals(\"stop\")) {",
                        "            words.add(w);",
                        "        }",
                        "        System.out.println(words.size());",
                        "    }",
                        "}"));

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.heading("SUMMARY");

        System.out.println("   - List, Map, and Set are interfaces; ArrayList/HashMap/HashSet");
        System.out.println("     are implementations of them");
        System.out.println("   - Map.get() on a missing key returns null, not an exception");
        System.out.println("     and not an inserted default -- getOrDefault() avoids the");
        System.out.println("     NullPointerException that null causes later");
        System.out.println("   - Set.add() returns false, not an exception, when the value");
        System.out.println("     was already present");
        System.out.println();
        System.out.println("  Module 12 is generics: the mechanism that lets List<String>");
        System.out.println("  and List<Integer> both exist from one class, ArrayList's own,");
        System.out.println("  not one written per type.");
        Ui.waitEnter();
    }

    static void lesson12Generics() {
        Ui.title("MODULE 12 - GENERICS");

        Ui.heading("PART 1: the problem -- a container holding plain Object");

        System.out.println("  Before generics, a reusable container had one honest option: hold");
        System.out.println("  everything as Object, since there was no syntax for \"a box of");
        System.out.println("  whatever type you give it\":");
        System.out.println();
        System.out.println("    class ObjectBox {");
        System.out.println("        private Object value;");
        System.out.println("        void set(Object value) { this.value = value; }");
        System.out.println("        Object get() { return value; }");
        System.out.println("    }");
        System.out.println();

        ObjectBox box = new ObjectBox();
        box.set("hello");
        String s = (String) box.get();
        System.out.println("    ObjectBox box = new ObjectBox();");
        System.out.println("    box.set(\"hello\");");
        System.out.println("    String s = (String) box.get();   // a cast, on every caller");
        System.out.println();
        System.out.println("  Running: s -> " + s);
        System.out.println();
        System.out.println("  get() returns Object, so getting the String back out needs a cast");
        System.out.println("  the compiler cannot check -- it trusts the cast is correct. Naming");
        System.out.println("  the wrong type compiles fine and fails only once the line runs:");
        System.out.println();
        System.out.println("    Integer n = (Integer) box.get();   // box actually holds a String");
        System.out.println();

        try {
            Integer n = (Integer) box.get();
            System.out.println("  Running: " + n);
        } catch (ClassCastException e) {
            System.out.println("  Running: caught ClassCastException -- " + e.getMessage());
        }
        System.out.println();
        System.out.println("  Every caller of a class like this repeats the same cast, and the");
        System.out.println("  compiler has no way to catch the wrong one ahead of time. Generics");
        System.out.println("  give that type information back to the compiler, so a mismatch is");
        System.out.println("  caught at compile time instead of becoming the runtime error above.");

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.heading("PART 2: a generic class -- Box<T>, no casts needed");

        System.out.println("    class Box<T> {");
        System.out.println("        private T value;");
        System.out.println("        Box(T value) { this.value = value; }");
        System.out.println("        T get() { return value; }");
        System.out.println("    }");
        System.out.println();
        System.out.println("    Box<String> stringBox = new Box<>(\"hello\");");
        System.out.println("    Box<Integer> intBox = new Box<>(42);");
        System.out.println();

        Box<String> stringBox = new Box<>("hello");
        Box<Integer> intBox = new Box<>(42);
        System.out.println("  Running: stringBox.get() -> " + stringBox.get()
                + ", intBox.get() -> " + intBox.get());
        System.out.println();
        System.out.println("  T is a placeholder, filled in per instance -- stringBox.get()");
        System.out.println("  already returns String, no cast, and the compiler refuses a mismatch");
        System.out.println("  outright:");
        System.out.println();
        System.out.println("    Box<Integer> bad = new Box<Integer>(\"hello\");   // does NOT compile");
        System.out.println();
        System.out.println("    Test.java:1: error: incompatible types: String cannot be converted to Integer");
        System.out.println();
        System.out.println("  One class, the same idea ArrayList itself uses, produces both a");
        System.out.println("  Box<String> and a Box<Integer> -- not two hand-written classes.");

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.heading("PART 3: a generic method -- one method, any List<T>");

        System.out.println("    static <T> T firstOf(List<T> list) {");
        System.out.println("        return list.get(0);");
        System.out.println("    }");
        System.out.println();

        List<String> names = new ArrayList<>();
        names.add("Ana");
        names.add("Bea");
        List<Integer> nums = new ArrayList<>();
        nums.add(10);
        nums.add(20);
        System.out.println("    List<String> names = ...;    // \"Ana\", \"Bea\"");
        System.out.println("    List<Integer> nums = ...;    // 10, 20");
        System.out.println();
        System.out.println("  Running: firstOf(names) -> " + firstOf(names)
                + ", firstOf(nums) -> " + firstOf(nums));
        System.out.println();
        System.out.println("  <T> before the return type declares the method's own type parameter,");
        System.out.println("  separate from any class it lives in -- one firstOf works on a");
        System.out.println("  List<String> and a List<Integer> alike, with T filled in per call");
        System.out.println("  from the argument, not written out by the caller.");

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.heading("PART 4: a bounded type parameter -- T must be Comparable");

        System.out.println("  An unbounded <T> is only known to be an Object -- calling .compareTo()");
        System.out.println("  on it does not compile, since Object declares no such method. Bounding");
        System.out.println("  T to types that implement Comparable makes the call legal:");
        System.out.println();
        System.out.println("    static <T extends Comparable<T>> T max(T a, T b) {");
        System.out.println("        return a.compareTo(b) >= 0 ? a : b;");
        System.out.println("    }");
        System.out.println();
        System.out.println("  Running: max(3, 7) -> " + max(3, 7)
                + ", max(\"pear\", \"apple\") -> " + max("pear", "apple"));
        System.out.println();
        System.out.println("  Integer and String both implement Comparable, so the same max()");
        System.out.println("  works on either -- the bound is what lets the compiler trust that");
        System.out.println("  .compareTo() exists on whatever T turns out to be.");

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.heading("PART 5: type erasure -- generics vanish by runtime");

        System.out.println("    boolean sameClass = new ArrayList<String>().getClass()");
        System.out.println("            == new ArrayList<Integer>().getClass();");
        System.out.println();

        boolean sameClass = new ArrayList<String>().getClass() == new ArrayList<Integer>().getClass();
        System.out.println("  Running: sameClass -> " + sameClass);
        System.out.println();
        System.out.println("  Generic type parameters are a compile-time-only check -- once the");
        System.out.println("  compiler has verified every use, <String> and <Integer> are erased,");
        System.out.println("  and both variables run as the exact same ArrayList class, with no");
        System.out.println("  memory left of which one it was declared with. C++'s templates take");
        System.out.println("  the opposite approach, compiling a separate version of the code per");
        System.out.println("  type instead of erasing the type the way Java does.");

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.exercise(12);

        Ui.question(
                "Box<Object> holds any object. Can it be assigned to a\n"
                        + "  variable of type Box<String>?  (answer: yes or no)",
                "no",
                "Generic types are not related just because their type arguments are -- every String "
                        + "is an Object, but Box<Object> is not a Box<String>.");

        Ui.question(
                "At runtime, is new ArrayList<String>().getClass() the\n"
                        + "  same Class object as new ArrayList<Integer>().getClass()?  (answer: yes or no)",
                "yes",
                "Type erasure removes generic type parameters after compilation -- every ArrayList "
                        + "of any type argument runs as the same plain ArrayList class.");

        Ui.question(
                "A generic method declares an unbounded <T> parameter.\n"
                        + "  Can the method body call .compareTo() on a value of type T?  (answer: yes or no)",
                "no",
                "An unbounded T is only guaranteed to be an Object -- the type parameter needs a "
                        + "bound requiring Comparable before .compareTo() is legal to call.");

        Ui.challenge(
                List.of(
                        "Write a generic class Pair<A, B> holding two fields, first",
                        "and second, with a constructor and getters getFirst()/",
                        "getSecond(). Create a Pair<String, Integer> holding",
                        "(\"Ana\", 30) and print both values."),
                List.of(),
                List.of("Ana", "30"),
                List.of(
                        "class Pair<A, B> {",
                        "    private final A first;",
                        "    private final B second;",
                        "",
                        "    Pair(A first, B second) {",
                        "        this.first = first;",
                        "        this.second = second;",
                        "    }",
                        "",
                        "    A getFirst() { return first; }",
                        "    B getSecond() { return second; }",
                        "}",
                        "",
                        "Pair<String, Integer> p = new Pair<>(\"Ana\", 30);",
                        "System.out.println(p.getFirst());",
                        "System.out.println(p.getSecond());"));

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.heading("SUMMARY");

        System.out.println("   - a container typed as Object needs a cast on every read, and the wrong");
        System.out.println("     cast is a runtime ClassCastException, not a compile error");
        System.out.println("   - a generic class (Box<T>) or method (<T> firstOf(List<T>)) gives the");
        System.out.println("     compiler that type information back -- wrong types are caught before");
        System.out.println("     the program ever runs, instead of three calls deep at runtime");
        System.out.println("   - a bounded type parameter (<T extends Comparable<T>>) is how a generic");
        System.out.println("     method promises the compiler that T supports a specific method");
        System.out.println("   - type erasure means generics are compile-time only -- ArrayList<String>");
        System.out.println("     and ArrayList<Integer> are the identical class at runtime");
        Ui.waitEnter();
    }

    static void lesson13Concurrency() {
        Ui.title("MODULE 13 - CONCURRENCY");

        Ui.heading("PART 1: Thread and Runnable -- .start() runs concurrently, .run() does not");

        System.out.println("  A Runnable is a task: an object with one method, run(), taking no");
        System.out.println("  arguments and returning nothing. Wrapping one in a Thread and");
        System.out.println("  calling .start() runs it on a NEW thread, at the same time as");
        System.out.println("  whatever called .start(). Calling .run() directly does nothing of");
        System.out.println("  the sort -- it is an ordinary method call, on whichever thread makes it:");
        System.out.println();
        System.out.println("    Runnable task = new Runnable() {");
        System.out.println("        public void run() {");
        System.out.println("            System.out.println(Thread.currentThread().getName());");
        System.out.println("        }");
        System.out.println("    };");
        System.out.println("    task.run();               // an ordinary call, on THIS thread");
        System.out.println("    new Thread(task).start(); // runs on a NEW thread");
        System.out.println();

        Runnable task = new Runnable() {
            public void run() {
                System.out.println("  Running: " + Thread.currentThread().getName());
            }
        };

        System.out.println("  Running: main is on thread " + Thread.currentThread().getName());
        System.out.println("  Calling task.run() directly:");
        task.run();
        System.out.println("  Calling new Thread(task).start():");
        Thread greeter = new Thread(task);
        greeter.start();
        join(greeter);
        System.out.println();
        System.out.println("  task.run() printed the SAME thread name as main -- no new thread,");
        System.out.println("  just a method call. new Thread(task).start() printed a DIFFERENT");
        System.out.println("  name -- a second thread actually ran that line, concurrently with");
        System.out.println("  everything else main was doing.");

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.heading("PART 2: a real race -- two threads, one plain int, no protection");

        System.out.println("  count++ looks like one step. It is actually three: read count,");
        System.out.println("  add 1, write the result back. Two threads sharing a plain int");
        System.out.println("  field with no protection can each read the SAME value before");
        System.out.println("  either one writes its +1 back -- one of the two increments is");
        System.out.println("  then simply lost.");
        System.out.println();
        System.out.println("  Module 9 had you write a Counter with increment() and value().");
        System.out.println("  Here is that same shape, handed to two threads at once:");
        System.out.println();
        System.out.println("    class SharedCounter {");
        System.out.println("        private int count = 0;");
        System.out.println("        void incrementUnsafe() { count++; }");
        System.out.println("        int value() { return count; }");
        System.out.println("    }");
        System.out.println();
        System.out.println("    SharedCounter counter = new SharedCounter();");
        System.out.println("    Runnable work = new Runnable() {");
        System.out.println("        public void run() {");
        System.out.println("            for (int i = 0; i < 100_000; i++) {");
        System.out.println("                counter.incrementUnsafe();");
        System.out.println("            }");
        System.out.println("        }");
        System.out.println("    };");
        System.out.println("    Thread a = new Thread(work);");
        System.out.println("    Thread b = new Thread(work);");
        System.out.println("    a.start();");
        System.out.println("    b.start();");
        System.out.println("    a.join();");
        System.out.println("    b.join();");
        System.out.println();

        final int n = 100_000;
        final SharedCounter unsafeCounter = new SharedCounter();
        Runnable unsafeWork = new Runnable() {
            public void run() {
                for (int i = 0; i < n; i++) {
                    unsafeCounter.incrementUnsafe();
                }
            }
        };
        runTwoThreads(unsafeWork);

        final int unsafeTotal = unsafeCounter.value();
        System.out.println("  Running: expected " + (2 * n) + " (100,000 + 100,000), got "
                + unsafeTotal);
        System.out.println();
        if (unsafeTotal < 2 * n) {
            System.out.println("  Whatever number just printed, it is less than " + (2 * n) + ", and");
            System.out.println("  running this module again will print a DIFFERENT wrong number --");
            System.out.println("  which increments get lost depends on how the OS happens to");
            System.out.println("  schedule the two threads, which a program does not control.");
        } else {
            System.out.println("  This run got lucky: " + (2 * n) + " is the mathematically correct");
            System.out.println("  total, and this time no increment was lost. That CAN happen --");
            System.out.println("  the race is about timing, not guaranteed corruption, and it shows");
            System.out.println("  up less often on a single-core machine, where the two threads have");
            System.out.println("  fewer chances to actually interleave mid-increment. It does not");
            System.out.println("  mean incrementUnsafe() is safe: run it enough times, or on more");
            System.out.println("  cores, and a lost update will show up. The fix in PART 3 is still");
            System.out.println("  the correct takeaway regardless of what this one run showed.");
        }
        System.out.println();
        System.out.println("  This is safe to run live, which a double free or a buffer overrun");
        System.out.println("  in C never would be: a plain int in Java never TEARS -- a read");
        System.out.println("  always sees some value one of the threads actually wrote, never");
        System.out.println("  garbage or a half-written value. There is no memory corruption and");
        System.out.println("  no crash here. But lost updates are still a real, contest-worthy");
        System.out.println("  bug -- a DIFFERENT CLASS of bug than C's undefined behavior, not a");
        System.out.println("  safer version of it.");

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.heading("PART 3: the fix -- synchronized");

        System.out.println("  synchronized on a method lets only one thread execute it at a time");
        System.out.println("  ON THE SAME OBJECT -- a second thread calling it blocks until the");
        System.out.println("  first one returns. That closes the exact gap PART 2 fell through:");
        System.out.println("  the read-add-write inside incrementUnsafe() can no longer be split");
        System.out.println("  apart by another thread's read landing in the middle of it.");
        System.out.println();
        System.out.println("    synchronized void incrementSafe() { count++; }");
        System.out.println();

        final SharedCounter safeCounter = new SharedCounter();
        Runnable safeWork = new Runnable() {
            public void run() {
                for (int i = 0; i < n; i++) {
                    safeCounter.incrementSafe();
                }
            }
        };
        runTwoThreads(safeWork);

        System.out.println("  Running: expected " + (2 * n) + ", got " + safeCounter.value());
        System.out.println();
        System.out.println("  Run this module again and it will still be exactly " + (2 * n)
                + ". Unlike");
        System.out.println("  PART 2, synchronized makes the correct total the ONLY possible");
        System.out.println("  result, not just the likely one -- there is no race left to lose.");

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.heading("PART 4: join() -- what makes reading the result mean anything");

        System.out.println("  start() returns immediately. It does not wait for the new thread");
        System.out.println("  to finish, or even to begin -- the calling thread is free to keep");
        System.out.println("  going on its own. Reading a shared result right after start(),");
        System.out.println("  before any join(), reads whatever happens to be there at that");
        System.out.println("  instant, which is usually not the finished answer:");
        System.out.println();
        System.out.println("    Holder result = new Holder();          // holds one int field");
        System.out.println("    Thread worker = new Thread(new Runnable() {");
        System.out.println("        public void run() {");
        System.out.println("            try {");
        System.out.println("                Thread.sleep(50);");
        System.out.println("            } catch (InterruptedException e) {");
        System.out.println("                Thread.currentThread().interrupt();");
        System.out.println("            }");
        System.out.println("            result.value = 42;");
        System.out.println("        }");
        System.out.println("    });");
        System.out.println("    worker.start();");
        System.out.println("    System.out.println(result.value);   // read BEFORE join()");
        System.out.println("    worker.join();");
        System.out.println("    System.out.println(result.value);   // read AFTER join()");
        System.out.println();

        final Holder result = new Holder();
        Thread worker = new Thread(new Runnable() {
            public void run() {
                try {
                    Thread.sleep(50);
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
                result.value = 42;
            }
        });
        worker.start();
        System.out.println("  Running: right after start(), before join() -> result.value is "
                + result.value);
        join(worker);
        System.out.println("  Running: after join() -> result.value is " + result.value);
        System.out.println();
        System.out.println("  Before join(), the worker thread has barely begun -- result.value");
        System.out.println("  is still its starting value, not the 42 the thread eventually sets");
        System.out.println("  50 milliseconds later. join() blocks the CALLING thread until the");
        System.out.println("  target thread finishes; that is what makes reading its result");
        System.out.println("  afterward mean anything at all, whether the result is a plain int");
        System.out.println("  here or the total from PARTS 2 and 3 above.");

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.exercise(13);

        Ui.question(
                "A Runnable named task exists. Which call actually runs it\n"
                        + "  on a NEW thread: task.run(), or new Thread(task).start()?\n"
                        + "  (answer: run or start)",
                "start",
                "run() is an ordinary method call on the calling thread; start() is what "
                        + "actually creates and schedules a new thread to run it.");

        Ui.question(
                "Two threads both call the same synchronized method on the\n"
                        + "  same object. Does synchronized guarantee which one runs FIRST?\n"
                        + "  (answer: yes or no)",
                "no",
                "synchronized only guarantees mutual exclusion -- one thread at a time in the "
                        + "critical section -- never which of the two goes first.");

        Ui.question(
                "A thread calls worker.join(). Does the CALLING thread wait\n"
                        + "  for worker to finish before continuing past that line?\n"
                        + "  (answer: yes or no)",
                "yes",
                "join() blocks the caller until the target thread finishes -- without it, code "
                        + "after start() can run before the new thread has done anything.");

        Ui.heading("TASK: WRITE THE SYNCHRONIZED COUNTER YOURSELF");

        System.out.println("  Write a program with two threads that each increment a shared");
        System.out.println("  counter 100,000 times, join both, then print the total.");
        System.out.println();
        System.out.println("  Unlike every challenge() so far, this task has no single");
        System.out.println("  transcript to match against -- PART 2 already showed why: the");
        System.out.println("  broken version's own number is different every time it runs. What");
        System.out.println("  this task actually asks for is RELIABILITY, not one matching line:");
        System.out.println("  use synchronized so the total is 200000 every single run, not just");
        System.out.println("  this one.");
        System.out.println();
        System.out.println("  Example run:");
        Ui.rule();
        System.out.println("  Total: 200000");
        Ui.rule();
        System.out.println();
        System.out.println("  Run the compiled program at least five times in a row. If any run");
        System.out.println("  prints anything other than 200000, synchronized is missing from");
        System.out.println("  wherever the increment actually happens.");
        System.out.println();
        System.out.println("  Try it first. Compile with:");
        System.out.println("    javac Test.java && java Test");

        if (Ui.askYes("Want to see example code?")) {
            System.out.println();
            Ui.rule();
            System.out.println("  class Test {");
            System.out.println("      static int count = 0;");
            System.out.println();
            System.out.println("      static synchronized void increment() {");
            System.out.println("          count++;");
            System.out.println("      }");
            System.out.println();
            System.out.println("      public static void main(String[] args) throws InterruptedException {");
            System.out.println("          Runnable work = new Runnable() {");
            System.out.println("              public void run() {");
            System.out.println("                  for (int i = 0; i < 100_000; i++) {");
            System.out.println("                      increment();");
            System.out.println("                  }");
            System.out.println("              }");
            System.out.println("          };");
            System.out.println("          Thread a = new Thread(work);");
            System.out.println("          Thread b = new Thread(work);");
            System.out.println("          a.start();");
            System.out.println("          b.start();");
            System.out.println("          a.join();");
            System.out.println("          b.join();");
            System.out.println("          System.out.println(\"Total: \" + count);");
            System.out.println("      }");
            System.out.println("  }");
            Ui.rule();
            System.out.println("  static synchronized locks the Class object itself (Test.class),");
            System.out.println("  not an instance -- there is only one of those, shared by every");
            System.out.println("  call to increment() no matter which object, if any, made it. That");
            System.out.println("  is still the exact \"same object\" exclusion PART 3 described, just");
            System.out.println("  with the class itself standing in as that shared object.");
            System.out.println();
            System.out.println("  This is EXAMPLE CODE, not the answer. It is one way to get that");
            System.out.println("  output; yours may look nothing like it and still be right -- or");
            System.out.println("  better. Compare the output, not the code.");
        }

        Ui.waitEnter();
        Ui.clearScreen();
        Ui.heading("SUMMARY");

        System.out.println("   - new Thread(runnable).start() runs concurrently, on a new thread;");
        System.out.println("     calling .run() directly is an ordinary call on the current one");
        System.out.println("   - two threads sharing a plain field with no protection can each");
        System.out.println("     read the same value before either writes back -- a lost update,");
        System.out.println("     the textbook shape of a data race");
        System.out.println("   - synchronized fixes it by letting only one thread execute the");
        System.out.println("     critical section at a time -- it guarantees mutual exclusion,");
        System.out.println("     not which thread goes first");
        System.out.println("   - a Java data race on a plain field is not undefined behavior the");
        System.out.println("     way it would be in C -- no corruption, no crash -- but it is a");
        System.out.println("     real bug: lost updates and stale reads, a different class of bug,");
        System.out.println("     not a safe one");
        System.out.println("   - join() blocks the calling thread until the target finishes;");
        System.out.println("     without it, a result read after start() may not be finished yet");
        Ui.waitEnter();
    }

    private static class ObjectBox {
        private Object value;

        void set(Object value) {
            this.value = value;
        }

        Object get() {
            return value;
        }
    }

    private static class Box<T> {
        private final T value;

        Box(T value) {
            this.value = value;
        }

        T get() {
            return value;
        }
    }

    private static <T> T firstOf(List<T> list) {
        return list.get(0);
    }

    private static <T extends Comparable<T>> T max(T a, T b) {
        return a.compareTo(b) >= 0 ? a : b;
    }

    // Shared by both the broken and the fixed demo in module 13 -- only
    // which increment method gets called differs between PART 2 and PART 3.
    private static class SharedCounter {
        private int count = 0;

        void incrementUnsafe() {
            count++;
        }

        synchronized void incrementSafe() {
            count++;
        }

        int value() {
            return count;
        }
    }

    // One mutable int, for PART 4's join() demo -- a local variable cannot
    // be reassigned from inside an anonymous Runnable, so the field this
    // wraps stands in for whatever real result a background thread computes.
    private static class Holder {
        int value = 0;
    }

    private static void runTwoThreads(Runnable work) {
        Thread a = new Thread(work);
        Thread b = new Thread(work);
        a.start();
        b.start();
        join(a);
        join(b);
    }

    // join() itself is checked (InterruptedException), but every lesson
    // method here is a Runnable (see Main's MODULES table) and cannot
    // declare a throws clause -- this is where that gets caught instead.
    private static void join(Thread t) {
        try {
            t.join();
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            throw new RuntimeException("interrupted while waiting for a demo thread to finish", e);
        }
    }
}
