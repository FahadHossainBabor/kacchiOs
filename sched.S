.section .text
.global context_switch
/* context_switch(uint32_t **old_sp, uint32_t *new_sp)
   ABI: args at 4(%esp) and 8(%esp) on entry.
   Save caller registers with pusha, store the resulting esp into *old_sp,
   switch to new_sp, then pop registers from the new stack and return. */
context_switch:
    /* load arguments into registers before pushing regs */
    movl 4(%esp), %eax    /* old_sp (pointer to saved esp) */
    movl 8(%esp), %edx    /* new_sp (value) */
    pusha                 /* push general registers onto current stack */
    movl %esp, (%eax)     /* store pointer to saved register block */
    movl %edx, %esp       /* switch to new stack */
    popa                  /* restore registers from new stack */
    ret

