/*
 * Module 1 - GTK: your first window.
 *
 * This course, unlike c/, cpp/, python/ and javascript/, is not a
 * zero-dependency single binary all the way down: the lesson program
 * itself (this file, compiled into gtk-course) needs nothing beyond a C
 * compiler, but the code it teaches needs GTK4's development files
 * installed to actually compile and run.
 *
 * What was run, on gcc 14.2.1 and GTK 4.22.5:
 *
 *   - the three examples (window.c, button.c, click.c) compile with
 *     -std=c11 -Wall -Wextra and no warnings, also with the exact command
 *     the lesson prints. A test program read the title back, checked the
 *     window was visible, emitted "clicked" and read the label before and
 *     after, and checked the exit status was 0.
 *   - every BAD example in the reference screens was compiled or run, and
 *     the error or message quoted next to it is its real output:
 *       no GTK_WINDOW cast             -> compile error (gcc 14)
 *       GTK_WINDOW(button)             -> GLib-GObject-CRITICAL at run time
 *       window never presented         -> window exists, visible = 0, app keeps running
 *       activate() creates no window   -> g_application_run returns 0 at once
 *       application id "myapp"         -> Gtk-CRITICAL, gtk_application_new returns NULL
 *       signal name "clik"             -> GLib-GObject-CRITICAL, g_signal_connect returns 0
 *       button never added to window   -> no message, gtk_window_get_child is NULL
 *       gtk_init(&argc,&argv), gtk_main -> compile errors in GTK4
 *   - not run, because they are undefined behaviour: a callback with the
 *     wrong parameters, and releasing a reference you do not own.
 *
 * The comments inside the examples are the lesson. They are printed as they
 * appear in the compiled files.
 */

#include <stdio.h>

#include "lessons.h"
#include "strings_pt.h"
#include "ui.h"

