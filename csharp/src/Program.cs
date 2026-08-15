// C# course - main menu.
//
// Written in C#, the same way every other course here is written in the
// language it teaches. Top-level statements (no explicit class/Main
// wrapper) on purpose, not just for brevity -- this file demonstrates
// module 1's own subject. MODULES is an array of a small record holding a
// title and an Action, C#'s answer to Java's Runnable and C's function
// pointer table.

var modules = new (string Title, Action Run)[]
{
    ("Compiling and printing",              LessonsBasics.Lesson01Running),
    ("Variables and types",                 LessonsBasics.Lesson02Variables),
    ("Reading input",                       LessonsBasics.Lesson03Input),
    ("Conditions and pattern matching",     LessonsBasics.Lesson04Conditions),
    ("Loops",                               LessonsBasics.Lesson05Loops),
    ("Arrays and List<T>",                  LessonsMore.Lesson06Collections),
    ("Methods and default parameters",      LessonsMore.Lesson07Methods),
    ("Classes, structs, and records",       LessonsMore.Lesson08ClassesStructs),
    ("Nullable reference types",            LessonsMore.Lesson09Nullable),
    ("Exceptions and IDisposable",          LessonsMore.Lesson10Exceptions),
};

ShowMenu();

while (true)
{
    Console.Write("\n  Pick a module: ");
    string? choice = Console.ReadLine();

    if (choice is null || choice.Trim() == "0")
    {
        break;
    }

    if (int.TryParse(choice.Trim(), out int n) && n >= 1 && n <= modules.Length)
    {
        Ui.ClearScreen();
        // A module that lets an exception escape should not end the
        // session with a raw stack trace -- every other exit path here
        // prints something and returns to this menu.
        try
        {
            modules[n - 1].Run();
        }
        catch (Exception e)
        {
            Console.WriteLine($"\n  Something went wrong: {e.Message}");
            Ui.WaitEnter();
        }
    }
    else
    {
        Console.WriteLine("\n  Not a valid option.");
        Ui.WaitEnter();
    }

    ShowMenu();
}

Console.WriteLine("\n  See you next time.\n");

void ShowMenu()
{
    Ui.ClearScreen();
    Ui.Title("C# COURSE - FROM TOP-LEVEL STATEMENTS TO IDISPOSABLE");

    for (int i = 0; i < modules.Length; i++)
    {
        Console.WriteLine($"   [{i + 1,2}]  {modules[i].Title}");
    }
    Console.WriteLine("\n   [ 0]  Quit");
    Ui.Rule();
}
