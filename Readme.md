# 🍚 kacchiOS

A minimal, educational baremetal operating system with scheduler, memory manager, and process manager.

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Build](https://img.shields.io/badge/build-passing-brightgreen.svg)]()
[![Platform](https://img.shields.io/badge/platform-x86-lightgrey.svg)]()

## 📖 Overview

kacchiOS is a complete bare-metal OS kernel built from scratch for educational purposes. It demonstrates core OS concepts including process scheduling, dynamic memory allocation, and process management.

## ✨ Current Features

### Core Components
- ✅ **Multiboot-compliant bootloader** — Boots via GRUB/QEMU
- ✅ **Serial I/O driver** (COM1) — Communication via serial port
- ✅ **Basic string utilities** — Essential string operations
- ✅ **Clean, documented code** — Well-structured and extensible

### Scheduler (Cooperative Round-Robin)
- ✅ **Task creation** — `create_task(fn, priority)`
- ✅ **Cooperative yielding** — `yield()` for manual task switching
- ✅ **Priority scheduling** — Tasks scheduled by priority level
- ✅ **Task sleep** — `sleep_ticks(n)` to block for n scheduler ticks
- ✅ **Process listing** — `ps` command shows all scheduler tasks
- ✅ **Task exit** — `exit_task()` for clean task termination

### Memory Manager (Dynamic Heap Allocation)
- ✅ **malloc()** — Allocate memory blocks (first-fit algorithm)
- ✅ **free()** — Deallocate memory
- ✅ **realloc()** — Resize existing allocations
- ✅ **Free-list coalescing** — Merges adjacent free blocks
- ✅ **Block splitting** — Efficient memory utilization
- ✅ **Heap statistics** — `mem` command shows heap usage
- ✅ **Debug dump** — `memdump` shows all allocations with addresses
- ✅ **Double-free detection** — Warns on invalid frees

### Process Manager
- ✅ **Process creation** — `proc_create(ppid)` creates new process
- ✅ **Process termination** — `proc_exit(code)` with exit codes
- ✅ **Parent-child relationships** — Tracks process hierarchy
- ✅ **Process waiting** — `proc_wait(pid, &code)` to reap zombies
- ✅ **Process queries** — `proc_getpid()`, `proc_getppid()`
- ✅ **Process listing** — `plist` command shows all processes
- ✅ **Signal handling** — Framework for signal registration and sending
- ✅ **Stack allocation** — Each process gets its own stack
- ✅ **CPU accounting** — Tracks CPU ticks per process
- ✅ **Process states** — CREATED, RUNNING, BLOCKED, ZOMBIE

### CLI Commands
| Command | Description |
|---------|-------------|
| `ps` | List all scheduler tasks with state and priority |
| `plist` | List all processes with PID, PPID, state, and CPU ticks |
| `mem` | Show memory statistics (total, used, free, block counts) |
| `memdump` | Debug dump of all heap allocations with addresses |
| `clear` | Clear the screen (ANSI escape codes) |
| `yield` | Manually yield to scheduler |
| `exit` | Shutdown the OS and halt the CPU |
| `help` | Show available commands |

## 🚀 Quick Start

### Prerequisites

```bash
# On macOS (with Docker)
brew install docker

# On Linux
sudo apt-get install docker.io

# On Windows
# Download Docker Desktop
```

### Build and Run

```bash
# Clone the repository
git clone https://github.com/yourusername/kacchiOS.git
cd kacchiOS

# Build Docker image (one time)
docker build -t kacchios-build --platform linux/amd64 .

# Build and run the OS
docker run --rm -it --platform linux/amd64 -v $(pwd):/workspace kacchios-build bash -c "cd /workspace && make clean && make run"
```

You should see:
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

Try these commands:

```bash
kacchiOS> ps
PID     STATE   PRIO    WAKE
0       RUN     0       0
1       READY   1       0
2       READY   1       0

kacchiOS> plist
PID     PPID    STATE           CPU
0       4294967295      RUNNING 0

kacchiOS> mem
[MEM STATS]
  Total heap:   65536 bytes
  Used:         0 bytes
  Free:         65536 bytes
  Free blocks:  1
  Alloc blocks: 0

kacchiOS> memdump
[MEM DUMP]
  [0] addr=0x00100000 size=65536 state=FREE

kacchiOS> clear
(screen clears)

kacchiOS> exit
Shutting down kacchiOS...
(CPU halts)
```

## 📁 Project Structure

```
kacchiOS/
├── boot.S              # x86 bootloader entry (Assembly)
├── kernel.c            # Main kernel + CLI shell
├── serial.c/.h         # Serial port driver (COM1)
├── string.c/.h         # String utilities
├── types.h             # Basic type definitions
├── io.h                # I/O port operations
│
├── scheduler.c/.h      # Cooperative round-robin scheduler
├── sched.S             # Context switch (Assembly)
│
├── memory.c/.h         # Dynamic heap allocator
├── process.c/.h        # Process manager
│
├── link.ld             # Linker script
├── Makefile            # Build system
├── Dockerfile          # Docker container definition
└── README.md           # This file
```

## 🛠️ Build System

### Makefile Targets

| Command | Description |
|---------|-------------|
| `make` or `make all` | Build kernel.elf |
| `make run` | Run in QEMU (serial output) |
| `make run-vga` | Run in QEMU (with VGA window) |
| `make debug` | Run with GDB debugging enabled |
| `make clean` | Remove build artifacts |

### Compile Without Docker

On Linux with x86 cross-compiler:
```bash
make clean
make
make run
```

## 📚 Architecture Details

### Memory Layout
```
0x00000000 - 0x000FFFFF: Reserved (bootloader, BIOS)
0x00100000 - 0x0011FFFF: Heap (64KB) - Managed by memory manager
0x00120000 - 0x00200000: Available for future expansion
```

### Scheduler Algorithm
- **Type**: Cooperative round-robin with priorities
- **Context Switch**: Manual stack switching via assembly (`context_switch()`)
- **Tick System**: Simulated time tracking for sleep operations
- **Selection**: Priority-based (higher priority runs first)

### Memory Allocation
- **Algorithm**: First-fit with free-list coalescing
- **Block Structure**: Each allocation has metadata (size, free flag, linked list pointers)
- **Coalescence**: Adjacent free blocks are automatically merged to reduce fragmentation
- **Block Splitting**: Large allocations are split if remainder is useful

### Process Management
- **Hierarchy**: Tracks parent-child relationships
- **States**: CREATED → RUNNING → ZOMBIE → FREE
- **Stack**: Each process allocated 2KB private stack via malloc
- **Signals**: Framework for signal handlers (extensible)

## 🔧 Extending kacchiOS

### Adding a New Command

Edit `kernel.c`:
```c
} else if (strcmp(input, "newcmd") == 0) {
    my_function();
} else if (strcmp(input, "help") == 0) {
    serial_puts("Commands: ... newcmd ...\n");
```

### Adding to Memory Manager

Use `malloc()` and `free()` like standard C:
```c
void *buf = malloc(256);
if (buf) {
    /* use buf */
    free(buf);
}
```

### Creating Processes

```c
int pid = proc_create(0);  /* Create child of process 0 */
if (pid > 0) {
    serial_puts("Created process ");
    print_u32(pid);
}
```

## 📊 Features by Category

### Must-Have (✅ Implemented)
- Bootloader
- Serial I/O
- Scheduler with task creation/yield
- Memory allocation (malloc/free)
- Process creation/termination
- CLI shell

### Good-to-Have (✅ Implemented)
- Priority scheduling
- Sleep functionality
- Free-list coalescing
- Process hierarchy
- Signal handling framework
- Heap statistics
- Process accounting
- Screen clearing

### Bonus (✅ Implemented)
- Simulated tick counter
- Double-free detection
- Debug memory dump
- Process state tracking
- ANSI terminal control
- Realloc support
- Process listing with details

## 🐛 Known Limitations

- **Single-core** — No multiprocessor support
- **No preemption** — Relies on cooperative yielding
- **No virtual memory** — Direct memory access
- **No interrupts** — No hardware timer (simulated ticks only)
- **No I/O drivers** — Serial only, no disk/network
- **Fixed memory** — Heap size is 64KB (configured at boot)

## 🚀 Future Enhancements

- Hardware timer with preemptive scheduling (PIT)
- Interrupt Descriptor Table (IDT) and exception handling
- Virtual memory with paging
- File system (simple FAT-like)
- Additional device drivers (keyboard, disk)
- Network stack
- IPC (inter-process communication)

## 📚 Learning Resources

This project teaches:
- **Assembly**: x86 real mode, bootloader, context switching
- **C Systems Programming**: Kernel structures, memory management
- **OS Concepts**: Scheduling, process management, memory allocation
- **Baremetal Development**: QEMU, linker scripts, multiboot format

## 📝 License

MIT License - See [LICENSE](LICENSE) for details

## 👨‍💻 Author

Built for educational purposes. Designed to be understandable and extensible for OS learning.

---

**Ready to run?**
```bash
docker run --rm -it --platform linux/amd64 -v $(pwd):/workspace kacchios-build bash -c "cd /workspace && make run"
```

Type `help` in the kacchiOS shell to see all available commands!

### Recommended Reading

- [XINU OS](https://xinu.cs.purdue.edu/) - Educational OS similar to kacchiOS
- [OSDev Wiki](https://wiki.osdev.org/) - Comprehensive OS development guide
- [The Little OS Book](https://littleosbook.github.io/) - Practical OS development
- [Operating Systems: Three Easy Pieces](https://pages.cs.wisc.edu/~remzi/OSTEP/) - OS concepts textbook

### Related Topics

- x86 Assembly Language
- Memory Management
- Process Scheduling
- System Calls
- Interrupt Handling

## 🤝 Contributing

Contributions are welcome! Please feel free to submit issues and pull requests.

### Guidelines

1. Keep code simple and educational
2. Add comments explaining complex concepts
3. Follow existing code style
4. Test changes in QEMU before submitting

## 📄 License

This project is licensed under the MIT License.

## 👨‍🏫 About

kacchiOS was created as an educational tool for teaching operating system concepts. It provides a minimal, working foundation that students can extend to learn core OS principles through hands-on implementation.

## 🙏 Acknowledgments

- Inspired by XINU OS
- Built with guidance from OSDev community
- Thanks to all students who have contributed