static const char *const EX_WINDOW[] = {
    "// window.c : the smallest GTK4 program that opens a window.",
    "",
    "#include <gtk/gtk.h>   // everything GTK offers: types, functions, macros",
    "",
    "// activate() is a \"callback\": a function that GTK calls for us.",
    "// GTK calls it once, when the program is ready to show its first window.",
    "// GTK always passes these two parameters, even if we do not use them:",
    "//   app       - the application object that is running",
    "//   user_data - an extra pointer we may hand over (unused here)",
    "static void activate(GtkApplication *app, gpointer user_data)",
    "{",
    "    (void) user_data;   // unused; this stops the compiler's",
    "                        // \"unused parameter\" warning",
    "",
    "    // Make a new window that belongs to our application.",
    "    // It returns a GtkWidget *, GTK's general type for \"any widget\".",
    "    GtkWidget *window = gtk_application_window_new(app);",
    "",
    "    // Set the text in the title bar.",
    "    // GTK_WINDOW(window) says \"treat this GtkWidget as a GtkWindow\".",
    "    // C has no inheritance, so GTK uses cast macros like this one, and",
    "    // the macro also checks at run time that the cast is valid.",
    "    gtk_window_set_title(GTK_WINDOW(window), \"My first window\");",
    "",
    "    // Starting size in pixels: 300 wide, 200 tall (users can resize).",
    "    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);",
    "",
    "    // A new window starts hidden. present() shows it, in front.",
    "    gtk_window_present(GTK_WINDOW(window));",
    "}",
    "",
    "int main(int argc, char **argv)",
    "{",
    "    // Create the application object.",
    "    // 1st argument: an id that names the app, written like a reversed",
    "    //   web address. \"com.example.window\" is a placeholder; a real app",
    "    //   would use a domain it owns.",
    "    // 2nd argument: flags. DEFAULT_FLAGS means \"nothing special\".",
    "    GtkApplication *app = gtk_application_new(",
    "        \"com.example.window\", G_APPLICATION_DEFAULT_FLAGS);",
    "",
    "    // Say: \"when the app emits the signal \"activate\", call activate()\".",
    "    // \"activate\" is the signal's name, written as text.",
    "    // G_CALLBACK(...) turns our function into the generic function",
    "    //   pointer type that g_signal_connect expects.",
    "    // NULL is the user_data we want passed to our function: nothing.",
    "    g_signal_connect(app, \"activate\", G_CALLBACK(activate), NULL);",
    "",
    "    // Hand control to GTK. This call does not return until the last",
    "    // window is closed. While it runs, GTK shows the window and",
    "    // reacts to the mouse, the keyboard, resizing and closing.",
    "    // argc and argv pass the command-line arguments on to GTK.",
    "    int status = g_application_run(G_APPLICATION(app), argc, argv);",
    "",
    "    // We created app, so we release our hold on it when we are done.",
    "    g_object_unref(app);",
    "",
    "    // Return GTK's exit code to the shell (0 means all went fine).",
    "    return status;",
    "}",
};
static const char *const EX_BUTTON[] = {
    "// button.c : window.c plus a button. The button does nothing yet.",
    "",
    "#include <gtk/gtk.h>",
    "",
    "static void activate(GtkApplication *app, gpointer user_data)",
    "{",
    "    (void) user_data;",
    "",
    "    GtkWidget *window = gtk_application_window_new(app);",
    "    gtk_window_set_title(GTK_WINDOW(window), \"My first window\");",
    "    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);",
    "",
    "    // NEW: make a button with the text \"Click me\" on it.",
    "    GtkWidget *button = gtk_button_new_with_label(\"Click me\");",
    "",
    "    // NEW: put the button inside the window.",
    "    // A window holds exactly ONE child; here that child is the button.",
    "    // (Module 2 shows how to fit several widgets in a window.)",
    "    gtk_window_set_child(GTK_WINDOW(window), button);",
    "",
    "    gtk_window_present(GTK_WINDOW(window));",
    "}",
    "",
    "int main(int argc, char **argv)",
    "{",
    "    GtkApplication *app = gtk_application_new(",
    "        \"com.example.button\", G_APPLICATION_DEFAULT_FLAGS);",
    "    g_signal_connect(app, \"activate\", G_CALLBACK(activate), NULL);",
    "    int status = g_application_run(G_APPLICATION(app), argc, argv);",
    "    g_object_unref(app);",
    "    return status;",
    "}",
};
static const char *const EX_CLICK[] = {
    "// click.c : button.c, now the button reacts when it is clicked.",
    "",
    "#include <gtk/gtk.h>",
    "",
    "// NEW: a second callback. GTK calls it every time the button is clicked.",
    "// For the \"clicked\" signal GTK passes:",
    "//   button    - the button that was clicked",
    "//   user_data - whatever we gave g_signal_connect (NULL below)",
    "static void on_click(GtkButton *button, gpointer user_data)",
    "{",
    "    (void) user_data;   // unused, silences the warning",
    "",
    "    // Replace the text on the button that was clicked.",
    "    gtk_button_set_label(button, \"Clicked!\");",
    "}",
    "",
    "static void activate(GtkApplication *app, gpointer user_data)",
    "{",
    "    (void) user_data;",
    "",
    "    GtkWidget *window = gtk_application_window_new(app);",
    "    gtk_window_set_title(GTK_WINDOW(window), \"My first window\");",
    "    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);",
    "",
    "    GtkWidget *button = gtk_button_new_with_label(\"Click me\");",
    "",
    "    // NEW: connect the button's \"clicked\" signal to on_click().",
    "    // Same call as in main(), just on the button instead of the app.",
    "    // Careful: \"clicked\" is only text. A typo such as \"click\" compiles",
    "    // without any error and the function simply never gets called.",
    "    g_signal_connect(button, \"clicked\", G_CALLBACK(on_click), NULL);",
    "",
    "    gtk_window_set_child(GTK_WINDOW(window), button);",
    "    gtk_window_present(GTK_WINDOW(window));",
    "}",
    "",
    "int main(int argc, char **argv)",
    "{",
    "    GtkApplication *app = gtk_application_new(",
    "        \"com.example.click\", G_APPLICATION_DEFAULT_FLAGS);",
    "    g_signal_connect(app, \"activate\", G_CALLBACK(activate), NULL);",
    "    int status = g_application_run(G_APPLICATION(app), argc, argv);",
    "    g_object_unref(app);",
    "    return status;",
    "}",
};

