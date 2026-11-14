// The Security course runs its examples in Python, the same way the Python
// course does.

const python = require("../python/course.js");

module.exports = {
  ...python,
  id: "security",
  title: "SECURITY FROM ZERO",
  titlePt: "SEGURANÇA DO ZERO",
  errors: [
    [/OperationalError|sqlite3\./, "the database refused the query: often text that changed the meaning of the SQL.", "a base de dados recusou a consulta: muitas vezes texto que mudou o significado do SQL."],
    [/PermissionError/, "the program refused, or was refused, access to that file.", "o programa recusou, ou foi-lhe recusado, o acesso a esse ficheiro."],
    ...python.errors,
  ],
};
