// Ui.java - the pieces every lesson uses to draw the screen.
//
// Same purpose and the same visual style as the other courses' ui modules,
// kept as a separate implementation rather than shared code -- see
// docs/writing-a-course.md for why. Static methods on a final class rather
// than free functions, because that is what Java has; IN is a single
// Scanner opened once over System.in and reused everywhere, the same way
// the C++ course reuses a single std::cin.

import java.util.List;
import java.util.Scanner;

public final class Ui {
    private static final int WIDTH = 54;

    // Package-private, not private: lessons that need a raw read (Scanner's
    // nextInt()/nextLine() themselves are what module 3 teaches) share this
    // one Scanner rather than each opening their own over System.in, which
    // risks two Scanners racing over the same underlying stream.
    static final Scanner IN = new Scanner(System.in);

    private Ui() {
    }

    public static void clearScreen() {
        // ANSI codes, not ProcessBuilder("clear").inheritIO().start(): no
        // dependency on TERM being set. Same fix, same reason, as every
        // other course's ui module.
        System.out.print("\033[H\033[2J\033[3J");
        System.out.flush();
    }

    public static void waitEnter() {
        System.out.print("\n  Press ENTER to continue...");
        System.out.flush();
        IN.nextLine();
    }

    public static void rule() {
        System.out.println("  " + "-".repeat(WIDTH));
    }

    private static void paddedLine(String text, char border) {
        String padded = text.length() < WIDTH - 3
                ? text + " ".repeat(WIDTH - 3 - text.length())
                : text;
        System.out.println("  " + border + " " + padded + border);
    }

    private static void frame(char fill) {
        System.out.println("  +" + String.valueOf(fill).repeat(WIDTH - 2) + "+");
    }

    public static void title(String text) {
        System.out.println();
        frame('=');
        paddedLine(text, '|');
        frame('=');
        System.out.println();
    }

    public static void heading(String text) {
        System.out.println("\n  " + text);
        rule();
    }

    public static boolean askYes(String questionText) {
        System.out.print("\n  " + questionText + " (y/N): ");
        System.out.flush();
        String answer = IN.nextLine();
        return !answer.isEmpty() && (answer.charAt(0) == 'y' || answer.charAt(0) == 'Y');
    }

    public static void exercise(int number) {
        System.out.println("\n  >> EXERCISE - MODULE " + number);
        rule();
    }

    public static boolean question(String text, String correct, String why) {
        System.out.println("\n  " + text);
        System.out.print("  Your answer: ");
        System.out.flush();
        String answer = IN.nextLine();

        boolean right = answer.strip().equalsIgnoreCase(correct.strip());
        if (right) {
            System.out.println("\n  CORRECT.  " + why);
        } else {
            System.out.println("\n  NOT QUITE. The answer is: " + correct);
            System.out.println("             " + why);
        }
        return right;
    }

    // A task to write in a real file.
    //
    // `input` is every line the program will read while producing
    // `expected` -- empty for a task that reads nothing. Together they are
    // the actual specification: run the solution, type `input`, get
    // `expected`, however the code that does it is written. `solution` is
    // one way of getting there and appears only after a confirmation.
    public static void challenge(List<String> task, List<String> input,
            List<String> expected, List<String> solution) {
        System.out.println("\n  >> WRITE THIS YOURSELF, in a real file");
        rule();

        for (String line : task) {
            System.out.println("  " + line);
        }

        if (!input.isEmpty()) {
            System.out.println("\n  Try it with this input:\n");
            for (String line : input) {
                System.out.println("      " + line);
            }
        }

        if (!expected.isEmpty()) {
            System.out.println("\n  It must print:\n");
            for (String line : expected) {
                System.out.println("      " + line);
            }
            System.out.println("\n  That output is the whole specification. Any code that");
            System.out.println("  produces it is correct.");
        }

        System.out.println("\n  Try it first. Compile with:");
        System.out.println("    javac Test.java && java Test");

        if (!askYes("Want to see example code?")) {
            return;
        }

        System.out.println();
        rule();
        for (String line : solution) {
            System.out.println("  " + line);
        }
        rule();
        System.out.println("  This is EXAMPLE CODE, not the answer. It is one way to get");
        System.out.println("  that output; yours may look nothing like it and still be");
        System.out.println("  right -- or better. Compare the output, not the code.");
    }
}
