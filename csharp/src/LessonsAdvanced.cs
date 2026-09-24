// LessonsAdvanced.cs - module 11 onward.

using System.Diagnostics;
using System.Threading;

static class LessonsAdvanced
{
    public static void Lesson11Linq()
    {
        Ui.Title("MODULE 11 - LINQ");

        Ui.Heading("PART 1: the loop LINQ replaces");

        Console.WriteLine("  Filtering and transforming a list, the way module 6 already");
        Console.WriteLine("  would:");
        Console.WriteLine();
        Console.WriteLine("    List<int> nums = new() { 1, 2, 3, 4, 5, 6 };");
        Console.WriteLine("    List<int> evenSquares = new();");
        Console.WriteLine("    foreach (int n in nums)");
        Console.WriteLine("    {");
        Console.WriteLine("        if (n % 2 == 0) evenSquares.Add(n * n);");
        Console.WriteLine("    }");
        Console.WriteLine();

        List<int> nums = new() { 1, 2, 3, 4, 5, 6 };
        List<int> evenSquares = new();
        foreach (int n in nums)
        {
            if (n % 2 == 0) evenSquares.Add(n * n);
        }
        Console.WriteLine($"  Running: {string.Join(", ", evenSquares)}");
        Console.WriteLine();
        Console.WriteLine("  The same result, one line, with LINQ's method syntax:");
        Console.WriteLine();
        Console.WriteLine("    var evenSquares2 = nums.Where(n => n % 2 == 0)");
        Console.WriteLine("                            .Select(n => n * n);");
        Console.WriteLine();

        var evenSquares2 = nums.Where(n => n % 2 == 0).Select(n => n * n);
        Console.WriteLine($"  Running: {string.Join(", ", evenSquares2)}");
        Console.WriteLine();
        Console.WriteLine("  Where filters, Select transforms -- the same two steps the");
        Console.WriteLine("  loop did explicitly, chained instead of nested.");

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Heading("PART 2: aggregates -- Sum, Count, OrderBy");

        Console.WriteLine("  LINQ also replaces the loops that reduce a list to one value:");
        Console.WriteLine();
        Console.WriteLine("    int total = nums.Sum();");
        Console.WriteLine("    int countEven = nums.Count(n => n % 2 == 0);");
        Console.WriteLine("    var sorted = nums.OrderByDescending(n => n);");
        Console.WriteLine();

        int total = nums.Sum();
        int countEven = nums.Count(n => n % 2 == 0);
        var sorted = nums.OrderByDescending(n => n);
        Console.WriteLine($"  Running: total={total}, countEven={countEven}, sorted={string.Join(", ", sorted)}");

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Heading("PART 3: LINQ is lazy -- a query doesn't run until you enumerate it");

        Console.WriteLine("  var query = nums.Where(n => n % 2 == 0); does not filter");
        Console.WriteLine("  anything yet -- it builds a plan, run only when something");
        Console.WriteLine("  actually iterates it: a foreach, or .ToList()/.Count()/etc.");
        Console.WriteLine();
        Console.WriteLine("  Proof: add to the list AFTER building the query, BEFORE");
        Console.WriteLine("  enumerating it, and the addition is included:");
        Console.WriteLine();
        Console.WriteLine("    List<int> data = new() { 1, 2, 3 };");
        Console.WriteLine("    var query = data.Where(n => n > 1);   // not run yet");
        Console.WriteLine("    data.Add(4);                          // still not run");
        Console.WriteLine("    foreach (int n in query) { ... }      // NOW it runs");
        Console.WriteLine();

        List<int> data = new() { 1, 2, 3 };
        var query = data.Where(n => n > 1);
        data.Add(4);
        Console.WriteLine($"  Running: {string.Join(", ", query)}");
        Console.WriteLine();
        Console.WriteLine("  4 is in the result, even though it was added after the query");
        Console.WriteLine("  variable was created -- the query re-runs over whatever `data`");
        Console.WriteLine("  contains at enumeration time, not at query-creation time.");
        Console.WriteLine("  .ToList() forces it to run immediately instead, once, capturing");
        Console.WriteLine("  the results as they are right then.");

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Exercise(11);

        Ui.Question(
            "nums.Where(n => n > 0) -- does this run immediately, or only\n" +
            "  when something enumerates it?  (answer: immediately or later)",
            "later",
            "LINQ query methods are deferred (lazy): building the query does nothing " +
            "until a foreach, ToList(), Count(), or similar actually walks it.");

        Ui.Question(
            "nums.Where(n => n % 2 == 0).Select(n => n * n) -- which\n" +
            "  comes first, the filtering or the transforming?",
            "filtering",
            "Where runs first in the chain shown; Select only ever sees the values that " +
            "already passed the Where.");

        Ui.Question(
            "var q = list.Where(...); list.Add(x); foreach (var v in\n" +
            "  q). Is the added x included in what the foreach sees?\n" +
            "  (answer: yes or no)",
            "yes",
            "Because the query is lazy, it re-evaluates over list's current contents at " +
            "enumeration time -- including anything added after the query was built.");

        Ui.Challenge(
            task: new[]
            {
                "Given List<int> nums = new() { 3, 7, 2, 9, 4 }, use LINQ to",
                "print the sum of only the values greater than 3.",
            },
            input: Array.Empty<string>(),
            expected: new[] { "20" },
            solution: new[]
            {
                "List<int> nums = new() { 3, 7, 2, 9, 4 };",
                "int sum = nums.Where(n => n > 3).Sum();",
                "Console.WriteLine(sum);",
            });

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Heading("SUMMARY");

        Console.WriteLine("  - Where filters, Select transforms -- chain them instead of nesting loops");
        Console.WriteLine("  - Sum, Count, OrderBy(Descending) replace the loops that reduce a list");
        Console.WriteLine("    to one value or a sorted copy");
        Console.WriteLine("  - a LINQ query is lazy: it runs at enumeration time, not creation time --");
        Console.WriteLine("    .ToList() forces it to run once, immediately, if that's what you want");
        Console.WriteLine();
        Console.WriteLine("  Module 12 is async/await -- compared directly against JavaScript's");
        Console.WriteLine("  own Promise-based version of the same idea.");
        Ui.WaitEnter();
    }

