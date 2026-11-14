// A tiny database every example in this lesson starts with.
const SETUP = `import sqlite3

db = sqlite3.connect(":memory:")
db.execute("CREATE TABLE users (name TEXT, password TEXT)")
db.execute("INSERT INTO users VALUES ('ana', 'x1'), ('rui', 'x2')")`;

module.exports = {
  id: "sql-injection",
  title: "SQL injection",
  titlePt: "Injeção de SQL",
  screens: [
    [
      "Databases are asked questions in a language called SQL. sqlite3 comes with Python and keeps a whole database in a file, or here in memory. Every example on the next screens starts with this:",
      { code: SETUP + '\n\nfor row in db.execute("SELECT name FROM users"):\n    print(row)' },
    ],
    [
      "The mistake: building the SQL by joining in text the user typed. Here a login that should find one user:",
      { setup: SETUP, setupLabel: "(the database setup from the previous screen goes here)", code: "def find(name):\n    query = f\"SELECT name FROM users WHERE name = '{name}'\"\n    return db.execute(query).fetchall()\n\nprint(find(\"ana\"))\nprint(find(\"x' OR '1'='1\"))" },
      "The second \"name\" closes the quote and adds OR '1'='1', which is always true, so it returns every user. The user's text changed what the query means. This is SQL injection.",
    ],
    [
      "The fix: never put user text into the SQL. Write ? where a value goes, and pass the values separately. The database treats them as plain values, never as SQL:",
      { setup: SETUP, setupLabel: "(the database setup from the previous screen goes here)", code: 'def find(name):\n    return db.execute("SELECT name FROM users WHERE name = ?", (name,)).fetchall()\n\nprint(find("ana"))\nprint(find("x\' OR \'1\'=\'1"))' },
      "(name,) is a tuple with one value. The attack now just searches for a user with that odd name, and finds none.",
      { quiz: "What stops SQL injection?", choices: ["removing quotes from the input", "? placeholders with the values passed separately"], answer: 2, why: "Placeholders keep values out of the SQL itself." },
    ],
    [
      {
        exercise: "sql",
        task: "Using the same users table (ana/x1, rui/x2), write login(name, password) that returns True only if a row matches both, using ? placeholders. Print login(\"ana\", \"x1\"), login(\"ana\", \"x2\") and login(\"ana\", \"' OR '1'='1\").",
        expected: ["True", "False", "False"],
        starter: SETUP + "\n\n# your code here",
        hints: ['db.execute("SELECT 1 FROM users WHERE name = ? AND password = ?", (name, password))', ".fetchone() is None when nothing matched."],
        solution: SETUP + '\n\ndef login(name, password):\n    row = db.execute("SELECT 1 FROM users WHERE name = ? AND password = ?", (name, password)).fetchone()\n    return row is not None\n\nprint(login("ana", "x1"))\nprint(login("ana", "x2"))\nprint(login("ana", "\' OR \'1\'=\'1"))',
      },
    ],
  ],
  recapPt: [
    "Juntar texto do utilizador ao SQL deixa esse texto mudar a consulta.",
    "Usa sempre ? e passa os valores à parte: db.execute(sql, (valor,)).",
    "Os valores passados assim nunca são lidos como SQL.",
  ],
  recap: [
    "Joining user text into SQL lets that text change the query.",
    "Always use ? and pass values separately: db.execute(sql, (value,)).",
    "Values passed that way are never read as SQL.",
  ],
};
