/* In idt.c, in interrupt_handler, the branch for vector 32 (the timer): */

    if (f->vector == 32) {
        ticks++;
        if (ticks % 100 == 0)       /* the timer runs at 100 Hz: once a second */
            print(".", 0x0E);
    }