    public static async Task Lesson12Async()
    {
        Ui.Title("MODULE 12 - ASYNC/AWAIT");

        Ui.Heading("PART 1: Task -- the value that doesn't exist yet");

        Console.WriteLine("  A Task is C#'s equivalent of a JavaScript Promise: a value that");
        Console.WriteLine("  doesn't exist yet. It starts pending and later either completes");
        Console.WriteLine("  with a result or faults with an exception -- once, ever, the same");
        Console.WriteLine("  two-outcome shape a Promise has.");
        Console.WriteLine();
        Console.WriteLine("  Task.Delay(100) and Thread.Sleep(100) both \"wait 100ms\", but not");
        Console.WriteLine("  the same way. Thread.Sleep blocks the calling thread until the");
        Console.WriteLine("  time is up -- nothing else on that thread runs meanwhile.");
        Console.WriteLine("  Task.Delay hands back a Task immediately; the 100ms happens on");
        Console.WriteLine("  its own, and only awaiting that Task actually waits for it:");
        Console.WriteLine();
        Console.WriteLine("    Stopwatch sw = Stopwatch.StartNew();");
        Console.WriteLine("    Task delayTask = Task.Delay(100);   // returns immediately");
        Console.WriteLine("    await delayTask;                    // NOW it actually waits");
        Console.WriteLine();

        Stopwatch sw = Stopwatch.StartNew();
        Console.WriteLine($"  Running: [{sw.ElapsedMilliseconds}ms] calling Task.Delay(100)...");
        Task delayTask = Task.Delay(100);
        Console.WriteLine($"  Running: [{sw.ElapsedMilliseconds}ms] Task.Delay already returned -- the 100ms hasn't happened yet");
        await delayTask;
        Console.WriteLine($"  Running: [{sw.ElapsedMilliseconds}ms] now it has -- awaiting is what did the actual waiting");
        Console.WriteLine();

        sw.Restart();
        Console.WriteLine($"  Running: [{sw.ElapsedMilliseconds}ms] calling Thread.Sleep(100)...");
        Thread.Sleep(100);
        Console.WriteLine($"  Running: [{sw.ElapsedMilliseconds}ms] Thread.Sleep only returned once the full 100ms had passed");
        Console.WriteLine();
        Console.WriteLine("  Same 100ms wait, two different costs: Task.Delay frees the thread");
        Console.WriteLine("  to do other work while it waits; Thread.Sleep ties the thread up");
        Console.WriteLine("  for the whole duration.");

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Heading("PART 2: async/await syntax, and why it's contagious upward");

        Console.WriteLine("  A method that awaits something inside itself must be marked");
        Console.WriteLine("  async, and its return type wraps in Task -- Task<int> instead of");
        Console.WriteLine("  int, or plain Task instead of void for one that returns nothing:");
        Console.WriteLine();
        Console.WriteLine("    static async Task<int> ComputeAsync()");
        Console.WriteLine("    {");
        Console.WriteLine("        await Task.Delay(50);");
        Console.WriteLine("        return 42;");
        Console.WriteLine("    }");
        Console.WriteLine();
        Console.WriteLine("  Calling it is where \"contagious\" shows up: the caller can't just");
        Console.WriteLine("  write int x = ComputeAsync();  -- ComputeAsync() returns a");
        Console.WriteLine("  Task<int>, not an int, so the caller must await it too:");
        Console.WriteLine();
        Console.WriteLine("    int result = await ComputeAsync();");
        Console.WriteLine();
        Console.WriteLine("  That await forces the CALLER's own method to be async as well,");
        Console.WriteLine("  which forces its own caller to be async, and so on, all the way");
        Console.WriteLine("  up to the entry point -- one await at the bottom of a call chain");
        Console.WriteLine("  pushes async onto every method above it. JavaScript's own");
        Console.WriteLine("  async/await follows the identical rule.");
        Console.WriteLine();
        Console.WriteLine("  This course's own launcher (Program.cs) is the one exception --");
        Console.WriteLine("  it calls this method with .GetAwaiter().GetResult() instead of");
        Console.WriteLine("  await, deliberately blocking to stay a plain, synchronous Action.");
        Console.WriteLine("  That is a safe escape hatch in a console app with no captured");
        Console.WriteLine("  SynchronizationContext -- the same call in a GUI or ASP.NET app");
        Console.WriteLine("  can deadlock, which is why \"just block on it\" is not the default");
        Console.WriteLine("  advice, contagion up to a real async entry point is.");
        Console.WriteLine();

        int result = await ComputeAsync();
        Console.WriteLine($"  Running: {result}");

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Heading("PART 3: Task.WhenAll -- independent awaits, run together");

        Console.WriteLine("  Two awaits that don't depend on each other still pay for the gap");
        Console.WriteLine("  between them when awaited one after another:");
        Console.WriteLine();
        Console.WriteLine("    string a = await DelayedValue(\"a\", 100);   // waits 100ms");
        Console.WriteLine("    string b = await DelayedValue(\"b\", 100);   // THEN waits another 100ms");
        Console.WriteLine();
        Console.WriteLine("  Task.WhenAll starts every Task passed to it immediately and waits");
        Console.WriteLine("  for whichever finishes last, once:");
        Console.WriteLine();
        Console.WriteLine("    string[] results = await Task.WhenAll(");
        Console.WriteLine("        DelayedValue(\"a\", 100),");
        Console.WriteLine("        DelayedValue(\"b\", 100));");
        Console.WriteLine();

        sw.Restart();
        string a = await DelayedValue("a", 100);
        string b = await DelayedValue("b", 100);
        long sequentialMs = sw.ElapsedMilliseconds;
        Console.WriteLine($"  Running: sequential got [{a}, {b}] in {(sequentialMs < 250 ? "~200" : sequentialMs.ToString())}ms");

        sw.Restart();
        string[] results = await Task.WhenAll(DelayedValue("a", 100), DelayedValue("b", 100));
        long concurrentMs = sw.ElapsedMilliseconds;
        Console.WriteLine($"  Running: concurrent got [{string.Join(", ", results)}] in {(concurrentMs < 180 ? "~100" : concurrentMs.ToString())}ms");
        Console.WriteLine();
        Console.WriteLine("  Same two 100ms delays, roughly half the wall-clock time -- both");
        Console.WriteLine("  ran at the same time instead of one after the other.");

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Heading("PART 4: an awaited faulted Task throws -- no new mechanism");

        Console.WriteLine("  A Task that faults (its body threw) behaves exactly like a");
        Console.WriteLine("  rejected Promise: awaiting it re-throws the same exception,");
        Console.WriteLine("  catchable with the ordinary try/catch module 10 already covers:");
        Console.WriteLine();
        Console.WriteLine("    static async Task FailAsync()");
        Console.WriteLine("    {");
        Console.WriteLine("        await Task.Delay(10);");
        Console.WriteLine("        throw new InvalidOperationException(\"boom\");");
        Console.WriteLine("    }");
        Console.WriteLine();
        Console.WriteLine("    try");
        Console.WriteLine("    {");
        Console.WriteLine("        await FailAsync();");
        Console.WriteLine("    }");
        Console.WriteLine("    catch (InvalidOperationException e)");
        Console.WriteLine("    {");
        Console.WriteLine("        Console.WriteLine($\"caught: {e.Message}\");");
        Console.WriteLine("    }");
        Console.WriteLine();

        try
        {
            await FailAsync();
        }
        catch (InvalidOperationException e)
        {
            Console.WriteLine($"  Running: caught: {e.Message}");
        }

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Exercise(12);

        Ui.Question(
            "Task.Delay(100) is called but not yet awaited -- the call\n" +
            "  returns almost instantly. Does the 100ms wait happen before\n" +
            "  that return, or after it?  (answer: before or after)",
            "after",
            "Task.Delay hands back a Task representing work already in progress; the wait " +
            "itself finishes independently of when the call returns -- await is what makes " +
            "the calling code actually wait for it.");

        Ui.Question(
            "ComputeAsync() has an await inside it, so it's marked async\n" +
            "  and returns Task<int>. Can its caller call it like an\n" +
            "  ordinary method -- no await, no async on the caller -- and\n" +
            "  still get useful work done?  (answer: yes or no)",
            "no",
            "await is contagious upward: whatever calls an async method must itself await it " +
            "(or otherwise block on the Task) instead of calling it like an ordinary method -- " +
            "this repeats up the call chain to the entry point.");

        Ui.Question(
            "Two independent 100ms delays, awaited one after another,\n" +
            "  take about how long in total -- 100ms or 200ms?",
            "200ms",
            "Sequential awaits wait fully for the first before even starting the second. " +
            "Task.WhenAll runs them concurrently instead.");

        Ui.Challenge(
            task: new[]
            {
                "Write an async method Task<int> DoubleAsync(int n) that",
                "awaits Task.Delay(10) and then returns n * 2. Await it with",
                "21 and print the result.",
            },
            input: Array.Empty<string>(),
            expected: new[] { "42" },
            solution: new[]
            {
                "static async Task<int> DoubleAsync(int n)",
                "{",
                "    await Task.Delay(10);",
                "    return n * 2;",
                "}",
                "",
                "int result = await DoubleAsync(21);",
                "Console.WriteLine(result);",
            });

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Heading("SUMMARY");

        Console.WriteLine("  - a Task is pending, then completes or faults once -- the same shape");
        Console.WriteLine("    a JavaScript Promise has, under a different name");
        Console.WriteLine("  - Task.Delay doesn't block the thread the way Thread.Sleep does; only");
        Console.WriteLine("    awaiting it actually waits");
        Console.WriteLine("  - await is contagious: a method that awaits must be async, and so");
        Console.WriteLine("    must every caller above it, up to the entry point");
        Console.WriteLine("  - Task.WhenAll runs independent awaits concurrently instead of one");
        Console.WriteLine("    after another");
        Console.WriteLine("  - an awaited faulted Task throws, caught with the same try/catch");
        Console.WriteLine("    module 10 already taught -- no new mechanism");
        Console.WriteLine();
        Console.WriteLine("  Module 13 is generics and interfaces in depth -- a generic method");
        Console.WriteLine("  with a real constraint, and default interface methods, past what");
        Console.WriteLine("  module 8's records and structs already touched.");
        Ui.WaitEnter();

        static async Task<int> ComputeAsync()
        {
            await Task.Delay(50);
            return 42;
        }

        static async Task<string> DelayedValue(string tag, int ms)
        {
            await Task.Delay(ms);
            return tag;
        }

        static async Task FailAsync()
        {
            await Task.Delay(10);
            throw new InvalidOperationException("boom");
        }
    }

