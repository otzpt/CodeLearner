/*
 * Module 1 - GTK: your first window.
 *
 * This course, unlike c/, cpp/, python/ and javascript/, is not a
 * zero-dependency single binary all the way down: the lesson program
 * itself (this file, compiled into gui-course) needs nothing beyond a C
 * compiler, but the code it teaches needs GTK4's development files
 * installed to actually compile and run.
 *
 * Every example in this module is a file that was compiled and run, not
 * typed from memory:
 *
 *   gcc -std=c11 -Wall -Wextra $(pkg-config --cflags gtk4) x.c -o x $(pkg-config --libs gtk4)
 *
 * gcc 14.2.1, GTK 4.22.5, no warnings. Each one was also driven by a test
 * program that read the window's title back with gtk_window_get_title(),
 * checked it was visible, emitted "clicked" on the button and read its
 * label before and after (unchanged in button.c, "Clicked!" in click.c),
 * and checked the exit status was 0. The two run-time messages quoted in
 * the lesson (the invalid cast, the missing GTK package) are real output
 * from this machine. An earlier version of this module, with a different
 * window title, was also confirmed against the window manager's client
 * list; that was not repeated for this rewrite.
 *
 * The comments inside the examples are the lesson: every line says what it
 * does. They are printed exactly as they appear in the compiled files.
 */

#include <stdio.h>

#include "lessons.h"
#include "strings_pt.h"
#include "ui.h"

