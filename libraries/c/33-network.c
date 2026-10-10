/*
 * TITLE: Network interfaces and local IP address
 * GROUP: System information
 * USES: #include <ifaddrs.h>, <arpa/inet.h>, <net/if.h>
 * SUMMARY: getifaddrs() returns every address of every network interface in
 *   one linked list. inet_ntop turns a binary address into text.
 * NOTES:
 *   - The list has one entry per address, so an interface with an IPv4 and an
 *     IPv6 address appears twice. Check ifa_addr->sa_family (AF_INET or
 *     AF_INET6).
 *   - ifa_addr can be NULL for some entries: test it before reading.
 *   - Skip loopback (IFF_LOOPBACK) and interfaces that are down (no IFF_UP).
 *   - The prefix length (/24) comes from counting the 1 bits of the netmask.
 *   - Free the list with freeifaddrs().
 *   - Which interface the machine really uses for the internet is the one
 *     with the default route: /proc/net/route, the line whose Destination is
 *     00000000.
 *   - This gives LOCAL addresses. A public IP needs an outside service.
 * SEE: man 3 getifaddrs, man 3 inet_ntop
 */

#include <arpa/inet.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

/* The interface of the default route, or an empty string. */
static void default_interface(char *out, size_t size)
{
    FILE *file = fopen("/proc/net/route", "r");
    char line[256];

    out[0] = '\0';
    if (file == NULL) {
        return;
    }
    while (fgets(line, sizeof line, file) != NULL) {
        char name[32];
        unsigned destination;
        if (sscanf(line, "%31s %x", name, &destination) == 2 && destination == 0) {
            snprintf(out, size, "%s", name);
            break;
        }
    }
    fclose(file);
}

int main(void)
{
    struct ifaddrs *list, *item;
    char text[INET6_ADDRSTRLEN];
    char route[32];

    if (getifaddrs(&list) != 0) {
        perror("getifaddrs");
        return 1;
    }
    default_interface(route, sizeof route);
    printf("default route goes out through: %s\n", route[0] ? route : "(none)");

    for (item = list; item != NULL; item = item->ifa_next) {
        if (item->ifa_addr == NULL || (item->ifa_flags & IFF_LOOPBACK) || !(item->ifa_flags & IFF_UP)) {
            continue;
        }
        if (item->ifa_addr->sa_family == AF_INET) {
            struct sockaddr_in *address = (struct sockaddr_in *) item->ifa_addr;
            struct sockaddr_in *mask = (struct sockaddr_in *) item->ifa_netmask;
            int prefix = mask != NULL ? __builtin_popcount(mask->sin_addr.s_addr) : 0;

            inet_ntop(AF_INET, &address->sin_addr, text, sizeof text);
            printf("%-12s IPv4 %s/%d%s\n", item->ifa_name, text, prefix,
                   strcmp(item->ifa_name, route) == 0 ? "  <- default route" : "");
        } else if (item->ifa_addr->sa_family == AF_INET6) {
            struct sockaddr_in6 *address = (struct sockaddr_in6 *) item->ifa_addr;

            inet_ntop(AF_INET6, &address->sin6_addr, text, sizeof text);
            printf("%-12s IPv6 %s\n", item->ifa_name, text);
        }
    }
    freeifaddrs(list);
    return 0;
}
