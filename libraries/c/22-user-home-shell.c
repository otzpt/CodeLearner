/*
 * TITLE: pwd.h: user name, home directory and login shell
 * GROUP: System information
 * USES: #include <pwd.h>, <unistd.h>
 * STANDARD: POSIX. getpwuid_r and getpwnam_r are the thread-safe forms;
 *   getpwent and friends walk the whole database.
 * SUMMARY: getuid() says who is running the program; getpwuid_r() looks that
 *   user up in the account database and returns the name, the home directory
 *   and the login shell. The environment variables are the quick route.
 * PROVIDES:
 *   struct passwd: pw_name, pw_uid, pw_gid, pw_gecos (full name), pw_dir (home),
 *                  pw_shell (login shell), pw_passwd (usually "x")
 *   int getpwuid_r(uid, &pwd, buf, buflen, &result)     look up by user id
 *   int getpwnam_r(name, &pwd, buf, buflen, &result)    look up by name
 *   struct passwd *getpwuid(uid), getpwnam(name)        simple but not thread-safe
 *   setpwent(), getpwent(), endpwent()    walk all accounts
 *   uid_t getuid(void), geteuid(void)    (unistd.h)
 * NOTES:
 *   - $USER, $HOME and $SHELL are convenient but can be changed or missing
 *     (cron, sudo, containers). getpwuid is the reliable answer.
 *   - pw_shell and $SHELL are the user's LOGIN shell. The shell you are
 *     typing in right now may be another one: see the terminal and shell page.
 *   - Use the _r function; the plain getpwuid returns a pointer to static
 *     memory that the next call overwrites.
 *   - The _r functions return 0 even when the user does not exist: look at
 *     result. It is NULL when nothing matched. A non-zero return is a real
 *     error (ERANGE: the buffer was too small, so grow it).
 *   - Some accounts are looked up in LDAP or other services, not just
 *     /etc/passwd: do not read that file yourself.
 *   - getlogin() needs a controlling terminal and often fails: avoid it.
 *   - pw_gecos holds the full name first, then comma separated extras.
 * TOOL:
 *   - The user name and home directory for the title and for finding config.
 *   - pw_shell for the "Shell" line when $SHELL is not set.
 * SEE: man 3 getpwuid, man 5 passwd, man 7 environ
 */

#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    struct passwd account;
    struct passwd *found = NULL;
    char buffer[1024];

    if (getpwuid_r(getuid(), &account, buffer, sizeof buffer, &found) != 0 || found == NULL) {
        fprintf(stderr, "no account entry for uid %d\n", (int) getuid());
        return 1;
    }
    printf("uid           %d\n", (int) getuid());
    printf("pw_name       %s\n", found->pw_name);
    printf("pw_dir        %s\n", found->pw_dir);
    printf("pw_shell      %s\n", found->pw_shell);

    const char *shell = getenv("SHELL");
    printf("$SHELL        %s\n", shell != NULL ? shell : "(not set)");

    struct passwd root;
    struct passwd *result = NULL;
    int status = getpwnam_r("root", &root, buffer, sizeof buffer, &result);
    printf("getpwnam_r(\"root\"): status %d, uid %d, home %s\n", status, result ? (int) result->pw_uid : -1,
           result ? result->pw_dir : "?");

    status = getpwnam_r("no-such-user-here", &root, buffer, sizeof buffer, &result);
    printf("getpwnam_r(\"no-such-user-here\"): status %d, result is %s\n", status, result ? "set" : "NULL (no such user)");
    return 0;
}
