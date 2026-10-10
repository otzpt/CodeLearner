/*
 * TITLE: sys/utsname.h: kernel, architecture and hostname (uname)
 * GROUP: System information
 * USES: #include <sys/utsname.h>
 * STANDARD: POSIX. The domainname field is a GNU/Linux extension, visible
 *   only with _GNU_SOURCE.
 * SUMMARY: uname() fills a struct with the kernel name, its release, the
 *   machine architecture and the hostname. One call, no files.
 * PROVIDES:
 *   int uname(struct utsname *name)      0, or -1 with errno set
 *   struct utsname:
 *     char sysname[]     "Linux"
 *     char nodename[]    the hostname
 *     char release[]     "6.17.0-5-generic": the kernel version people mean
 *     char version[]     a long build string (number and date)
 *     char machine[]     "x86_64", "aarch64", "riscv64"
 *     char domainname[]  NIS domain, usually "(none)" (needs _GNU_SOURCE)
 *   int gethostname(char *name, size_t size)    (unistd.h) same name as nodename
 * NOTES:
 *   - The strings are fixed-size arrays inside the struct (65 bytes each on
 *     Linux); copy them, do not keep a pointer past the struct's lifetime.
 *   - release is what people mean by "kernel version". machine is the
 *     architecture, and is what a 32-bit program running on a 64-bit kernel
 *     still reports as x86_64.
 *   - nodename is the hostname. gethostname() returns the same name.
 *   - Parsing release: "6.17.0-5-generic" is major.minor.patch-build-flavour.
 *     Use sscanf("%d.%d.%d") to get the numbers; the part after the dash is
 *     the distribution's.
 *   - uname fails only if you pass a bad pointer (EFAULT): in practice it
 *     always works, but check the result anyway.
 * TOOL:
 *   - The "Kernel" line (sysname + release) and the architecture in the OS line.
 *   - The hostname for the user@host title.
 * SEE: man 2 uname, man 2 gethostname
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <sys/utsname.h>
#include <unistd.h>

int main(void)
{
    struct utsname name;
    char host[256];

    if (uname(&name) != 0) {
        perror("uname");
        return 1;
    }
    printf("sysname   %s\n", name.sysname);
    printf("release   %s\n", name.release);
    printf("machine   %s\n", name.machine);
    printf("nodename  %s\n", name.nodename);
    printf("domain    %s\n", name.domainname);
    printf("each field is %zu bytes in the struct\n", sizeof name.sysname);

    int major = 0, minor = 0, patch = 0;
    if (sscanf(name.release, "%d.%d.%d", &major, &minor, &patch) >= 2) {
        printf("release parsed as major %d, minor %d, patch %d\n", major, minor, patch);
    }

    if (gethostname(host, sizeof host) == 0) {
        printf("gethostname() gives the same name: %s\n",
               host[0] != '\0' ? host : "(empty)");
    }
    return 0;
}
