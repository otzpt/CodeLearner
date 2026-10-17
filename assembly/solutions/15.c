/* ---- above kmain: a decimal printer, built from the pieces already there ---- */

static void print_dec(uint64_t value, uint8_t attr)
{
    char reversed[21];
    char text[21];
    int n = 0;

    do {                                    /* the last digit first, as in module 4 */
        reversed[n++] = (char)('0' + value % 10);
        value /= 10;
    } while (value != 0);

    for (int i = 0; i < n; i++)             /* then turn it around */
        text[i] = reversed[n - 1 - i];
    text[n] = '\0';
    print(text, attr);
}

/* ---- inside kmain, before the final for (;;) ---- */

    print("dec = ", 0x07);
    print_dec(123456789, 0x0E);
    print("\n", 0x07);
