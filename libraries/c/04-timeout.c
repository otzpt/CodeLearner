/*
 * TITLE: poll(2): waiting for input with a timeout
 * GROUP: Common tasks
 * USES: #include <poll.h>
 * SUMMARY: A read blocks until data comes, however long that takes. poll waits
 *   on a descriptor for a limited time, so you can give up instead.
 * SIGNATURE:
 *   int poll(struct pollfd *fds, nfds_t count, int timeout_ms)
 * PARAMETERS:
 *   - fds, count: the descriptors to watch. Set .fd and .events in each one,
 *     usually POLLIN for "there is something to read".
 *   - timeout_ms: milliseconds to wait. 0 returns at once, -1 waits for ever.
 * RETURNS: How many descriptors are ready, 0 if the time ran out, or -1 on
 *   failure. The kernel writes the result for each one to its .revents.
 * ERRORS:
 *   - EINTR: a signal arrived while waiting. Usually you call poll again.
 *   - EINVAL: count is too large, or the timeout is not valid.
 * NOTES:
 *   - It works on anything with a descriptor: pipes, sockets, terminals.
 *   - After poll says POLLIN, a read will not block. Regular files always
 *     count as ready.
 *   - Check .revents for POLLHUP too: the other end has gone.
 *   - Time with CLOCK_MONOTONIC, because the wall clock can jump.
 * USEDFOR:
 *   - A network client that gives up on a silent server.
 *   - Waiting on several connections at once.
 * SEE: man 2 poll, man 2 select, man 7 signal
 */

#include <poll.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

static long milliseconds_since(const struct timespec *start)
{
    struct timespec now;

    clock_gettime(CLOCK_MONOTONIC, &now);
    return (now.tv_sec - start->tv_sec) * 1000 + (now.tv_nsec - start->tv_nsec) / 1000000;
}

int main(void)
{
    int ends[2];
    struct timespec start;

    if (pipe(ends) != 0) {
        perror("pipe");
        return 1;
    }

    struct pollfd watch = { .fd = ends[0], .events = POLLIN };

    clock_gettime(CLOCK_MONOTONIC, &start);
    int ready = poll(&watch, 1, 200);
    printf("nothing written yet: poll returned %d after about %ld ms\n", ready,
           milliseconds_since(&start));

    const char *message = "hello";
    if (write(ends[1], message, strlen(message)) < 0) {
        perror("write");
        return 1;
    }

    clock_gettime(CLOCK_MONOTONIC, &start);
    ready = poll(&watch, 1, 200);
    printf("data is waiting:     poll returned %d after about %ld ms\n", ready,
           milliseconds_since(&start));

    if (ready == 1 && (watch.revents & POLLIN)) {
        char text[16];
        ssize_t got = read(ends[0], text, sizeof text - 1);

        if (got >= 0) {
            text[got] = '\0';
            printf("read returned at once: \"%s\"\n", text);
        }
    }

    close(ends[0]);
    close(ends[1]);
    return 0;
}
