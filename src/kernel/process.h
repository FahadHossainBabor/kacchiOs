/* process.h - Process manager */
#ifndef PROCESS_H
#define PROCESS_H

#include "types.h"

#define MAX_PROCESSES 32
#define MAX_CHILDREN 8
#define MAX_SIGNALS 16

/* Process states */
typedef enum {
    PROC_FREE = 0,
    PROC_CREATED,
    PROC_RUNNING,
    PROC_BLOCKED,
    PROC_ZOMBIE
} proc_state_t;

/* Process structure */
typedef struct process {
    int pid;
    int ppid;               /* parent pid */
    proc_state_t state;
    int exit_code;
    
    /* Children tracking */
    int children[MAX_CHILDREN];
    int child_count;
    
    /* Signal handling */
    void (*signal_handlers[MAX_SIGNALS])(int);
    
    /* Resource tracking */
    uint32_t *esp;          /* stack pointer (for scheduler) */
    uint8_t *stack;         /* process stack */
    
    /* Accounting */
    uint32_t cpu_ticks;
} process_t;

/* Initialize process manager */
void proc_init(void);

/* Create a new process */
int proc_create(int ppid);

/* Wait for child process */
int proc_wait(int pid, int *exit_code);

/* Register signal handler */
void proc_signal_register(int sig, void (*handler)(int));

/* Send signal to process */
int proc_signal_send(int pid, int sig);

/* Exit current process */
void proc_exit(int code);

/* Get current process pid */
int proc_getpid(void);

/* Get parent pid of current process */
int proc_getppid(void);

/* List all processes */
void proc_list(void);

/* Get process info */
process_t* proc_get(int pid);

#endif
