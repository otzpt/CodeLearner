// Java course - main menu.
//
// Written in Java, the same way every other course here is written in the
// language it teaches. MODULES is an array of a small record type holding a
// title and a Runnable -- Java's functional-interface answer to the
// function-pointer table the C course uses and the std::function table the
// C++ course uses for the same job.

import java.util.Scanner;

public class Main {

    private record Module(String title, Runnable action) {
    }

    private static final Module[] MODULES = {
            new Module("Compiling and printing", LessonsBasics::lesson01Running),
            new Module("Variables and types", LessonsBasics::lesson02Variables),
            new Module("Reading input", LessonsBasics::lesson03Input),
            new Module("Conditions", LessonsBasics::lesson04Conditions),
            new Module("Loops", LessonsBasics::lesson05Loops),
            new Module("Arrays and Strings", LessonsMore::lesson06ArraysStrings),
            new Module("Methods and overloading", LessonsMore::lesson07Methods),
            new Module("ArrayList and boxing", LessonsMore::lesson08ArrayList),
            new Module("Classes, interfaces, and real encapsulation", LessonsMore::lesson09Classes),
            new Module("Exceptions: checked vs unchecked", LessonsMore::lesson10Exceptions),
    };

    private static void showMenu() {
        Ui.clearScreen();
        Ui.title("JAVA COURSE - FROM CLASS Main TO CHECKED EXCEPTIONS");

        for (int i = 0; i < MODULES.length; i++) {
            System.out.printf("   [%2d]  %s%n", i + 1, MODULES[i].title());
        }
        System.out.println("\n   [ 0]  Quit");
        Ui.rule();
    }

    public static void main(String[] args) {
        Scanner sc = Ui.IN;

        while (true) {
            showMenu();
            System.out.print("\n  Pick a module: ");
            System.out.flush();

            if (!sc.hasNextLine()) {
                break;
            }
            String choice = sc.nextLine().strip();

            if (choice.equals("0")) {
                break;
            }

            int n;
            try {
                n = Integer.parseInt(choice);
            } catch (NumberFormatException e) {
                n = 0;
            }

            if (n >= 1 && n <= MODULES.length) {
                Ui.clearScreen();
                // A module that lets an exception escape should not end the
                // session with a raw stack trace -- every other exit path
                // here prints something and returns to this menu.
                try {
                    MODULES[n - 1].action().run();
                } catch (RuntimeException e) {
                    System.out.println("\n  Something went wrong: " + e.getMessage());
                    Ui.waitEnter();
                }
            } else {
                System.out.println("\n  Not a valid option.");
                Ui.waitEnter();
            }
        }

        System.out.println("\n  See you next time.\n");
    }
}