void lesson_01_gtk(void)
{
    title("MODULE 1 - GTK: YOUR FIRST WINDOW");

    heading("WHAT YOU WILL HAVE AT THE END");

    printf("  A real window on your screen, with a button in it, and the\n");
    printf("  button changes its own text when you click it. Three short\n");
    printf("  programs get you there, each one a small step from the last.\n\n");

    printf("  GTK is a C library for building graphical programs: windows,\n");
    printf("  buttons, text fields. It is what GNOME and many Linux apps\n");
    printf("  are built with. You call its functions the way you call any\n");
    printf("  C library. Everything you already know about C still applies.\n\n");

    printf("  How to read this module: the code is printed in full, and\n");
    printf("  every line has a comment saying what it does. Read the\n");
    printf("  comments, then type the program into a file yourself and run\n");
    printf("  it. Typing it is how it sticks.\n");

    wait_enter();
    clear_screen();
    heading("STEP 0: INSTALL GTK4 AND CHECK IT WORKED");

    printf("  GTK is not part of your C compiler, so it is installed\n");
    printf("  separately. You need the \"development files\" (the headers\n");
    printf("  your code #includes), not only the program libraries:\n\n");

    printf("    Arch:           sudo pacman -S gtk4\n");
    printf("    Debian/Ubuntu:  sudo apt install libgtk-4-dev\n");
    printf("    Void:           sudo xbps-install gtk4-devel\n\n");

    printf("  Another distribution: search your package manager for the\n");
    printf("  GTK 4 development package.\n\n");

    printf("  You also need gcc (or another C compiler) and pkg-config.\n\n");

    printf("  Check it worked:\n\n");
    printf("    pkg-config --modversion gtk4\n\n");

    printf("  This prints a version number (4.22.5 on the machine this\n");
    printf("  module was written on; yours will be 4.something). If it\n");
    printf("  prints this instead, GTK4's development files are not\n");
    printf("  installed yet:\n\n");

    printf("    Package gtk4 was not found in the pkg-config search path.\n\n");

    printf("  This program, the lesson you are reading, needs none of\n");
    printf("  this. Only the code you compile yourself touches GTK.\n");

    wait_enter();
    clear_screen();
    heading("STEP 1: THE SMALLEST WINDOW");

    printf("  Save this as window.c. Every line is commented:\n\n");

    rule();
    printf("  // window.c : the smallest GTK4 program that opens a window.\n");
    printf("  \n");
    printf("  #include <gtk/gtk.h>   // everything GTK offers: its types, functions and macros\n");
    printf("  \n");
    printf("  // activate() is a \"callback\": a function that GTK calls for us.\n");
    printf("  // GTK calls it once, when the program is ready to show its first window.\n");
    printf("  // GTK always passes these two parameters, even if we do not use them:\n");
    printf("  //   app       - the application object that is running\n");
    printf("  //   user_data - an extra pointer we may choose to hand over (unused here)\n");
    printf("  static void activate(GtkApplication *app, gpointer user_data)\n");
    printf("  {\n");
    printf("      (void) user_data;   // we do not use it; this line silences the compiler's\n");
    printf("                          // \"unused parameter\" warning\n");
    printf("  \n");
    printf("      // Make a new window that belongs to our application.\n");
    printf("      // It returns a GtkWidget *, GTK's general type for \"any thing on screen\".\n");
    printf("      GtkWidget *window = gtk_application_window_new(app);\n");
    printf("  \n");
    printf("      // Set the text in the title bar.\n");
    printf("      // GTK_WINDOW(window) says \"treat this general GtkWidget as a GtkWindow\".\n");
    printf("      // C has no inheritance, so GTK uses cast macros like this one, and the\n");
    printf("      // macro also checks at run time that the cast is really valid.\n");
    printf("      gtk_window_set_title(GTK_WINDOW(window), \"My first window\");\n");
    printf("  \n");
    printf("      // Starting size in pixels: 300 wide, 200 tall. The user can still resize.\n");
    printf("      gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);\n");
    printf("  \n");
    printf("      // A new window starts hidden. present() shows it and brings it forward.\n");
    printf("      gtk_window_present(GTK_WINDOW(window));\n");
    printf("  }\n");
    printf("  \n");
    printf("  int main(int argc, char **argv)\n");
    printf("  {\n");
    printf("      // Create the application object.\n");
    printf("      // 1st argument: an id that names the app, written like a reversed web\n");
    printf("      //   address. \"com.example.window\" is a placeholder; a real app would use\n");
    printf("      //   a domain it owns.\n");
    printf("      // 2nd argument: flags. G_APPLICATION_DEFAULT_FLAGS means \"nothing special\".\n");
    printf("      GtkApplication *app = gtk_application_new(\n");
    printf("          \"com.example.window\", G_APPLICATION_DEFAULT_FLAGS);\n");
    printf("  \n");
    printf("      // Say: \"when the app emits the signal named activate, call activate()\".\n");
    printf("      // \"activate\" is the signal's name, written as text.\n");
    printf("      // G_CALLBACK(...) turns our function into the generic function pointer\n");
    printf("      //   type that g_signal_connect expects.\n");
    printf("      // NULL is the user_data we want passed to our function: nothing.\n");
    printf("      g_signal_connect(app, \"activate\", G_CALLBACK(activate), NULL);\n");
    printf("  \n");
    printf("      // Hand control to GTK. This call does not return until the last window\n");
    printf("      // is closed. While it runs, GTK shows the window and reacts to the\n");
    printf("      // mouse, the keyboard, resizing and closing.\n");
    printf("      // argc and argv pass the command-line arguments on to GTK.\n");
    printf("      int status = g_application_run(G_APPLICATION(app), argc, argv);\n");
    printf("  \n");
    printf("      // We created app, so we release our hold on it when we are done.\n");
    printf("      g_object_unref(app);\n");
    printf("  \n");
    printf("      // Return GTK's exit code to the shell (0 means everything went fine).\n");
    printf("      return status;\n");
    printf("  }\n");
    rule();

    wait_enter();
    clear_screen();
    heading("STEP 1: COMPILE IT AND RUN IT");

    printf("    gcc -Wall -Wextra window.c -o window $(pkg-config --cflags gtk4 --libs gtk4)\n");
    printf("    ./window\n\n");

    printf("  What you should see: a 300 by 200 window titled \"My first\n");
    printf("  window\", with nothing inside it. Close it with the X in its\n");
    printf("  title bar. The terminal gets its prompt back, and\n");
    printf("  echo $? prints 0, which means the program ended normally.\n\n");

    printf("  About that compile line. A plain gcc call cannot find GTK:\n\n");
    printf("    fatal error: gtk/gtk.h: No such file or directory\n\n");

    printf("  gcc does not know where GTK's headers live or which\n");
    printf("  libraries to link, and there can be several GTK versions\n");
    printf("  installed side by side. pkg-config knows. The $( ... ) part\n");
    printf("  runs it first and pastes its answer into the gcc command:\n");
    printf("  --cflags gives the include paths, --libs gives what to link.\n");
    printf("  Any library that registers with pkg-config is compiled this\n");
    printf("  same way.\n");

    wait_enter();
    clear_screen();
    heading("STEP 2: ADD A BUTTON");

    printf("  Save this as button.c. Lines marked NEW are what changed\n");
    printf("  from window.c:\n\n");

    rule();
    printf("  // button.c : window.c plus a button. The button does nothing yet.\n");
    printf("  \n");
    printf("  #include <gtk/gtk.h>\n");
    printf("  \n");
    printf("  static void activate(GtkApplication *app, gpointer user_data)\n");
    printf("  {\n");
    printf("      (void) user_data;\n");
    printf("  \n");
    printf("      GtkWidget *window = gtk_application_window_new(app);\n");
    printf("      gtk_window_set_title(GTK_WINDOW(window), \"My first window\");\n");
    printf("      gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);\n");
    printf("  \n");
    printf("      // NEW: make a button with the text \"Click me\" on it.\n");
    printf("      GtkWidget *button = gtk_button_new_with_label(\"Click me\");\n");
    printf("  \n");
    printf("      // NEW: put the button inside the window.\n");
    printf("      // A window can hold exactly ONE child, and here that child is the button.\n");
    printf("      // (Module 2 shows how to fit several widgets in a window.)\n");
    printf("      gtk_window_set_child(GTK_WINDOW(window), button);\n");
    printf("  \n");
    printf("      gtk_window_present(GTK_WINDOW(window));\n");
    printf("  }\n");
    printf("  \n");
    printf("  int main(int argc, char **argv)\n");
    printf("  {\n");
    printf("      GtkApplication *app = gtk_application_new(\n");
    printf("          \"com.example.button\", G_APPLICATION_DEFAULT_FLAGS);\n");
    printf("      g_signal_connect(app, \"activate\", G_CALLBACK(activate), NULL);\n");
    printf("      int status = g_application_run(G_APPLICATION(app), argc, argv);\n");
    printf("      g_object_unref(app);\n");
    printf("      return status;\n");
    printf("  }\n");
    rule();

    printf("\n  Compile and run it the same way. You should see a window\n");
    printf("  with a button reading \"Click me\". Clicking it does nothing\n");
    printf("  yet; that is the next step.\n");

    wait_enter();
    clear_screen();
    heading("STEP 3: MAKE THE BUTTON DO SOMETHING");

    printf("  Save this as click.c. Lines marked NEW are what changed\n");
    printf("  from button.c:\n\n");

    rule();
    printf("  // click.c : button.c, now the button reacts when it is clicked.\n");
    printf("  \n");
    printf("  #include <gtk/gtk.h>\n");
    printf("  \n");
    printf("  // NEW: a second callback. GTK calls it every time the button is clicked.\n");
    printf("  // For the \"clicked\" signal GTK passes:\n");
    printf("  //   button    - the button that was clicked\n");
    printf("  //   user_data - whatever we gave g_signal_connect (NULL below)\n");
    printf("  static void on_click(GtkButton *button, gpointer user_data)\n");
    printf("  {\n");
    printf("      (void) user_data;                          // unused, silences the warning\n");
    printf("      gtk_button_set_label(button, \"Clicked!\");  // replace the text on the button\n");
    printf("  }\n");
    printf("  \n");
    printf("  static void activate(GtkApplication *app, gpointer user_data)\n");
    printf("  {\n");
    printf("      (void) user_data;\n");
    printf("  \n");
    printf("      GtkWidget *window = gtk_application_window_new(app);\n");
    printf("      gtk_window_set_title(GTK_WINDOW(window), \"My first window\");\n");
    printf("      gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);\n");
    printf("  \n");
    printf("      GtkWidget *button = gtk_button_new_with_label(\"Click me\");\n");
    printf("  \n");
    printf("      // NEW: connect the button's \"clicked\" signal to on_click().\n");
    printf("      // Same call as in main(), just on the button instead of the app.\n");
    printf("      // Careful: \"clicked\" is only text. A typo such as \"click\" compiles\n");
    printf("      // without any error and the function simply never gets called.\n");
    printf("      g_signal_connect(button, \"clicked\", G_CALLBACK(on_click), NULL);\n");
    printf("  \n");
    printf("      gtk_window_set_child(GTK_WINDOW(window), button);\n");
    printf("      gtk_window_present(GTK_WINDOW(window));\n");
    printf("  }\n");
    printf("  \n");
    printf("  int main(int argc, char **argv)\n");
    printf("  {\n");
    printf("      GtkApplication *app = gtk_application_new(\n");
    printf("          \"com.example.click\", G_APPLICATION_DEFAULT_FLAGS);\n");
    printf("      g_signal_connect(app, \"activate\", G_CALLBACK(activate), NULL);\n");
    printf("      int status = g_application_run(G_APPLICATION(app), argc, argv);\n");
    printf("      g_object_unref(app);\n");
    printf("      return status;\n");
    printf("  }\n");
    rule();

    printf("\n  Compile and run it. Click the button: its text changes from\n");
    printf("  \"Click me\" to \"Clicked!\". That is a complete GTK program:\n");
    printf("  a window, a widget in it, and a function that runs when you\n");
    printf("  use it.\n");

    wait_enter();
    clear_screen();
    heading("TRAP 1: g_application_run, not gtk_init");

    printf("  Many tutorials online are for GTK3, where a program calls\n");
    printf("  gtk_init() and then gtk_main(). That is not how GTK4 is\n");
    printf("  structured. In GTK4, main() builds a GtkApplication and hands\n");
    printf("  control to g_application_run(). GTK calls your activate()\n");
    printf("  function once it is ready, and the window is built there.\n");
    printf("  Building a window inside main(), before that event loop\n");
    printf("  exists, is the mistake this layout exists to prevent.\n");

    wait_enter();
    clear_screen();
    heading("TRAP 2: GTK checks the type of a cast, and only while running");

    printf("  GTK_WINDOW(widget) is a checked cast. Give it something that\n");
    printf("  is not a window, for example a button, and the program still\n");
    printf("  compiles. When it runs, GTK prints (this is real output from\n");
    printf("  passing a button to gtk_window_set_title):\n\n");

    printf("    GLib-GObject-CRITICAL: invalid cast from 'GtkButton' to 'GtkWindow'\n");
    printf("    Gtk-CRITICAL: gtk_window_set_title: assertion 'GTK_IS_WINDOW (window)' failed\n\n");

    printf("  Those lines are the warning. The program keeps going, and\n");
    printf("  the call you made did nothing. So when a GTK call seems to be\n");
    printf("  ignored, read the terminal you started the program from.\n");

    wait_enter();
    clear_screen();
    heading("TRAP 3: what G_CALLBACK does not check");

    printf("  \"clicked\" is a piece of text that GTK matches while the\n");
    printf("  program runs. Nothing checks it when you compile, so a typo\n");
    printf("  (\"click\") does not give an error. The function simply never\n");
    printf("  runs.\n\n");

    printf("  The second half is sharper. G_CALLBACK(on_click) turns your\n");
    printf("  function into a generic function pointer. It does not check\n");
    printf("  that on_click's parameters match what the \"clicked\" signal\n");
    printf("  passes. Get the parameters wrong and it still compiles, and\n");
    printf("  GTK calls your function with arguments it was never written\n");
    printf("  for. That is undefined behaviour, not a caught error. Plain C\n");
    printf("  already treats calling a function pointer with the wrong\n");
    printf("  parameters as undefined behaviour; G_CALLBACK just makes it\n");
    printf("  easy to do by accident, because the compiler no longer has\n");
    printf("  the real signature to check against.\n\n");

    printf("  The correct parameters for each signal are listed per widget\n");
    printf("  in GTK's reference at docs.gtk.org. Check it whenever a\n");
    printf("  callback runs but behaves strangely.\n");

    wait_enter();
    clear_screen();
    heading("TRAP 4: who frees the button?");

    printf("  Nowhere above is there a free(button) or free(window). If you\n");
    printf("  know malloc and free from plain C, GTK widgets work\n");
    printf("  differently on purpose. Every GtkWidget is a reference\n");
    printf("  counted GObject. gtk_window_set_child() makes the window hold\n");
    printf("  a reference to the button, and closing the window drops it,\n");
    printf("  which frees the button. You release only what you created\n");
    printf("  yourself with a plain reference: g_object_unref(app) in\n");
    printf("  main(). The widget tree owns everything under it.\n");

    wait_enter();
    clear_screen();
    exercise(1);

    question("gcc file.c -o app fails with \"gtk/gtk.h: No such file or\n"
             "  directory\" even though GTK is installed. What is missing\n"
             "  from the compile line? (one word)",
             "pkg-config",
             "gcc has no built-in idea where GTK's headers or libraries\n"
             "             live; $(pkg-config --cflags gtk4) and $(pkg-config\n"
             "             --libs gtk4) are what supply that.");

    question("Where does a GTK4 app build its window: inside main(), or\n"
             "  inside the function connected to the \"activate\" signal?",
             "activate",
             "main() only builds the GtkApplication and starts\n"
             "             g_application_run()'s event loop. The loop calls\n"
             "             activate() once it is actually ready to show a window.");

    question("If the function you pass to G_CALLBACK has the wrong\n"
             "  parameters for the signal, does the compiler stop you?\n"
             "  (yes or no)",
             "no",
             "G_CALLBACK turns your function into a generic function\n"
             "             pointer, which erases the real parameter types the\n"
             "             compiler could have checked. The mismatch becomes\n"
             "             undefined behaviour when GTK calls it, not a compile error.");

    question("Who frees a GtkButton that was added to a window with\n"
             "  gtk_window_set_child()? (one word: you, or GTK)",
             "gtk",
             "GtkWidgets are reference-counted GObjects. The window holds\n"
             "             a reference to its child; destroying the window\n"
             "             drops it, which is what actually frees the button.\n"
             "             You never call free() on it directly.");

    question("How many children can a GtkWindow hold?\n"
             "  (a number)",
             "1",
             "gtk_window_set_child() takes one widget. To show several\n"
             "             widgets, put them in a container such as a GtkBox and\n"
             "             make that the window's child (module 2).");

    printf("\n  >> BUILD THIS YOURSELF, in a real file\n");
    rule();
    printf("  Starting from the full example above, add a GtkLabel above\n");
    printf("  the button (gtk_label_new(\"...\")) and a container to hold\n");
    printf("  both widgets, since gtk_window_set_child() only takes one\n");
    printf("  child -- GtkBox is the simplest (gtk_box_new, then\n");
    printf("  gtk_box_append for each widget, then set the box as the\n");
    printf("  window's child instead of the button directly). Make the\n");
    printf("  click handler update the label's text instead of the\n");
    printf("  button's own label.\n\n");

    printf("  There is no fixed transcript to match here -- the result is a\n");
    printf("  window, not text. You have it right when a window opens with\n");
    printf("  a label and a button stacked vertically, and clicking the\n");
    printf("  button changes the label's text.\n\n");

    printf("  Compile with:\n");
    printf("    gcc -Wall -Wextra $(pkg-config --cflags gtk4) main.c -o app $(pkg-config --libs gtk4)\n");

    if (ask_yes("Want to see example code?")) {
        printf("\n");
        rule();
        printf("  #include <gtk/gtk.h>\n");
        printf("  \n");
        printf("  static void on_click(GtkButton *button, gpointer user_data)\n");
        printf("  {\n");
        printf("      (void) button;\n");
        printf("      gtk_label_set_text(GTK_LABEL(user_data), \"Clicked!\");\n");
        printf("  }\n");
        printf("  \n");
        printf("  static void activate(GtkApplication *app, gpointer user_data)\n");
        printf("  {\n");
        printf("      (void) user_data;\n");
        printf("      GtkWidget *window = gtk_application_window_new(app);\n");
        printf("      gtk_window_set_title(GTK_WINDOW(window), \"Label Demo\");\n");
        printf("      gtk_window_set_default_size(GTK_WINDOW(window), 300, 120);\n");
        printf("  \n");
        printf("      GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);\n");
        printf("      GtkWidget *label = gtk_label_new(\"Not clicked yet\");\n");
        printf("      GtkWidget *button = gtk_button_new_with_label(\"Click me\");\n");
        printf("  \n");
        printf("      g_signal_connect(button, \"clicked\", G_CALLBACK(on_click), label);\n");
        printf("  \n");
        printf("      gtk_box_append(GTK_BOX(box), label);\n");
        printf("      gtk_box_append(GTK_BOX(box), button);\n");
        printf("      gtk_window_set_child(GTK_WINDOW(window), box);\n");
        printf("  \n");
        printf("      gtk_window_present(GTK_WINDOW(window));\n");
        printf("  }\n");
        printf("  \n");
        printf("  int main(int argc, char **argv)\n");
        printf("  {\n");
        printf("      GtkApplication *app = gtk_application_new(\n");
        printf("          \"com.example.labeldemo\", G_APPLICATION_DEFAULT_FLAGS);\n");
        printf("      g_signal_connect(app, \"activate\", G_CALLBACK(activate), NULL);\n");
        printf("      int status = g_application_run(G_APPLICATION(app), argc, argv);\n");
        printf("      g_object_unref(app);\n");
        printf("      return status;\n");
        printf("  }\n");
        rule();
        printf("  user_data is how on_click reaches the label without a\n");
        printf("  global variable: g_signal_connect's fourth argument is\n");
        printf("  handed straight to the callback's user_data parameter,\n");
        printf("  so passing `label` there makes it available inside\n");
        printf("  on_click without either widget needing to know about the\n");
        printf("  other ahead of time.\n");
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    if (!print_summary_pt(1)) {
        printf("   - GTK needs pkg-config in the compile line; gcc alone\n");
        printf("     cannot find its headers or libraries\n");
        printf("   - a GTK4 app builds its window inside the function connected\n");
        printf("     to \"activate\", not inside main() -- main() only starts\n");
        printf("     the event loop\n");
        printf("   - G_CALLBACK casts away the real function signature, so a\n");
        printf("     mismatched callback compiles and misbehaves instead of\n");
        printf("     failing to build\n");
        printf("   - widgets are reference-counted GObjects, not malloc'd\n");
        printf("     blocks -- the window frees its children when it is freed,\n");
        printf("     you do not free() them yourself\n\n");

        printf("  This is one window with one button. GTK's own reference at\n");
        printf("  docs.gtk.org is where the rest of it -- layout containers,\n");
        printf("  dozens of widgets, CSS styling -- actually lives; this module\n");
        printf("  is the trap-filled first step onto that ground, not the\n");
        printf("  whole of it.\n");
    }
    wait_enter();
}
