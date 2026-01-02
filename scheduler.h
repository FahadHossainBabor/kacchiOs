/* scheduler.h - Simple cooperative scheduler API */
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "types.h"

#define MAX_TASKS 16
#define STACK_SIZE 4096

typedef enum {
    TASK_FREE = 0,
    TASK_RUNNING,
    TASK_READY,
    TASK_BLOCKED,
    TASK_ZOMBIE
} task_state_t;

typedef void (*task_fn_t)(void);

void sched_init(void);
int create_task(task_fn_t fn, int priority);
void yield(void);
void exit_task(void);
void sleep_ticks(uint32_t ticks);
void sched_ps(void);

/* Expose ticks for tests/inspections */
uint32_t sched_get_ticks(void);

#endif
