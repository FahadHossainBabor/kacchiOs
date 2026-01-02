/* scheduler.c - Cooperative round-robin scheduler */
#include "scheduler.h"
#include "serial.h"
#include "string.h"
#include "types.h"

/* Minimal PCB */
typedef struct pcb {
    uint32_t *esp;            /* saved stack pointer */
    uint8_t stack[STACK_SIZE];
    int pid;
    task_state_t state;
    int priority;
    uint32_t wake_tick;
} pcb_t;

static pcb_t pcbs[MAX_TASKS];
static int current = 0; /* index of current running task */
static int next_pid = 1;
static uint32_t ticks = 0;

/* extern assembly context switch */
extern void context_switch(uint32_t **old_sp, uint32_t *new_sp);

static uint32_t* get_esp(void) {
    uint32_t* sp;
    __asm__ volatile ("movl %%esp, %0" : "=r"(sp));
    return sp;
}

void sched_init(void) {
    int i;
    for (i = 0; i < MAX_TASKS; i++) {
        pcbs[i].esp = NULL;
        pcbs[i].pid = 0;
        pcbs[i].state = TASK_FREE;
        pcbs[i].priority = 0;
        pcbs[i].wake_tick = 0;
    }

    /* Set up null process (pid 0) to capture current kernel stack */
    pcbs[0].pid = 0;
    pcbs[0].esp = get_esp();
    pcbs[0].state = TASK_RUNNING;
    pcbs[0].priority = 0;
    current = 0;
}

int create_task(task_fn_t fn, int priority) {
    int i;
    for (i = 1; i < MAX_TASKS; i++) {
        if (pcbs[i].state == TASK_FREE) break;
    }
    if (i == MAX_TASKS) return -1;

    pcbs[i].pid = next_pid++;
    pcbs[i].state = TASK_READY;
    pcbs[i].priority = priority;
    pcbs[i].wake_tick = 0;

    /* Prepare initial stack for new task
       Layout: [EDI][ESI][EBP][ESP][EBX][EDX][ECX][EAX][EIP]
       We set registers to 0 and EIP to task entry. */
    uint32_t *stk_top = (uint32_t*)(pcbs[i].stack + STACK_SIZE);
    uint32_t *stk = stk_top;

    *(--stk) = (uint32_t)fn; /* initial return address -> EIP */
    *(--stk) = 0; /* EAX */
    *(--stk) = 0; /* ECX */
    *(--stk) = 0; /* EDX */
    *(--stk) = 0; /* EBX */
    *(--stk) = 0; /* ESP (ignored) */
    *(--stk) = 0; /* EBP */
    *(--stk) = 0; /* ESI */
    *(--stk) = 0; /* EDI */

    pcbs[i].esp = stk;
    return pcbs[i].pid;
}

/* Choose next runnable task (simple priority + round-robin) */
static int pick_next(void) {
    int best = -1;
    int best_prio = -1000000;
    int i;
    for (i = 0; i < MAX_TASKS; i++) {
        int idx = (current + 1 + i) % MAX_TASKS;
        if (pcbs[idx].state == TASK_READY && pcbs[idx].wake_tick <= ticks) {
            if (pcbs[idx].priority > best_prio) {
                best_prio = pcbs[idx].priority;
                best = idx;
            }
        }
    }
    return best;
}

void yield(void) {
    /* advance ticks (simulated) */
    ticks++;

    int nxt = pick_next();
    if (nxt < 0) {
        /* no ready task, continue current (null process) */
        return;
    }

    int prev = current;
    current = nxt;
    pcbs[current].state = TASK_RUNNING;
    pcbs[prev].state = (pcbs[prev].state == TASK_RUNNING) ? TASK_READY : pcbs[prev].state;

    context_switch(&pcbs[prev].esp, pcbs[current].esp);
}

void exit_task(void) {
    pcbs[current].state = TASK_ZOMBIE;
    /* find next runnable */
    int nxt = pick_next();
    if (nxt < 0) {
        /* switch back to null process */
        nxt = 0;
    }
    int prev = current;
    current = nxt;
    pcbs[current].state = TASK_RUNNING;
    context_switch(&pcbs[prev].esp, pcbs[current].esp);
}

void sleep_ticks(uint32_t t) {
    pcbs[current].wake_tick = ticks + t;
    pcbs[current].state = TASK_BLOCKED;
    int nxt = pick_next();
    if (nxt < 0) nxt = 0;
    int prev = current;
    current = nxt;
    pcbs[current].state = TASK_RUNNING;
    context_switch(&pcbs[prev].esp, pcbs[current].esp);
}

/* Print small integer */
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

void sched_ps(void) {
    serial_puts("PID\tSTATE\tPRIO\tWAKE\n");
    int i;
    for (i = 0; i < MAX_TASKS; i++) {
        if (pcbs[i].state != TASK_FREE) {
            print_u32(pcbs[i].pid);
            serial_puts("\t");
            switch (pcbs[i].state) {
                case TASK_RUNNING: serial_puts("RUN   \t"); break;
                case TASK_READY: serial_puts("READY \t"); break;
                case TASK_BLOCKED: serial_puts("BLOCK \t"); break;
                case TASK_ZOMBIE: serial_puts("ZOMBIE\t"); break;
                default: serial_puts("FREE  \t"); break;
            }
            print_u32(pcbs[i].priority);
            serial_puts("\t");
            print_u32(pcbs[i].wake_tick);
            serial_puts("\n");
        }
    }
}

uint32_t sched_get_ticks(void) { return ticks; }
