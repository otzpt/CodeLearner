// Ui.cs - the pieces every lesson uses to draw the screen.
//
// Same purpose and the same visual style as the other courses' ui modules,
// kept as a separate implementation rather than shared code -- see
// docs/writing-a-course.md for why. A static class, same shape as Java's
// Ui, since C# has the identical "no free functions" constraint Java does.

static class Ui
{
    const int Width = 54;

    public static void ClearScreen()
    {
        // ANSI codes, not Process.Start("clear"): no dependency on TERM
        // being set. Same fix, same reason, as every other course's ui
        // module.
        Console.Write("\x1b[H\x1b[2J\x1b[3J");
    }

    public static void WaitEnter()
    {
        Console.Write("\n  Press ENTER to continue...");
        Console.ReadLine();
    }

    public static void Rule() => Console.WriteLine("  " + new string('-', Width));

    static void PaddedLine(string text, char border)
    {
        string padded = text.Length < Width - 3
            ? text + new string(' ', Width - 3 - text.Length)
            : text;
        Console.WriteLine($"  {border} {padded}{border}");
    }

    static void Frame(char fill) => Console.WriteLine("  +" + new string(fill, Width - 2) + "+");

    public static void Title(string text)
    {
        Console.WriteLine();
        Frame('=');
        PaddedLine(text, '|');
        Frame('=');
        Console.WriteLine();
    }

    public static void Heading(string text)
    {
        Console.WriteLine($"\n  {text}");
        Rule();
    }

    public static bool AskYes(string questionText)
    {
        Console.Write($"\n  {questionText} (y/N): ");
        string? answer = Console.ReadLine();
        return !string.IsNullOrEmpty(answer) && (answer[0] == 'y' || answer[0] == 'Y');
    }

    public static void Exercise(int number)
    {
        Console.WriteLine($"\n  >> EXERCISE - MODULE {number}");
        Rule();
    }

    public static bool Question(string text, string correct, string why)
    {
        Console.WriteLine($"\n  {text}");
        Console.Write("  Your answer: ");
        string? answer = Console.ReadLine();

        bool right = string.Equals(answer?.Trim(), correct.Trim(), StringComparison.OrdinalIgnoreCase);
        if (right)
        {
            Console.WriteLine($"\n  CORRECT.  {why}");
        }
        else
        {
            Console.WriteLine($"\n  NOT QUITE. The answer is: {correct}");
            Console.WriteLine($"             {why}");
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
    public static void Challenge(string[] task, string[] input, string[] expected, string[] solution)
    {
        Console.WriteLine("\n  >> WRITE THIS YOURSELF, in a real file");
        Rule();

        foreach (string line in task)
        {
            Console.WriteLine($"  {line}");
        }

        if (input.Length > 0)
        {
            Console.WriteLine("\n  Try it with this input:\n");
            foreach (string line in input)
            {
                Console.WriteLine($"      {line}");
            }
        }

        if (expected.Length > 0)
        {
            Console.WriteLine("\n  It must print:\n");
            foreach (string line in expected)
            {
                Console.WriteLine($"      {line}");
            }
            Console.WriteLine("\n  That output is the whole specification. Any code that");
            Console.WriteLine("  produces it is correct.");
        }

        Console.WriteLine("\n  Try it first. Run with:");
        Console.WriteLine("    dotnet run");

        if (!AskYes("Want to see example code?"))
        {
            return;
        }

        Console.WriteLine();
        Rule();
        foreach (string line in solution)
        {
            Console.WriteLine($"  {line}");
        }
        Rule();
        Console.WriteLine("  This is EXAMPLE CODE, not the answer. It is one way to get");
        Console.WriteLine("  that output; yours may look nothing like it and still be");
        Console.WriteLine("  right -- or better. Compare the output, not the code.");
    }
}
