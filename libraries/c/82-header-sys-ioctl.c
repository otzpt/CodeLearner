/*
 * TITLE: sys/ioctl.h: device control requests (terminal size, bytes waiting)
 * GROUP: Header reference: signals and terminals
 * USES: #include <sys/ioctl.h>
 * STANDARD: Not ISO C, and POSIX only defines ioctl for STREAMS. In practice
 *   it is a Unix de facto interface, and the REQUEST CODES are Linux-specific.
 *   Other systems have similar names with different numbers.
 * SUMMARY: ioctl(fd, request, argument) is the catch-all call for things
 *   that read and write do not cover: asking a terminal its size, asking a
 *   socket for an address, asking a pipe how many bytes are waiting.
 * PROVIDES:
 *   int ioctl(int fd, unsigned long request, ...)     0 or a value, -1 on error
 *   TIOCGWINSZ  terminal size into struct winsize (ws_row, ws_col,
 *               ws_xpixel, ws_ypixel)
 *   FIONREAD    bytes waiting to be read, into an int
 *   SIOCGIFADDR, SIOCGIFNETMASK, SIOCGIFHWADDR, SIOCGIFFLAGS
 *               network interface address, mask, MAC address, flags (also need
 *               struct ifreq from <net/if.h>)
 *   TIOCGPGRP, TIOCSCTTY ...   process groups and controlling terminals
 * NOTES:
 *   - The request decides what the third argument must be (a pointer to a
 *     struct winsize, to an int, to a struct ifreq). Passing the wrong type
 *     compiles and corrupts memory.
 *   - Returns -1 with errno ENOTTY if the descriptor is not that kind of
 *     device: TIOCGWINSZ on a pipe or a file fails this way. That is the
 *     expected case when output is redirected, not an error.
 *   - ws_xpixel and ws_ypixel are 0 on many terminals.
 *   - ioctl numbers are encoded differently on some architectures: always use
 *     the macro names, never the numbers.
 *   - Where a normal function exists, prefer it: getifaddrs() over the
 *     SIOCGIF requests, tcgetattr() over the terminal ioctls.
 * TOOL:
 *   - TIOCGWINSZ for the terminal width, so the logo is dropped on a narrow
 *     terminal (try stdout, then stdin, then /dev/tty).
 *   - SIOCGIFHWADDR for an interface's MAC address (getifaddrs also reports it,
 *     as an AF_PACKET entry).
 * SEE: man 2 ioctl, man 4 tty_ioctl, man 7 netdevice
 */

#include <errno.h>
#include <net/if.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

int main(void)
{
    struct winsize size;
    int channel[2];
    int waiting = 0;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) == 0) {
        printf("stdout terminal: %d columns x %d rows\n", size.ws_col, size.ws_row);
    } else {
        printf("TIOCGWINSZ on stdout: -1, errno %d = %s\n", errno, strerror(errno));
    }

    if (pipe(channel) == 0) {
        if (write(channel[1], "hello", 5) != 5) {
            return 1;
        }
        ioctl(channel[0], FIONREAD, &waiting);
        printf("FIONREAD on a pipe holding \"hello\": %d bytes waiting\n", waiting);
        close(channel[0]);
        close(channel[1]);
    }

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock >= 0) {
        struct if_nameindex *interfaces = if_nameindex();

        for (struct if_nameindex *item = interfaces; item != NULL && item->if_name != NULL; item++) {
            struct ifreq request;

            if (strcmp(item->if_name, "lo") == 0) {
                continue;
            }
            memset(&request, 0, sizeof request);
            snprintf(request.ifr_name, sizeof request.ifr_name, "%s", item->if_name);
            if (ioctl(sock, SIOCGIFHWADDR, &request) == 0) {
                const unsigned char *mac = (const unsigned char *) request.ifr_hwaddr.sa_data;
                printf("SIOCGIFHWADDR %s: %02x:%02x:%02x:%02x:%02x:%02x\n", item->if_name, mac[0], mac[1], mac[2],
                       mac[3], mac[4], mac[5]);
                break;
            }
        }
        if_freenameindex(interfaces);
        close(sock);
    }
    return 0;
}