static const char *const GOOD_ID[] = {
    "GtkApplication *app = gtk_application_new(",
    "    \"com.example.notes\", G_APPLICATION_DEFAULT_FLAGS);",
};
static const char *const BAD_ID[] = {
    "GtkApplication *app = gtk_application_new(",
    "    \"myapp\", G_APPLICATION_DEFAULT_FLAGS);   // no dot: not a valid id",
};
static const char *const BAD_NOWINDOW[] = {
    "static void activate(GtkApplication *app, gpointer user_data)",
    "{",
    "    // forgot to create a window here",
    "}",
};
static const char *const GOOD_CAST[] = {
    "gtk_window_set_title(GTK_WINDOW(window), \"Notes\");",
};
static const char *const BAD_NOCAST[] = {
    "gtk_window_set_title(window, \"Notes\");   // window is a GtkWidget *",
};
static const char *const BAD_WRONGCAST[] = {
    "GtkWidget *button = gtk_button_new();",
    "gtk_window_set_title(GTK_WINDOW(button), \"Notes\");   // not a window",
};
static const char *const BAD_NOPRESENT[] = {
    "GtkWidget *window = gtk_application_window_new(app);",
    "gtk_window_set_title(GTK_WINDOW(window), \"Notes\");",
    "// no gtk_window_present(): the window is never shown",
};
static const char *const BAD_ORPHAN[] = {
    "GtkWidget *button = gtk_button_new_with_label(\"Click me\");",
    "// no gtk_window_set_child(): the button is never put in the window",
};
static const char *const GOOD_CONNECT[] = {
    "g_signal_connect(button, \"clicked\", G_CALLBACK(on_click), NULL);",
};
static const char *const BAD_TYPO[] = {
    "g_signal_connect(button, \"clik\", G_CALLBACK(on_click), NULL);",
    "// \"clik\" is not a signal of GtkButton",
};
static const char *const GOOD_PRACTICE[] = {
    "// one small function per event, named on_<widget>_<event>, kept static",
    "static void on_save_clicked(GtkButton *button, gpointer user_data)",
    "{",
    "    (void) button;      // parameters we do not use are cast to void,",
    "    (void) user_data;   // never deleted: the signature must stay exact",
    "    // ... do the work here ...",
    "}",
};

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
    printf("  C library, and everything you know about C still applies.\n\n");

    printf("  How to read this module:\n\n");
    printf("    1. Read the idea.\n");
    printf("    2. Read the program. Every line has a comment.\n");
    printf("    3. Read the reference: what each function does, a good\n");
    printf("       use, and a bad use with the exact error it gives.\n");
    printf("    4. Type the program into a file yourself and run it.\n");
    printf("       Typing it is how it sticks.\n");

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
    printf("  GTK 4 development package. You also need gcc (or another C\n");
    printf("  compiler) and pkg-config.\n\n");

    printf("  Check it worked:\n\n");
    printf("    pkg-config --modversion gtk4\n\n");

    printf("  This prints a version number (4.22.5 on the machine this\n");
    printf("  module was written on; yours will be 4.something). If it\n");
    printf("  prints this instead, the development files are not\n");
    printf("  installed yet:\n\n");

    printf("    Package gtk4 was not found in the pkg-config search path.\n\n");

    printf("  This lesson program needs none of this. Only the code you\n");
    printf("  compile yourself touches GTK.\n");

    wait_enter();
    clear_screen();
    heading("THE IDEA: A WINDOW PROGRAM WAITS FOR EVENTS");

    printf("  A normal C program runs from the top of main() to the\n");
    printf("  bottom and ends. A window cannot work like that: it has to\n");
    printf("  stay open and wait for the user to click, type or resize.\n\n");

    printf("  So GTK runs a loop for you, called the event loop. It waits\n");
    printf("  for something to happen, calls the right function of yours,\n");
    printf("  and goes back to waiting. You never write that loop. You\n");
    printf("  write small functions and tell GTK when to call them.\n");
    printf("  That one idea is behind everything in this course.\n\n");

    printf("  Three words you will see constantly:\n\n");

    ref_line("widget", "anything on screen: a window, a button, a label.");
    ref_line("signal", "a named event a widget announces: \"clicked\".");
    ref_line("callback", "your function, which GTK calls when a signal happens.");

    printf("\n  Wiring a callback to a signal is done with g_signal_connect().\n");
    printf("  Every program below uses it.\n");

    wait_enter();
    clear_screen();
    heading("STEP 1: THE SMALLEST WINDOW");

    printf("  Save this as window.c. Comments are green:\n\n");

    code(LINES(EX_WINDOW));

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
    printf("  libraries to link, and several GTK versions can be installed\n");
    printf("  side by side. pkg-config knows. The $( ... ) part runs it\n");
    printf("  first and pastes its answer into the gcc command:\n");
    printf("  --cflags gives the include paths, --libs gives what to link.\n");
    printf("  Any library that registers with pkg-config is compiled this\n");
    printf("  same way.\n");

    wait_enter();
    clear_screen();
    heading("REFERENCE 1/3: THE APPLICATION");

    ref_head("GtkApplication *gtk_application_new(id, flags)");
    ref_line("Does:", "creates the application object. It owns your windows and");
    ref_line("", "runs the event loop.");
    ref_line("Takes:", "id     text like \"com.example.notes\": a reversed web");
    ref_line("", "       address with at least one dot.");
    ref_line("", "flags  G_APPLICATION_DEFAULT_FLAGS, meaning \"nothing special\".");
    ref_line("Returns:", "a new application you must release with g_object_unref().");
    printf("\n");
    code_good(LINES(GOOD_ID));
    code_bad(LINES(BAD_ID));
    bad("Gtk-CRITICAL: gtk_application_new: assertion 'application_id == NULL ||");
    more("g_application_id_is_valid (application_id)' failed");
    more("It returns NULL, so every later call that uses app fails too.");

    ref_head("gulong g_signal_connect(instance, \"signal\", G_CALLBACK(fn), data)");
    ref_line("Does:", "tells GTK to call fn whenever instance emits that signal.");
    ref_line("Takes:", "instance  the widget or application to listen to.");
    ref_line("", "signal    the signal's name, as text.");
    ref_line("", "fn        your callback.");
    ref_line("", "data      handed to your callback as user_data (NULL: none).");
    ref_line("Returns:", "a number identifying the connection, or 0 if it failed.");
    ref_line("", "(Most programs ignore it.)");

    ref_head("G_CALLBACK(fn)");
    ref_line("Does:", "casts your function to GTK's generic function-pointer");
    ref_line("", "type. It does not check that your parameters are right.");

    wait_enter();
    clear_screen();
    heading("REFERENCE 1/3: THE APPLICATION (continued)");

    ref_head("int g_application_run(app, argc, argv)");
    ref_line("Does:", "starts the event loop and calls your activate() once.");
    ref_line("", "It does not return until the last window is closed.");
    ref_line("Takes:", "argc, argv  your command-line arguments, passed on to GTK.");
    ref_line("Returns:", "the exit status. 0 means everything went fine.");
    printf("\n");
    code_bad(LINES(BAD_NOWINDOW));
    bad("activate() ran and made no window, so there is nothing to wait for:");
    more("g_application_run returned 0 immediately. The program starts and");
    more("vanishes with no message at all.");

    ref_head("void g_object_unref(object)");
    ref_line("Does:", "gives up one hold on an object. When nobody holds it,");
    ref_line("", "it is freed. GTK objects are counted, not malloc'd.");
    ref_line("Rule:", "release exactly what you created yourself: the app,");
    ref_line("", "once, after g_application_run returns.");
    bad("Releasing something you do not own, or releasing twice, can free an");
    more("object GTK is still using. That is undefined behaviour, so this");
    more("course explains it and does not run it.");

    wait_enter();
    clear_screen();
    heading("REFERENCE 2/3: THE WINDOW");

    ref_head("GtkWidget *gtk_application_window_new(app)");
    ref_line("Does:", "creates a window that belongs to app. It starts hidden.");
    ref_line("Returns:", "a GtkWidget *. You do not free it: the application");
    ref_line("", "keeps it until it is closed.");

    ref_head("GTK_WINDOW(widget)");
    ref_line("Does:", "a checked cast: \"treat this GtkWidget * as a GtkWindow *\".");
    ref_line("", "C has no inheritance, so GTK uses macros like this one.");
    ref_line("", "The same pattern exists for every type: GTK_BOX, GTK_LABEL...");

    ref_head("void gtk_window_set_title(window, title)");
    ref_line("Does:", "sets the text in the title bar.");
    ref_line("Takes:", "window  a GtkWindow * (so use the GTK_WINDOW cast).");
    ref_line("", "title   the text, as a C string.");
    printf("\n");
    code_good(LINES(GOOD_CAST));
    code_bad(LINES(BAD_NOCAST));
    bad("error: passing argument 1 of 'gtk_window_set_title' from incompatible");
    more("pointer type [-Wincompatible-pointer-types]");
    more("(gcc 14 makes this an error. Older gcc only warned, which is how");
    more("the mistake used to survive.)");
    printf("\n");
    code_bad(LINES(BAD_WRONGCAST));
    bad("It compiles. When it runs, GTK prints:");
    more("GLib-GObject-CRITICAL: invalid cast from 'GtkButton' to 'GtkWindow'");
    more("Gtk-CRITICAL: gtk_window_set_title: assertion");
    more("'GTK_IS_WINDOW (window)' failed");
    more("The program keeps going and the call did nothing. A CRITICAL line");
    more("always means a GTK call was ignored.");

    wait_enter();
    clear_screen();
    heading("REFERENCE 2/3: THE WINDOW (continued)");

    ref_head("void gtk_window_set_default_size(window, width, height)");
    ref_line("Does:", "sets the size the window starts at, in pixels. The user");
    ref_line("", "can still resize it.");

    ref_head("void gtk_window_present(window)");
    ref_line("Does:", "shows the window and brings it to the front.");
    printf("\n");
    code_bad(LINES(BAD_NOPRESENT));
    bad("Nothing is shown. The window exists but is invisible (read back:");
    more("visible = 0 a second later), and the program keeps running, so it");
    more("looks hung with nothing on screen. No error is printed.");

    wait_enter();
    clear_screen();
    heading("STEP 2: ADD A BUTTON");

    printf("  Save this as button.c. Lines marked NEW changed from window.c:\n\n");

    code(LINES(EX_BUTTON));

    printf("\n  Compile and run it the same way. You should see a window\n");
    printf("  with a button reading \"Click me\". Clicking it does nothing\n");
    printf("  yet. That is the next step.\n");

    wait_enter();
    clear_screen();
    heading("REFERENCE 3/3: THE BUTTON");

    ref_head("GtkWidget *gtk_button_new_with_label(text)");
    ref_line("Does:", "creates a button showing text.");
    ref_line("Returns:", "a GtkWidget *. Once it is inside a window, the window");
    ref_line("", "owns it. You never free it.");

    ref_head("void gtk_window_set_child(window, widget)");
    ref_line("Does:", "makes widget the window's contents.");
    ref_line("Takes:", "a window holds exactly one child (module 2 shows how to");
    ref_line("", "fit several widgets by putting them in a container first).");
    printf("\n");
    code_bad(LINES(BAD_ORPHAN));
    bad("No error and no message. The button exists, but it is not in the");
    more("window, so nothing appears (read back: the window's child is NULL).");
    more("GTK does not warn about a widget you created and never used.");

    wait_enter();
    clear_screen();
    heading("STEP 3: MAKE THE BUTTON DO SOMETHING");

    printf("  Save this as click.c. Lines marked NEW changed from button.c:\n\n");

    code(LINES(EX_CLICK));

    printf("\n  Compile and run it. Click the button: its text changes from\n");
    printf("  \"Click me\" to \"Clicked!\". That is a complete GTK program:\n");
    printf("  a window, a widget in it, and a function that runs when you\n");
    printf("  use it.\n");

    wait_enter();
    clear_screen();
    heading("REFERENCE 3/3: SIGNALS AND CALLBACKS");

    ref_head("void gtk_button_set_label(button, text)");
    ref_line("Does:", "replaces the text on a button.");
    ref_line("Takes:", "button  a GtkButton *. The callback already receives one,");
    ref_line("", "so no cast is needed inside on_click.");

    ref_head("A callback for \"clicked\": void fn(GtkButton *button, gpointer user_data)");
    ref_line("Rule:", "the parameters must match what the signal passes. For");
    ref_line("", "each widget's signals, GTK's reference at docs.gtk.org");
    ref_line("", "lists them.");
    ref_line("Why:", "G_CALLBACK does not check this. A wrong signature still");
    ref_line("", "compiles, and GTK then calls your function with arguments it");
    ref_line("", "was never written for. That is undefined behaviour, not a");
    ref_line("", "caught error, so this course does not run it.");
    printf("\n");
    code_good(LINES(GOOD_CONNECT));
    code_bad(LINES(BAD_TYPO));
    bad("It compiles. When it runs, GLib prints:");
    more("GLib-GObject-CRITICAL: signal 'clik' is invalid for instance");
    more("... of type 'GtkButton'");
    more("and g_signal_connect returns 0. The callback is never connected, so");
    more("clicking does nothing. If a click does nothing, read the terminal.");

    wait_enter();
    clear_screen();
    heading("GOOD PRACTICE");

    printf("  These habits cost nothing and prevent most of the mistakes\n");
    printf("  above.\n\n");

    code_good(LINES(GOOD_PRACTICE));
    printf("\n");

    tip("Compile with -Wall -Wextra and fix every warning. A missing cast");
    more("is a compile error today, and a warning on older compilers.");
    tip("Run the program from a terminal and read what GTK prints. Every");
    more("CRITICAL line you saw in this module was a call being ignored.");
    tip("Build the window inside activate(), not main(). main() only creates");
    more("the application and hands over to g_application_run().");
    tip("Mark callbacks static (they are private to this file) and name them");
    more("on_<widget>_<event>, so the connection reads like a sentence.");
    tip("Cast unused parameters to void instead of deleting them. The");
    more("signature must match what the signal passes.");
    tip("Create each widget once, in a variable with a clear name. Do not");
    more("write one long function that builds everything.");

    wait_enter();
    clear_screen();
    heading("WHERE GTK3 TUTORIALS WILL MISLEAD YOU");

    printf("  Many tutorials online are for GTK3, whose main() calls\n");
    printf("  gtk_init(&argc, &argv) and then gtk_main(). Against GTK4,\n");
    printf("  gcc says:\n\n");

    printf("    error: too many arguments to function 'gtk_init'\n");
    printf("    error: implicit declaration of function 'gtk_main'\n\n");

    printf("  \"Implicit declaration\" means the function does not exist in\n");
    printf("  the headers you included. It is not a typo. GTK4 programs use\n");
    printf("  GtkApplication and g_application_run(), as in this module.\n\n");

    printf("  Ownership is the other difference from plain C. There is no\n");
    printf("  free(button) or free(window) anywhere above. Widgets are\n");
    printf("  reference counted: gtk_window_set_child() makes the window\n");
    printf("  hold the button, and closing the window lets go of it. You\n");
    printf("  release only what you created yourself, which here is the\n");
    printf("  application, with g_object_unref(app).\n");

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

    question("Which call makes a window you have created actually appear\n"
             "  on screen? (the function name)",
             "gtk_window_present",
             "A new window starts hidden. Without gtk_window_present() it\n"
             "             exists but is never shown, and the program keeps\n"
             "             running with nothing on screen.");

    question("You misspell a signal name in g_signal_connect, for example\n"
             "  \"clik\". Does gcc stop you? (yes or no)",
             "no",
             "The name is only text, so it compiles. At run time GLib\n"
             "             prints a CRITICAL message and g_signal_connect returns\n"
             "             0: the callback is never connected.");

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
    printf("    gcc -Wall -Wextra main.c -o app $(pkg-config --cflags --libs gtk4)\n");

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
