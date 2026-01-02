/* process.c - Process manager implementation */
#include "process.h"
#include "memory.h"
#include "serial.h"
#include "string.h"

#define PROC_STACK_SIZE 2048

static process_t procs[MAX_PROCESSES];
static int next_pid = 1;
static int current_pid = 0;

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

void proc_init(void) {
    int i, j;
    for (i = 0; i < MAX_PROCESSES; i++) {
        procs[i].pid = -1;
        procs[i].ppid = -1;
        procs[i].state = PROC_FREE;
        procs[i].exit_code = 0;
        procs[i].child_count = 0;
        procs[i].stack = NULL;
        procs[i].esp = NULL;
        procs[i].cpu_ticks = 0;
        
        for (j = 0; j < MAX_CHILDREN; j++) {
            procs[i].children[j] = -1;
        }
        for (j = 0; j < MAX_SIGNALS; j++) {
            procs[i].signal_handlers[j] = NULL;
        }
    }

    /* Process 0: kernel/init */
    procs[0].pid = 0;
    procs[0].ppid = -1;
    procs[0].state = PROC_RUNNING;
    current_pid = 0;

    serial_puts("[PROC] Manager initialized\n");
}

int proc_create(int ppid) {
    int i;
    for (i = 1; i < MAX_PROCESSES; i++) {
        if (procs[i].state == PROC_FREE) break;
    }
    if (i == MAX_PROCESSES) return -1; /* No free slots */

    int pid = next_pid++;
    process_t *p = &procs[i];
    
    p->pid = pid;
    p->ppid = ppid;
    p->state = PROC_CREATED;
    p->exit_code = 0;
    p->child_count = 0;
    p->cpu_ticks = 0;

    /* Allocate stack */
    p->stack = (uint8_t*)malloc(PROC_STACK_SIZE);
    if (!p->stack) return -1;

    /* Initialize stack (simple: ESP points to top) */
    p->esp = (uint32_t*)((uint8_t*)p->stack + PROC_STACK_SIZE);

    /* Register as child of parent */
    if (ppid >= 0 && ppid < MAX_PROCESSES && procs[ppid].pid == ppid) {
        process_t *parent = &procs[ppid];
        if (parent->child_count < MAX_CHILDREN) {
            parent->children[parent->child_count++] = pid;
        }
    }

    serial_puts("[PROC] Created pid=");
    print_u32(pid);
    serial_puts(" ppid=");
    print_u32(ppid);
    serial_puts("\n");

    return pid;
}

int proc_wait(int pid, int *exit_code) {
    int i;
    for (i = 0; i < MAX_PROCESSES; i++) {
        if (procs[i].pid == pid) {
            if (procs[i].state == PROC_ZOMBIE) {
                if (exit_code) *exit_code = procs[i].exit_code;
                procs[i].state = PROC_FREE;
                procs[i].pid = -1;
                return 0;
            }
            return -1; /* Still running or invalid */
        }
    }
    return -1; /* Not found */
}

void proc_signal_register(int sig, void (*handler)(int)) {
    if (sig < 0 || sig >= MAX_SIGNALS) return;
    procs[current_pid].signal_handlers[sig] = handler;
}

int proc_signal_send(int pid, int sig) {
    int i;
    for (i = 0; i < MAX_PROCESSES; i++) {
        if (procs[i].pid == pid) {
            if (sig < 0 || sig >= MAX_SIGNALS) return -1;
            if (procs[i].signal_handlers[sig]) {
                procs[i].signal_handlers[sig](sig);
                return 0;
            }
            return -1;
        }
    }
    return -1; /* Process not found */
}

void proc_exit(int code) {
    procs[current_pid].exit_code = code;
    procs[current_pid].state = PROC_ZOMBIE;

    /* Free stack */
    if (procs[current_pid].stack) {
        free(procs[current_pid].stack);
        procs[current_pid].stack = NULL;
    }

    serial_puts("[PROC] Process ");
    print_u32(procs[current_pid].pid);
    serial_puts(" exited with code ");
    print_u32(code);
    serial_puts("\n");
}

int proc_getpid(void) {
    return procs[current_pid].pid;
}

int proc_getppid(void) {
    return procs[current_pid].ppid;
}

void proc_list(void) {
    serial_puts("PID\tPPID\tSTATE\t\tCPU\n");
    int i;
    for (i = 0; i < MAX_PROCESSES; i++) {
        if (procs[i].state != PROC_FREE) {
            print_u32(procs[i].pid);
            serial_puts("\t");
            print_u32(procs[i].ppid);
            serial_puts("\t");
            
            switch (procs[i].state) {
                case PROC_CREATED: serial_puts("CREATED\t"); break;
                case PROC_RUNNING: serial_puts("RUNNING\t"); break;
                case PROC_BLOCKED: serial_puts("BLOCKED\t"); break;
                case PROC_ZOMBIE: serial_puts("ZOMBIE\t"); break;
                default: serial_puts("UNKNOWN\t"); break;
            }
            
            print_u32(procs[i].cpu_ticks);
            serial_puts("\n");
        }
    }
}

process_t* proc_get(int pid) {
    int i;
    for (i = 0; i < MAX_PROCESSES; i++) {
        if (procs[i].pid == pid) {
            return &procs[i];
        }
    }
    return NULL;
}
