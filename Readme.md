# 🍚 kacchiOS - Educational Baremetal OS

> A complete, modular operating system kernel demonstrating core OS concepts through clean, understandable code.

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Build](https://img.shields.io/badge/build-passing-brightgreen.svg)]()
[![Platform](https://img.shields.io/badge/platform-x86%2032--bit-blue.svg)]()
[![Language](https://img.shields.io/badge/language-C%2FASM-orange.svg)]()

## 🎯 Overview

**kacchiOS** is a complete bare-metal OS kernel built from scratch for educational purposes. It demonstrates core operating system concepts including:

- 🔄 **Cooperative Task Scheduling** — Round-robin with priority support
- 💾 **Dynamic Memory Management** — First-fit heap allocation with coalescing
- 🔗 **Process Management** — Multi-process with parent-child relationships
- 📡 **Serial Communication** — Interactive CLI shell with 8 commands
- 🏗️ **Modular Architecture** — Clean separation of concerns

Perfect for students learning OS fundamentals through hands-on implementation.

## ✨ Features at a Glance

### Scheduler (Cooperative Round-Robin)
| Feature | Details |
|---------|---------|
| **Task Creation** | `create_task(fn, priority)` with up to 16 tasks |
| **Priority Scheduling** | Tasks selected by priority + round-robin |
| **Cooperative Yielding** | Manual context switches via `yield()` |
| **Task Sleep** | `sleep_ticks(n)` blocks for n ticks |
| **Task States** | RUNNING, READY, BLOCKED, ZOMBIE |
| **Tick Tracking** | Simulated time counter for scheduling |

### Memory Manager (Dynamic Heap)
| Feature | Details |
|---------|---------|
| **malloc()** | First-fit heap allocation |
| **free()** | Deallocate with double-free detection |
| **realloc()** | Resize existing allocations |
| **Coalescing** | Adjacent free blocks automatically merge |
| **Block Splitting** | Efficient memory utilization |
| **Heap Statistics** | `mem` command shows usage breakdown |
| **Debug Dump** | `memdump` shows all allocations |

### Process Manager
| Feature | Details |
|---------|---------|
| **Process Creation** | `proc_create(ppid)` creates processes |
| **Process Exit** | `proc_exit(code)` with exit codes |
| **Parent-Child Tracking** | Process hierarchy with up to 8 children |
| **Process Waiting** | `proc_wait()` reaps zombies |
| **Process Queries** | `proc_getpid()`, `proc_getppid()` |
| **Signal Handling** | Framework for 16 signals per process |
| **Process States** | CREATED, RUNNING, BLOCKED, ZOMBIE |
| **CPU Accounting** | Tracks ticks per process |

### Interactive CLI Shell
| Command | Function |
|---------|----------|
| `ps` | List all tasks (scheduler view) |
| `plist` | List all processes (detailed) |
| `mem` | Show memory statistics |
| `memdump` | Debug: dump all allocations |
| `clear` | Clear screen (ANSI codes) |
| `yield` | Manually yield to scheduler |
| `create` | Create a new process |
| `exit` | Shutdown OS and return to terminal |
| `help` | Show available commands |

## 🚀 Quick Start

### Prerequisites

Choose your platform:

**macOS with Docker:**
```bash
brew install docker
docker build -t kacchios-build --platform linux/amd64 .
```

**Linux with native tools:**
```bash
sudo apt-get install build-essential gcc-multilib qemu-system-x86 make
```

**Windows:**
Install Docker Desktop and set up WSL2

### Run the OS

**Using Docker (recommended):**
```bash
docker run --rm -it --platform linux/amd64 -v $(pwd):/workspace kacchios-build make run
```

**Using native build (Linux only):**
```bash
make clean && make run
```

### Expected Output

```
========================================
    kacchiOS - Minimal Baremetal OS
========================================
Hello from kacchiOS!
Initializing managers...

[MEM] Initialized at 0x00100000 size=65536
[PROC] Manager initialized
[task A] running (ticks=1)
[task B] hello

kacchiOS>
```

### Interactive Demo

```bash
kacchiOS> ps
PID     STATE   PRIO    WAKE
0       RUN     0       0
1       READY   1       0
2       READY   1       0

kacchiOS> mem
[MEM STATS]
  Total heap:   65536 bytes
  Used:         0 bytes
  Free:         65536 bytes
  Free blocks:  1
  Alloc blocks: 0

kacchiOS> plist
PID     PPID    STATE           CPU
0       4294967295      RUNNING 0

kacchiOS> create
Creating process...
Process created successfully!
PID: 1

kacchiOS> exit
Shutting down kacchiOS...
kacchiOS exiting...
(returns to terminal)
```

## 📁 Project Structure

```
kacchiOS/
├── src/                              # Source code (organized by subsystem)
│   ├── boot/                         # System initialization
│   │   ├── boot.S                    # x86 multiboot bootloader (Assembly)
│   │   ├── sched.S                   # Context switch routine (Assembly)
│   │   └── link.ld                   # Linker script for kernel layout
│   │
│   ├── kernel/                       # Core kernel subsystems
│   │   ├── kernel.c                  # Main kernel + interactive CLI shell
│   │   ├── types.h                   # Type definitions
│   │   ├── io.h                      # I/O port macros
│   │   ├── scheduler.c/.h            # Task scheduler (cooperative round-robin)
│   │   ├── memory.c/.h               # Dynamic heap allocator
│   │   ├── process.c/.h              # Process manager
│   │   └── string.c/.h               # String utilities
│   │
│   └── drivers/                      # Hardware device drivers
│       └── serial.c/.h               # Serial port (COM1) driver
│
├── bin/                              # Compiled object files (generated)
├── config/                           # Configuration and documentation
│   ├── LICENSE                       # MIT License
│   ├── Dockerfile                    # Docker build definition
│   ├── Makefile                      # Build system
│   ├── .gitignore                    # Git ignore rules
│   ├── ORGANIZATION.md               # Project organization guide
│   ├── RESTRUCTURING_SUMMARY.md      # Restructuring notes
│   └── OS_LAB_Project.pdf            # Project specification
│
├── docs/                             # Documentation
│   ├── BUILD_GUIDE.md                # Comprehensive build guide
│   └── (additional guides)
│
├── kernel.elf                        # Final bootable kernel (generated)
├── Readme.md                         # This file
└── .git/                             # Git repository

```

### Directory Purposes

| Directory | Purpose |
|-----------|---------|
| `src/boot/` | CPU initialization, bootloader, context switching |
| `src/kernel/` | Scheduler, memory manager, process manager, core logic |
| `src/drivers/` | Hardware drivers (serial, future: keyboard, disk) |
| `bin/` | Compiled .o files (auto-generated) |
| `config/` | Configuration files, Docker, licenses, specs |
| `docs/` | Complete project documentation |

## 🛠️ Build & Deployment

### Build System Overview

```
src/ (organized source)
  ↓
gcc/as (compile & assemble)
  ↓
bin/ (object files)
  ↓
ld (link with linker script)
  ↓
kernel.elf (bootable image)
  ↓
qemu (x86 emulation)
```

### Build Commands

**Using Docker (Recommended):**
```bash
# First time: build Docker image
docker build -t kacchios-build --platform linux/amd64 .

# Then: run builds in container
docker run --rm -it --platform linux/amd64 -v $(pwd):/workspace kacchios-build make run
```

**Using native Linux tools:**
```bash
# Requires: gcc, binutils, make with -m32 multilib support
make clean  # Remove old builds
make        # Compile and link
make run    # Launch in QEMU
```

### Makefile Targets

| Command | Purpose |
|---------|---------|
| `make` | Build kernel.elf from source |
| `make run` | Build + run in QEMU (serial mode) |
| `make run-vga` | Build + run in QEMU (GUI window) |
| `make debug` | Build + run with GDB support |
| `make clean` | Remove build artifacts |

### Compiler Configuration

- **Flags**: `-m32 -ffreestanding -O2 -Wall -Wextra`
- **Include paths**: `src/kernel src/drivers src/managers src/`
- **Linker**: x86 32-bit ELF (`-m elf_i386`)
- **Target**: QEMU x86 emulator

## 📚 Technical Architecture

### Memory Layout
```
0x00000000 - 0x000FFFFF: Reserved (bootloader, BIOS)
0x00100000 - 0x0011FFFF: Heap (64KB) - Managed by memory manager
0x00120000 onwards:      Available for future expansion
```

### Scheduler Design
- **Type**: Cooperative round-robin with priority levels
- **Context Switch**: Manual stack switching in assembly (sched.S)
- **Tick System**: Simulated time counter incremented on each yield
- **Selection**: Highest priority ready task, round-robin within priority level
- **Task States**: RUNNING, READY, BLOCKED, ZOMBIE

### Memory Manager Design
- **Algorithm**: First-fit allocation with free-list coalescing
- **Block Metadata**: Size, free flag, prev/next pointers
- **Coalescence**: Adjacent free blocks automatically merge
- **Block Splitting**: Large allocations split if remainder useful
- **Heap Size**: 64KB (configurable at initialization)

### Process Management Design
- **Hierarchy**: Parent-child relationships tracked (max 8 children per parent)
- **States**: CREATED → RUNNING → ZOMBIE → FREE
- **Stack**: 2KB private stack allocated per process via malloc
- **Signals**: Framework for 16 signals per process (extensible)
- **Accounting**: CPU ticks tracked per process

## 🔧 How to Extend kacchiOS

### Add a New CLI Command

Edit [src/kernel/kernel.c](src/kernel/kernel.c):

```c
else if (strcmp(input, "mycommand") == 0) {
    serial_puts("My command output\n");
} else if (strcmp(input, "help") == 0) {
    serial_puts("Commands: ps, plist, mem, memdump, clear, yield, create, exit, mycommand, help\n");
}
```

### Use Dynamic Memory

```c
// Allocate memory
int *array = (int*)malloc(100 * sizeof(int));
if (array == NULL) {
    serial_puts("Memory allocation failed\n");
    return;
}

// Use memory
array[0] = 42;

// Free memory
free(array);
```

### Create a New Process

```c
int pid = proc_create(0);  // Create as child of kernel (PID 0)
if (pid > 0) {
    serial_puts("Process created with PID: ");
    print_u32(pid);
    serial_puts("\n");
} else {
    serial_puts("Process creation failed\n");
}
```

### Create a New Task

```c
void my_task(void) {
    while (1) {
        serial_puts("Task running\n");
        sleep_ticks(5);  // Sleep for 5 ticks
    }
}

// In kmain():
create_task(my_task, 2);  // Priority 2
```

## ✅ Implementation Status

### Core Features (Completed)
| Feature | Status | File |
|---------|--------|------|
| Bootloader | ✅ Complete | [src/boot/boot.S](src/boot/boot.S) |
| Scheduler | ✅ Complete | [src/kernel/scheduler.c](src/kernel/scheduler.c) |
| Memory Manager | ✅ Complete | [src/kernel/memory.c](src/kernel/memory.c) |
| Process Manager | ✅ Complete | [src/kernel/process.c](src/kernel/process.c) |
| Serial Driver | ✅ Complete | [src/drivers/serial.c](src/drivers/serial.c) |
| CLI Shell | ✅ Complete | [src/kernel/kernel.c](src/kernel/kernel.c) |

### Feature Completeness
| Category | Status | Features |
|----------|--------|----------|
| **Must-Have** | ✅ 100% | Bootloader, Serial I/O, Scheduler, Memory, Processes, CLI |
| **Good-to-Have** | ✅ 100% | Priority scheduling, Sleep, Coalescing, Hierarchy, Signals, Stats |
| **Bonus** | ✅ 100% | Ticks, Double-free detection, Memory dump, Process states, ANSI control, Realloc |

## 🐛 Known Limitations & Future Work

### Current Limitations
- **Single-core** — No SMP/multi-processor support
- **No preemption** — Tasks must cooperatively yield
- **No virtual memory** — Direct physical memory access
- **No interrupts** — No interrupt handling/IDT
- **No I/O** — Serial driver only, no disk/keyboard
- **Limited processes** — Max 32 processes, 16 tasks

### Planned Enhancements
- [ ] Hardware timer (PIT) for preemptive scheduling
- [ ] Interrupt Descriptor Table (IDT) and exception handling
- [ ] Virtual memory with paging
- [ ] File system (FAT-like)
- [ ] Keyboard driver
- [ ] Disk driver
- [ ] Network stack
- [ ] IPC (inter-process communication)



