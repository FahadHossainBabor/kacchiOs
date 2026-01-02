/* kernel.c - Main kernel with scheduler, memory manager, and process manager */
#include "types.h"
#include "serial.h"
#include "string.h"
#include "scheduler.h"
#include "memory.h"
#include "process.h"

#define MAX_INPUT 128

/* Simple utility to print unsigned integer */
static void print_u32(uint32_t v) {
    char buf[12];
    int pos = 0;
    if (v == 0) { serial_putc('0'); return; }
    while (v) {
        buf[pos++] = '0' + (v % 10);
        v /= 10;
    }
    while (pos--) serial_putc(buf[pos]);
}

/* Example task A: prints a message and yields */
void task_a(void) {
    while (1) {
        serial_puts("[task A] running (ticks=");
        print_u32(sched_get_ticks());
        serial_puts(")\n");
        sleep_ticks(2);
    }
}

/* Example task B: prints and yields */
void task_b(void) {
    while (1) {
        serial_puts("[task B] hello\n");
        sleep_ticks(3);
    }
}

void kmain(void) {
    char input[MAX_INPUT];
    int pos = 0;
    
    /* Initialize hardware and managers */
    serial_init();

    /* Welcome */
    serial_puts("\n");
    serial_puts("========================================\n");
    serial_puts("    kacchiOS - Minimal Baremetal OS\n");
    serial_puts("========================================\n");
    serial_puts("Hello from kacchiOS!\n");
    serial_puts("Initializing managers...\n\n");

    /* Initialize memory manager (64KB heap at 0x00100000) */
    mem_init(0x00100000, 65536);

    /* Initialize process manager */
    proc_init();

    /* Initialize scheduler and create demo tasks */
    sched_init();
    create_task(task_a, 1);
    create_task(task_b, 1);

    serial_puts("Running null process (CLI). Type 'ps', 'plist', 'mem', 'memdump', 'help'\n");

    /* Main loop - the null process (cooperative) */
    while (1) {
        serial_puts("kacchiOS> ");
        pos = 0;

        /* Read input line */
        while (1) {
            char c = serial_getc();
            if (c == '\r' || c == '\n') {
                input[pos] = '\0';
                serial_puts("\n");
                break;
            } else if ((c == '\b' || c == 0x7F) && pos > 0) {
                pos--;
                serial_puts("\b \b");
            } else if (c >= 32 && c < 127 && pos < MAX_INPUT - 1) {
                input[pos++] = c;
                serial_putc(c);
            }
        }

        if (pos > 0) {
            if (strcmp(input, "ps") == 0) {
                sched_ps();
            } else if (strcmp(input, "plist") == 0) {
                proc_list();
            } else if (strcmp(input, "mem") == 0) {
                mem_stats();
            } else if (strcmp(input, "memdump") == 0) {
                mem_dump();
            } else if (strcmp(input, "clear") == 0) {
                serial_clear();
            } else if (strcmp(input, "yield") == 0) {
                yield();
            } else if (strcmp(input, "help") == 0) {
                serial_puts("Commands: ps, plist, mem, memdump, clear, yield, help\n");
            } else {
                serial_puts("You typed: ");
                serial_puts(input);
                serial_puts("\n");
            }
        }

        /* Cooperative point: allow scheduler to run other tasks */
        yield();
    }

    for (;;) { __asm__ volatile ("hlt"); }
}