    public static void Lesson13GenericsInterfaces()
    {
        Ui.Title("MODULE 13 - GENERICS AND INTERFACES IN DEPTH");

        Ui.Heading("PART 1: an unconstrained T can't do anything type-specific");

        Console.WriteLine("  Module 6's List<int> is a generic type someone else already");
        Console.WriteLine("  wrote; <T> in your own code means the compiler only knows T");
        Console.WriteLine("  could be ANY type -- not that it has any particular method:");
        Console.WriteLine();
        Console.WriteLine("    static T LargerOf<T>(T a, T b)");
        Console.WriteLine("    {");
        Console.WriteLine("        if (a.CompareTo(b) >= 0) return a;");
        Console.WriteLine("        return b;");
        Console.WriteLine("    }");
        Console.WriteLine();
        Console.WriteLine("  This does not compile -- verified with a real dotnet build, not");
        Console.WriteLine("  quoted from memory:");
        Console.WriteLine();
        Console.WriteLine("    error CS7036: There is no argument given that corresponds to the");
        Console.WriteLine("    required parameter 'comparisonType' of");
        Console.WriteLine("    'MemoryExtensions.CompareTo(ReadOnlySpan<char>, ReadOnlySpan<char>,");
        Console.WriteLine("    StringComparison)'");
        Console.WriteLine();
        Console.WriteLine("  That error is not \"T has no CompareTo\" -- it's subtler than that.");
        Console.WriteLine("  System.MemoryExtensions.CompareTo is an extension method always in");
        Console.WriteLine("  scope in this project, so an unconstrained a.CompareTo(b) doesn't");
        Console.WriteLine("  fail to find a candidate; the compiler reaches for that unrelated");
        Console.WriteLine("  3-argument extension method and then complains it's missing the");
        Console.WriteLine("  third argument. A student reading \"CompareTo\" in the error will");
        Console.WriteLine("  naturally assume it's about their own call, not realize the");
        Console.WriteLine("  compiler resolved to a completely different method with the same");
        Console.WriteLine("  name.");
        Console.WriteLine();
        Console.WriteLine("  T could be a Random object -- CompareTo isn't guaranteed to exist");
        Console.WriteLine("  on it. A constraint tells the compiler what T is guaranteed");
        Console.WriteLine("  to have, in exchange for only accepting types that have it:");
        Console.WriteLine();
        Console.WriteLine("    static T LargerOf<T>(T a, T b) where T : IComparable<T>");
        Console.WriteLine("    {");
        Console.WriteLine("        if (a.CompareTo(b) >= 0) return a;");
        Console.WriteLine("        return b;");
        Console.WriteLine("    }");
        Console.WriteLine();
        Console.WriteLine("  int and string both implement IComparable<T> in .NET -- one a");
        Console.WriteLine("  value type, one a reference type, both satisfy the same");
        Console.WriteLine("  constraint:");
        Console.WriteLine();

        Console.WriteLine($"  Running: LargerOf(3, 9)              -> {LargerOf(3, 9)}");
        Console.WriteLine($"  Running: LargerOf(\"apple\", \"banana\") -> {LargerOf("apple", "banana")}");

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Heading("PART 2: default interface methods -- an interface method with a body");

        Console.WriteLine("  Before C# 8, every interface method was a bare signature -- no");
        Console.WriteLine("  body allowed, exactly like the interfaces this course's own Java");
        Console.WriteLine("  module 9 teaches (interface Shape { double area(); }, and leaving");
        Console.WriteLine("  area() unwritten refuses to compile). C# 8 added the option of");
        Console.WriteLine("  giving an interface method a body -- a DEFAULT implementation,");
        Console.WriteLine("  inherited by any implementing class that doesn't write its own:");
        Console.WriteLine();
        Console.WriteLine("    interface IGreeter");
        Console.WriteLine("    {");
        Console.WriteLine("        string Name { get; }");
        Console.WriteLine("        void Greet() => Console.WriteLine($\"Hi, I'm {Name}.\");");
        Console.WriteLine("    }");
        Console.WriteLine();
        Console.WriteLine("    class QuietRobot : IGreeter { public string Name => \"R2\"; }");
        Console.WriteLine("    class LoudRobot : IGreeter");
        Console.WriteLine("    {");
        Console.WriteLine("        public string Name => \"Bender\";");
        Console.WriteLine("        public void Greet() => Console.WriteLine($\"{Name.ToUpper()}!!!\");");
        Console.WriteLine("    }");
        Console.WriteLine();
        Console.WriteLine("  QuietRobot never writes Greet() at all -- it just inherits");
        Console.WriteLine("  IGreeter's default. LoudRobot writes its own, which replaces");
        Console.WriteLine("  the default instead of running alongside it:");
        Console.WriteLine();

        IGreeter g1 = new QuietRobot();
        IGreeter g2 = new LoudRobot();
        Console.Write("  Running: ");
        g1.Greet();
        Console.Write("  Running: ");
        g2.Greet();
        Console.WriteLine();
        Console.WriteLine("  The trap: a default interface method is only reachable through an");
        Console.WriteLine("  interface-typed reference, not through the concrete class, even");
        Console.WriteLine("  though QuietRobot IS an IGreeter:");
        Console.WriteLine();
        Console.WriteLine("    QuietRobot r = new QuietRobot();");
        Console.WriteLine("    r.Greet();   // does NOT compile");
        Console.WriteLine();
        Console.WriteLine("    error CS1061: 'QuietRobot' does not contain a definition for");
        Console.WriteLine("    'Greet' and no accessible extension method 'Greet' accepting a");
        Console.WriteLine("    first argument of type 'QuietRobot' could be found (are you");
        Console.WriteLine("    missing a using directive or an assembly reference?)");
        Console.WriteLine();
        Console.WriteLine("  A default body only resolves through an interface-typed reference,");
        Console.WriteLine("  not through the concrete class, even though the class implements");
        Console.WriteLine("  it -- that's a lookup rule at the call site, not a change to how");
        Console.WriteLine("  dispatch works: g2.Greet() above still ran LoudRobot's own");
        Console.WriteLine("  override, resolved off the real object at runtime like any other");
        Console.WriteLine("  virtual call. Real Java has had the identical feature since Java 8; this");
        Console.WriteLine("  course's own Java module 9 has simply never exercised that side");
        Console.WriteLine("  of it, only the pure-contract half where every method needs an");
        Console.WriteLine("  implementation.");

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Exercise(13);

        Ui.Question(
            "static T Foo<T>(T a, T b) { return a.CompareTo(b) > 0 ? a\n" +
            "  : b; } -- with no constraint on T, does this compile?\n" +
            "  (answer: yes or no)",
            "no",
            "The compiler only knows T could be any type; CompareTo isn't guaranteed to exist " +
            "on it without a constraint like where T : IComparable<T> telling it otherwise.");

        Ui.Question(
            "An interface method in C# 8+ is given a body. Must every\n" +
            "  class implementing that interface still write its own\n" +
            "  version of that method?  (answer: yes or no)",
            "no",
            "A method with a default body is inherited as-is unless the implementing class " +
            "overrides it -- only members left with no body still force an implementation.");

        Ui.Question(
            "where T : IComparable<T> -- int satisfies that constraint.\n" +
            "  Does string satisfy the same constraint?  (answer: yes or no)",
            "yes",
            "The constraint only requires T to implement IComparable<T> -- it doesn't care " +
            "whether T is a value type like int or a reference type like string.");

        Ui.Challenge(
            task: new[]
            {
                "Write a generic method T Max<T>(T a, T b), constrained with",
                "where T : IComparable<T>, that returns the larger of the two",
                "values using CompareTo. Call it with 7 and 3 and print the result.",
            },
            input: Array.Empty<string>(),
            expected: new[] { "7" },
            solution: new[]
            {
                "static T Max<T>(T a, T b) where T : IComparable<T>",
                "{",
                "    if (a.CompareTo(b) >= 0) return a;",
                "    return b;",
                "}",
                "",
                "Console.WriteLine(Max(7, 3));",
            });

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Heading("SUMMARY");

        Console.WriteLine("  - an unconstrained T only has what every type has (Object's own");
        Console.WriteLine("    members) -- CompareTo needs where T : IComparable<T> first");
        Console.WriteLine("  - that constraint accepts any type implementing IComparable<T>,");
        Console.WriteLine("    value type or reference type alike -- int and string both qualify");
        Console.WriteLine("  - since C# 8, an interface method may have a body -- a default");
        Console.WriteLine("    implementation, inherited unless the implementing class overrides it");
        Console.WriteLine("  - a default body only resolves through an interface-typed reference,");
        Console.WriteLine("    not through the concrete class, even though the class implements it");
        Console.WriteLine("  - real Java has had default methods since Java 8 too; this course's");
        Console.WriteLine("    own Java module 9 only ever shows the pure-contract half");
        Console.WriteLine();
        Console.WriteLine("  Module 14 is delegates and events -- passing behavior itself as a");
        Console.WriteLine("  value, and the one trap that makes event its own keyword instead");
        Console.WriteLine("  of a plain delegate field.");
        Ui.WaitEnter();

        static T LargerOf<T>(T a, T b) where T : IComparable<T>
        {
            if (a.CompareTo(b) >= 0) return a;
            return b;
        }
    }

