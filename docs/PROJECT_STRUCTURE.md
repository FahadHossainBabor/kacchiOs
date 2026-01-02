# kacchiOS Project Structure

## Directory Organization

The project follows a modular architecture organized by functionality:

### `/src/` - Source Code

All source code is organized in subdirectories by function:

#### `/src/boot/` - Bootloader & Initialization
- **boot.S** - x86 multiboot bootloader entry point
  - Sets up basic CPU state
  - Initializes stack
  - Jumps to kernel main function
  
- **sched.S** - Assembly-level context switching
  - `context_switch()` function for task/process switching
  - Uses x86 register save/restore (pusha/popa)
  - Critical for scheduler operation
  
- **link.ld** - GNU Linker script
  - Defines memory layout (code, data, BSS sections)
  - Positions bootloader at entry point
  - Maps kernel sections for QEMU execution

#### `/src/kernel/` - Core Kernel Components
- **kernel.c** - Main kernel entry and CLI shell
  - `kmain()` function - kernel bootstrap
  - Interactive command loop with serial I/O
  - Initializes all subsystems
  
- **scheduler.c/.h** - Cooperative Round-Robin Scheduler
  - Task creation, yielding, sleeping, and termination
  - Priority-based task selection
  - Tick-based time simulation
  - API: `create_task()`, `yield()`, `sleep_ticks()`, `exit_task()`, `sched_ps()`
  
- **process.c/.h** - Process Manager
  - Process lifecycle management (create, run, block, exit)
  - Parent-child process relationships
  - Process state tracking and accounting
  - Signal handling framework
  - API: `proc_create()`, `proc_exit()`, `proc_list()`, `proc_signal_register()`
  
- **memory.c/.h** - Dynamic Memory Allocator
  - First-fit malloc/free with free-list coalescing
  - Block splitting for efficient memory use
  - Heap statistics and debugging
  - 64KB heap at address 0x00100000
  - API: `malloc()`, `free()`, `realloc()`, `mem_stats()`, `mem_dump()`
  
- **string.c/.h** - String Utilities
  - Basic string operations: strlen, strcmp, strcpy, memcpy
  - Essential for kernel and driver code
  
- **types.h** - Type Definitions
  - uint32_t, uint16_t, uint8_t, etc.
  - Multiboot header structures
  - Type compatibility across x86 32-bit platform
  
- **io.h** - I/O Port Operations
  - `outb()`, `inb()` for port read/write
  - Used by serial driver and hardware access

#### `/src/drivers/` - Device Drivers
- **serial.c/.h** - Serial Port Driver (COM1)
  - Serial I/O at COM1 (0x3F8)
  - 38400 baud configuration
  - Blocking character I/O (getc, putc, puts)
  - ANSI terminal control (clear screen)
  - API: `serial_init()`, `serial_getc()`, `serial_putc()`, `serial_puts()`, `serial_clear()`

#### `/src/managers/` - (Reserved for future subsystems)
- Interrupt handling
- Virtual memory
- File system
- Device management

### `/bin/` - Build Artifacts
- Compiled object files (.o)
- Generated during `make` command
- Cleaned by `make clean`

### `/docs/` - Documentation
- PROJECT_STRUCTURE.md (this file)
- Design documents
- API documentation
- Build instructions

### Root Level Files
- **Makefile** - Build automation
- **Dockerfile** - Container definition for cross-compilation
- **kernel.elf** - Final kernel executable
- **LICENSE** - MIT License
- **README.md** - Project overview and quick start

## Build Process

### File Dependencies

```
boot.S ──┐
         ├─→ boot.o ──┐
sched.S ─┘             │
kernel.c ──┐           │
scheduler.c ├→ *.o ──→ Linker (link.ld) ──→ kernel.elf
memory.c  │           │
process.c ├─→ *.o ──→│
serial.c  ├─→ *.o ──→│
string.c ─┘           │
```

### Compilation Steps
1. **Assembly files** (*.S) → compiled with GNU as → (*.o)
2. **C files** (*.c) → compiled with gcc → (*.o)
3. **Linker** combines all (*.o) using link.ld → kernel.elf
4. **Output**: kernel.elf is loaded and executed by QEMU

## Include Dependencies

```
kernel.c
├── serial.h (user input/output)
├── scheduler.h (task management)
├── memory.h (memory allocation)
├── process.h (process management)
└── string.h (string operations)

scheduler.h → types.h

process.h → types.h

memory.h → types.h

serial.h → types.h, io.h
```

## Memory Layout

```
0x00000000 ┌─────────────────────────┐
           │  Reserved (Bootloader)  │
           │       BIOS Data         │
0x00100000 ├─────────────────────────┤
           │                         │
           │   Heap (64KB)           │  ← malloc/free operates here
           │  (Memory Manager)       │
0x00120000 ├─────────────────────────┤
           │                         │
           │   Available Space       │
           │                         │
0x00200000 └─────────────────────────┘
```

## Module Responsibilities

| Module | Responsibility | Key Functions |
|--------|-----------------|----------------|
| boot.S | CPU initialization, bootloader | _start, multiboot header |
| sched.S | Low-level context switching | context_switch() |
| kernel.c | Kernel initialization, CLI | kmain(), command dispatcher |
| scheduler.c | Task scheduling, time management | sched_init(), create_task(), yield() |
| process.c | Process lifecycle, signals | proc_create(), proc_exit(), proc_list() |
| memory.c | Heap allocation, memory management | malloc(), free(), mem_stats() |
| serial.c | Serial I/O communication | serial_init(), serial_getc/putc() |
| string.c | String operations | strlen(), strcmp(), strcpy() |

## Adding New Components

### To Add a New Driver
1. Create file in `src/drivers/mydriver.c` and `src/drivers/mydriver.h`
2. Add object rule in Makefile: `$(BINDIR)/mydriver.o: $(DRIVERDIR)/mydriver.c`
3. Add to OBJS list in Makefile
4. Include header in `kernel.c` when needed

### To Add a New Kernel Subsystem
1. Create files in `src/kernel/subsystem.c` and `src/kernel/subsystem.h`
2. Add compilation rules to Makefile
3. Include and initialize in `kernel.c` kmain()
4. Extend CLI with new commands if needed

### To Add Documentation
1. Create markdown file in `docs/` directory
2. Reference from README.md
3. Keep technical documentation in docs/

## Compilation Details

### Compiler Flags Explanation
- `-m32`: 32-bit x86 architecture
- `-ffreestanding`: No C standard library assumptions
- `-O2`: Optimization level 2
- `-Wall -Wextra`: Enable warnings
- `-nostdinc`: Don't include standard C headers
- `-fno-builtin`: Don't use built-in functions
- `-fno-stack-protector`: Disable stack canaries

### Linker Flags
- `-m elf_i386`: 32-bit ELF output format
- `-T link.ld`: Use custom linker script

## Testing & Validation

### Build Validation
```bash
make clean          # Remove old artifacts
make                # Rebuild everything
```

### Execution Validation
```bash
make run            # Run in QEMU (serial output)
make run-vga        # Run with VGA window
```

### Docker Build (For Cross-Compilation)
```bash
docker build -t kacchios-build --platform linux/amd64 .
docker run --rm -it --platform linux/amd64 -v $(pwd):/workspace \
  kacchios-build bash -c "cd /workspace && make clean && make run"
```

## Version Control

The `.git/` directory tracks all changes:
- Original project creation
- Feature additions (scheduler, memory, process managers)
- Bug fixes and refactoring
- Documentation updates

Use `git log` to see commit history and `git diff` to track changes.
