/* free.c - C with no libc: the only way out is a system call. */

static long sys_write(int fd, const char *buf, unsigned long len)
{
    long ret;
    __asm__ volatile ("syscall"
                      : "=a"(ret)
                      : "a"(1L), "D"((long)fd), "S"(buf), "d"(len)
                      : "rcx", "r11", "memory");
    return ret;
}

static unsigned long length(const char *s)
{
    unsigned long n = 0;
    while (s[n])
        n++;
    return n;
}

int c_main(void)
{
    const char msg[] = "hello from C, with no libc\n";
    sys_write(1, msg, length(msg));
    return 7;
}