    public static void Lesson14DelegatesEvents()
    {
        Ui.Title("MODULE 14 - DELEGATES AND EVENTS");

        Ui.Heading("PART 1: behavior as a value -- Action<T> and Func<T,R>");

        Console.WriteLine("  This course's own launcher already does this. Program.cs's");
        Console.WriteLine("  module table holds a method as a value, to call later:");
        Console.WriteLine();
        Console.WriteLine("    var modules = new (string Title, Action Run, string Tier)[]");
        Console.WriteLine();
        Console.WriteLine("  Action is a delegate type built into .NET: a reference to a");
        Console.WriteLine("  method (or lambda) that takes some arguments and returns void.");
        Console.WriteLine("  Func is the same idea for a method that returns a value -- its");
        Console.WriteLine("  last type parameter is the return type:");
        Console.WriteLine();
        Console.WriteLine("    static void RunTwice(Action<string> action)");
        Console.WriteLine("    {");
        Console.WriteLine("        action(\"first call\");");
        Console.WriteLine("        action(\"second call\");");
        Console.WriteLine("    }");
        Console.WriteLine();
        Console.WriteLine("    RunTwice(msg => Console.WriteLine($\"  Running: {msg}\"));");
        Console.WriteLine();

        RunTwice(msg => Console.WriteLine($"  Running: {msg}"));
        Console.WriteLine();
        Console.WriteLine("  RunTwice never mentions Console.WriteLine -- it only knows it");
        Console.WriteLine("  received something callable with a string. The caller decides");
        Console.WriteLine("  WHAT happens; RunTwice only decides WHEN and HOW OFTEN.");
        Console.WriteLine();
        Console.WriteLine("    static int ApplyTwice(Func<int, int> f, int x) => f(f(x));");
        Console.WriteLine();

        Console.WriteLine($"  Running: ApplyTwice(n => n * 2, 3) = {ApplyTwice(n => n * 2, 3)}");

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Heading("PART 2: delegate -- a named signature, for when Action/Func aren't enough");

        Console.WriteLine("  Action<T> and Func<T,R> cover almost everything, but a named");
        Console.WriteLine("  delegate type documents intent better than a generic");
        Console.WriteLine("  instantiation -- and, as PART 3 shows, is required to declare an");
        Console.WriteLine("  event at all:");
        Console.WriteLine();
        Console.WriteLine("    delegate string Transform(string input);");
        Console.WriteLine();
        Console.WriteLine("    static string ApplyTransform(Transform t, string input) => t(input);");
        Console.WriteLine();
        Console.WriteLine("    Transform shout = s => s.ToUpper() + \"!\";");
        Console.WriteLine("    Console.WriteLine(ApplyTransform(shout, \"hello\"));");
        Console.WriteLine();

        Transform shout = s => s.ToUpper() + "!";
        Console.WriteLine($"  Running: {ApplyTransform(shout, "hello")}");
        Console.WriteLine();
        Console.WriteLine("  Transform and Func<string, string> describe the exact same");
        Console.WriteLine("  shape -- one string in, one string out. delegate buys a name");
        Console.WriteLine("  (Transform) instead of a generic instantiation, nothing more,");
        Console.WriteLine("  until PART 3.");

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Heading("PART 3: event -- outside code cannot overwrite it the way it could a field");

        Console.WriteLine("  A plain delegate FIELD is just a variable holding a callable --");
        Console.WriteLine("  any code with a reference to the object can replace it outright:");
        Console.WriteLine();
        Console.WriteLine("    public Transform OnFire;      // a field");
        Console.WriteLine("    thing.OnFire = whatever;      // wipes out any earlier subscriber");
        Console.WriteLine();
        Console.WriteLine("  event restricts outside code to += and -= only -- never a plain");
        Console.WriteLine("  assignment, never calling it directly:");
        Console.WriteLine();
        Console.WriteLine("    class Counter");
        Console.WriteLine("    {");
        Console.WriteLine("        public delegate void CounterChanged(int newValue);");
        Console.WriteLine("        public event CounterChanged? OnValueChanged;");
        Console.WriteLine();
        Console.WriteLine("        int value;");
        Console.WriteLine("        public void Increment()");
        Console.WriteLine("        {");
        Console.WriteLine("            value++;");
        Console.WriteLine("            OnValueChanged?.Invoke(value);");
        Console.WriteLine("        }");
        Console.WriteLine("    }");
        Console.WriteLine();
        Console.WriteLine("  ?.Invoke is module 9's null-conditional operator again: with no");
        Console.WriteLine("  subscriber, OnValueChanged is null, and calling a null delegate");
        Console.WriteLine("  directly throws NullReferenceException -- ?.Invoke skips the");
        Console.WriteLine("  call instead of crashing the first time nobody happens to be");
        Console.WriteLine("  listening yet.");
        Console.WriteLine();

        var counter = new Counter();
        counter.OnValueChanged += v => Console.WriteLine($"  Running: value is now {v}");
        counter.Increment();
        counter.Increment();
        Console.WriteLine();
        Console.WriteLine("  counter.OnValueChanged = null; from outside this method really");
        Console.WriteLine("  does not compile -- verified with a real dotnet build, not");
        Console.WriteLine("  quoted from memory:");
        Console.WriteLine();
        Console.WriteLine("    error CS0070: The event 'Counter.OnValueChanged' can only");
        Console.WriteLine("    appear on the left hand side of += or -= (except when used");
        Console.WriteLine("    from within the type 'Counter')");
        Console.WriteLine();
        Console.WriteLine("  A plain field would have let that assignment through silently,");
        Console.WriteLine("  wiping out every earlier subscriber with no error at all --");
        Console.WriteLine("  event turns that mistake into a compile error instead.");

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Heading("PART 4: -= removes exactly the handler += added");

        Console.WriteLine("  Subtracting a handler removes only that handler; once the last");
        Console.WriteLine("  one is gone, Increment's ?.Invoke has nothing left to call:");
        Console.WriteLine();
        Console.WriteLine("    Counter.CounterChanged handler = v => Console.WriteLine($\"tick {v}\");");
        Console.WriteLine("    counter2.OnValueChanged += handler;");
        Console.WriteLine("    counter2.Increment();      // handler runs");
        Console.WriteLine("    counter2.OnValueChanged -= handler;");
        Console.WriteLine("    counter2.Increment();      // handler is gone, nothing prints");
        Console.WriteLine();

        var counter2 = new Counter();
        Counter.CounterChanged handler = v => Console.WriteLine($"  Running: tick {v}");
        counter2.OnValueChanged += handler;
        counter2.Increment();
        counter2.OnValueChanged -= handler;
        Console.WriteLine("  Running: incrementing again after -=...");
        counter2.Increment();
        Console.WriteLine("  Running: ...and nothing printed above this line from the handler");

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Exercise(14);

        Ui.Question(
            "RunTwice(msg => Console.WriteLine(msg)) passes a lambda to a\n" +
            "  method expecting Action<string>. Is RunTwice given the RESULT\n" +
            "  of running that lambda, or the lambda ITSELF, to call later?\n" +
            "  (answer: result or itself)",
            "itself",
            "Action<T> and Func<T,R> hold a reference to the method or lambda, not a value it " +
            "already produced -- the receiving method decides if and when to call it.");

        Ui.Question(
            "A method's signature would read clearer with its own name\n" +
            "  than as Func<string, string> everywhere it's used. Which\n" +
            "  keyword names a signature like that? (answer: delegate)",
            "delegate",
            "delegate string Transform(string input); names that exact shape once; Action/Func " +
            "describe the same shape generically, with no name of their own.");

        Ui.Question(
            "counter.OnValueChanged is declared with event, not as a\n" +
            "  plain delegate field. From OUTSIDE the Counter class, can\n" +
            "  that code assign counter.OnValueChanged = null;?\n" +
            "  (answer: yes or no)",
            "no",
            "event restricts outside code to += and -= only; a plain field would have let " +
            "that assignment silently wipe out every subscriber instead.");

        Ui.Challenge(
            task: new[]
            {
                "Using the Counter class from PART 3 (no new type needed),",
                "create one, subscribe two different lambda handlers to its",
                "OnValueChanged event -- one printing \"A saw {v}\", the other",
                "\"B saw {v}\" -- then call Increment() once. Both handlers",
                "fire from that single raise of the event.",
            },
            input: Array.Empty<string>(),
            expected: new[] { "A saw 1", "B saw 1" },
            solution: new[]
            {
                "var counter = new Counter();",
                "counter.OnValueChanged += v => Console.WriteLine($\"A saw {v}\");",
                "counter.OnValueChanged += v => Console.WriteLine($\"B saw {v}\");",
                "counter.Increment();",
            });

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Heading("SUMMARY");

        Console.WriteLine("  - Action<T> and Func<T,R> pass behavior itself as a value -- a");
        Console.WriteLine("    method or lambda held by reference, called whenever the receiver decides");
        Console.WriteLine("  - delegate names a signature Action/Func would otherwise describe");
        Console.WriteLine("    generically -- and is what an event's type must be");
        Console.WriteLine("  - a plain delegate field can be reassigned or nulled by any code holding a");
        Console.WriteLine("    reference to it; event restricts outside code to += and -= only");
        Console.WriteLine("  - ?.Invoke skips the call when nobody has subscribed yet, instead of");
        Console.WriteLine("    throwing NullReferenceException on a null delegate");
        Ui.WaitEnter();

        static void RunTwice(Action<string> action)
        {
            action("first call");
            action("second call");
        }

        static int ApplyTwice(Func<int, int> f, int x) => f(f(x));

        static string ApplyTransform(Transform t, string input) => t(input);
    }

    delegate string Transform(string input);

    class Counter
    {
        public delegate void CounterChanged(int newValue);
        public event CounterChanged? OnValueChanged;

        int value;
        public void Increment()
        {
            value++;
            OnValueChanged?.Invoke(value);
        }
    }

    interface IGreeter
    {
        string Name { get; }
        void Greet() => Console.WriteLine($"Hi, I'm {Name}.");
    }

    class QuietRobot : IGreeter
    {
        public string Name => "R2";
    }

    class LoudRobot : IGreeter
    {
        public string Name => "Bender";
        public void Greet() => Console.WriteLine($"{Name.ToUpper()}!!!");
    }
}
