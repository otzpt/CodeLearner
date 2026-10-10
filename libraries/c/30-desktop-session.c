/*
 * TITLE: Desktop environment, window manager and display server
 * GROUP: System information
 * USES: #include <sys/socket.h>, <sys/un.h>, <stdlib.h> (getenv)
 * SUMMARY: The desktop and session type are environment variables. The window
 *   manager (compositor on Wayland) is found by asking who owns the Wayland
 *   socket.
 * NOTES:
 *   - $XDG_CURRENT_DESKTOP is the desktop ("GNOME", "KDE", "ubuntu:GNOME": a
 *     colon-separated list, most specific first). $DESKTOP_SESSION is the
 *     session name, which login managers set.
 *   - $XDG_SESSION_TYPE is "wayland", "x11" or "tty". $WAYLAND_DISPLAY (often
 *     "wayland-0") is set under Wayland, $DISPLAY (":0") under X11 or XWayland.
 *   - Wayland: connect to $XDG_RUNTIME_DIR/$WAYLAND_DISPLAY and ask the
 *     kernel who is on the other end with getsockopt(SO_PEERCRED). That pid is
 *     the compositor; /proc/<pid>/comm names it (gnome-shell, kwin_wayland,
 *     sway, Hyprland).
 *   - X11: the window manager is named by a property on the root window
 *     (_NET_SUPPORTING_WM_CHECK, then _NET_WM_NAME). That needs Xlib or xcb.
 *   - Variables are missing over SSH and inside cron. Print "(not set)"; do not
 *     treat it as an error.
 * SEE: man 7 unix, man 7 credentials
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

static const char *env_or(const char *name, const char *fallback)
{
    const char *value = getenv(name);
    return value != NULL && value[0] != '\0' ? value : fallback;
}

/* The pid at the other end of the Wayland socket, or -1. */
static int wayland_compositor_pid(void)
{
    const char *runtime = getenv("XDG_RUNTIME_DIR");
    const char *display = env_or("WAYLAND_DISPLAY", "wayland-0");
    struct sockaddr_un address = { .sun_family = AF_UNIX };
    struct ucred peer;
    socklen_t length = sizeof peer;
    int descriptor;
    int pid = -1;

    if (runtime == NULL
        || snprintf(address.sun_path, sizeof address.sun_path, "%s/%s", runtime, display)
               >= (int) sizeof address.sun_path) {
        return -1;
    }
    descriptor = socket(AF_UNIX, SOCK_STREAM, 0);
    if (descriptor < 0) {
        return -1;
    }
    if (connect(descriptor, (struct sockaddr *) &address, sizeof address) == 0
        && getsockopt(descriptor, SOL_SOCKET, SO_PEERCRED, &peer, &length) == 0) {
        pid = (int) peer.pid;
    }
    close(descriptor);
    return pid;
}

int main(void)
{
    printf("XDG_CURRENT_DESKTOP  %s\n", env_or("XDG_CURRENT_DESKTOP", "(not set)"));
    printf("DESKTOP_SESSION      %s\n", env_or("DESKTOP_SESSION", "(not set)"));
    printf("XDG_SESSION_TYPE     %s\n", env_or("XDG_SESSION_TYPE", "(not set)"));
    printf("WAYLAND_DISPLAY      %s\n", env_or("WAYLAND_DISPLAY", "(not set)"));
    printf("DISPLAY              %s\n", env_or("DISPLAY", "(not set)"));

    int pid = wayland_compositor_pid();
    if (pid > 0) {
        char path[64];
        char name[64] = "";
        FILE *file;

        snprintf(path, sizeof path, "/proc/%d/comm", pid);
        file = fopen(path, "r");
        if (file != NULL) {
            if (fgets(name, sizeof name, file) != NULL) {
                name[strcspn(name, "\n")] = '\0';
            }
            fclose(file);
        }
        printf("Wayland compositor   %s (pid %d)\n", name, pid);
    } else {
        puts("Wayland compositor   (no Wayland socket reachable)");
    }
    return 0;
}
