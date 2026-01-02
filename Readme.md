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
docker run --rm -it --platform linux/amd64 -v $(pwd):/workspace kacchios-build bash -c "cd /workspace && make run"
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
(returns to terminal)
```

## 📁 Project Structure

```
kacchiOS/
├── src/                           # Source code (organized by subsystem)
│   ├── boot/                      # Bootloader and system initialization
│   │   ├── boot.S                 # x86 multiboot bootloader (Assembly)
│   │   ├── sched.S                # Context switch assembly routine
│   │   └── link.ld                # Linker script for kernel layout
│   │
│   ├── kernel/                    # Core kernel subsystems
│   │   ├── kernel.c               # Main kernel entry + interactive CLI shell
│   │   ├── types.h                # Basic type definitions
│   │   ├── io.h                   # I/O port access macros
│   │   │
│   │   ├── scheduler.c/.h         # Cooperative round-robin task scheduler
│   │   ├── memory.c/.h            # Dynamic heap allocator (malloc/free)
│   │   ├── process.c/.h           # Process manager with parent-child tracking
│   │   └── string.c/.h            # String utility functions
│   │
│   ├── drivers/                   # Hardware device drivers
│   │   ├── serial.c/.h            # Serial port (COM1) driver at 0x3F8
│   │   └── (extensible for more drivers)
│   │
│   └── managers/                  # (Reserved for future subsystems)
│
├── bin/                           # Compiled object files (generated by make)
├── config/                        # Configuration and metadata
│   ├── Dockerfile                 # Docker container for reproducible builds
│   ├── LICENSE                    # MIT License
│   ├── ORGANIZATION.md            # Project organization notes
│   ├── RESTRUCTURING_SUMMARY.md   # Summary of restructuring changes
│   └── OS_LAB_Project.pdf         # Project specification
│
├── docs/                          # Documentation and build guides
│   ├── PROJECT_STRUCTURE.md       # Complete directory structure documentation
│   ├── BUILD_GUIDE.md             # Comprehensive build and development guide
│   └── (additional documentation)
│
├── Makefile                       # Build automation (src files → kernel.elf)
├── kernel.elf                     # Final bootable kernel executable
├── README.md                      # This file
└── .git/                          # Git version control
```

### Directory Purposes

| Directory | Purpose |
|-----------|---------|
| `src/boot/` | CPU initialization, bootloader, context switching |
| `src/kernel/` | Scheduler, memory manager, process manager, kernel logic |
| `src/drivers/` | Hardware drivers (serial, future: keyboard, disk) |
| `src/managers/` | Reserved for future subsystems |
| `bin/` | Compiled .o files (auto-generated) |
| `config/` | Configuration files, Docker, licenses, specs |
| `docs/` | Complete documentation and guides |

## 🛠️ Build System

### Makefile Overview

The Makefile automates compilation of the modular source tree:

```
src/ (organized code)
  ↓
gcc/as (compile and assemble)
  ↓
bin/ (object files: .o)
  ↓
ld (link with link.ld script)
  ↓
kernel.elf (bootable image)
  ↓
qemu (emulation)
```

### Include Paths

The Makefile is configured with include paths for all subsystems:
- `-Isrc/kernel` — Core scheduler, memory, process managers
- `-Isrc/drivers` — Serial driver and other hardware
- `-Isrc/managers` — Future subsystems
- `-Isrc` — Root includes

### Makefile Targets

| Command | Description |
|---------|-------------|
| `make` or `make all` | Build kernel.elf from source |
| `make run` | Build and run in QEMU (serial terminal mode) |
| `make run-vga` | Build and run in QEMU (graphical window) |
| `make debug` | Build and run with GDB debugging support |
| `make clean` | Remove all build artifacts (bin/*.o, kernel.elf) |

### Build Without Docker

On Linux with x86 build tools:
```bash
# Requires: gcc, binutils, nasm, make (multilib support for -m32)
make clean
make all
make run
```

### Docker-Based Build

Recommended for cross-platform compatibility:
```bash
# Build the image once
docker build -t kacchios-build --platform linux/amd64 .

# Run builds in containers
docker run --rm --platform linux/amd64 -v $(pwd):/workspace kacchios-build make
docker run --rm -it --platform linux/amd64 -v $(pwd):/workspace kacchios-build make run
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

## 📚 Documentation

- **[PROJECT_STRUCTURE.md](docs/PROJECT_STRUCTURE.md)** - Detailed project organization and module responsibilities
- **[BUILD_GUIDE.md](docs/BUILD_GUIDE.md)** - Comprehensive build and development guide
- **[README.md](README.md)** - Quick start and feature overview (this file)

## 📝 License

MIT License - See [LICENSE](LICENSE) for details

## 👨‍💻 Author

Built for educational purposes. Designed to be understandable and extensible for OS learning.

---

**Ready to run?**

```bash
# Build Docker image (first time only)
docker build -t kacchios-build --platform linux/amd64 .

# Build and run the OS
docker run --rm -it --platform linux/amd64 -v $(pwd):/workspace kacchios-build make run
```

Type `help` in the kacchiOS shell to see all available commands!

---

## 📖 Documentation Files

- **[PROJECT_STRUCTURE.md](PROJECT_STRUCTURE.md)** — Complete project directory structure and organization
- **[config/Dockerfile](config/Dockerfile)** — Docker container definition
- **[config/LICENSE](config/LICENSE)** — MIT License
- **[config/ORGANIZATION.md](config/ORGANIZATION.md)** — Project organization principles and module breakdown
- **[config/RESTRUCTURING_SUMMARY.md](config/RESTRUCTURING_SUMMARY.md)** — Summary of directory restructuring and why
- **[docs/BUILD_GUIDE.md](docs/BUILD_GUIDE.md)** — Comprehensive build instructions and troubleshooting

## 🔗 External Resources

### OS Development

- [XINU OS](https://xinu.cs.purdue.edu/) — Educational OS similar to kacchiOS
- [OSDev Wiki](https://wiki.osdev.org/) — Comprehensive OS development reference
- [The Little OS Book](https://littleosbook.github.io/) — Practical OS development tutorial
- [Operating Systems: Three Easy Pieces](https://pages.cs.wisc.edu/~remzi/OSTEP/) — Classic OS textbook

### x86 Architecture

- [x86-64 System V ABI](https://refspecs.linuxbase.org/elf/x86_64-abi-0.99.pdf) — 64-bit calling conventions
- [Intel 8086/8088 Reference](https://en.wikipedia.org/wiki/X86) — 16/32-bit architecture
- [Multiboot Specification](https://www.gnu.org/software/grub/manual/multiboot/) — Bootloader standard

## 🤝 Development Workflow

### Adding a New Feature

1. Create source file in appropriate directory (`src/kernel/`, `src/drivers/`, etc.)
2. Add `.h` header with public interface
3. Update `Makefile` if adding new source files:
   ```makefile
   OBJS += $(BINDIR)/mynewmodule.o
   $(BINDIR)/mynewmodule.o: src/path/mynewmodule.c
   ```
4. Build and test:
   ```bash
   docker run --rm --platform linux/amd64 -v $(pwd):/workspace kacchios-build make
   ```

### Code Organization Principles

- **Modularity**: Each subsystem has clear responsibility
- **Documentation**: Descriptive comments explaining complex logic
- **Simplicity**: Educational code favors clarity over optimization
- **Extensibility**: New drivers/managers fit easily into structure

## 📄 Project Status

**Current Version**: 1.0 (Fully Featured Educational OS)

**Completed Subsystems**:
- ✅ Bootloader (Multiboot compliant)
- ✅ Scheduler (Cooperative round-robin with priorities)
- ✅ Memory Manager (First-fit heap allocation with coalescing)
- ✅ Process Manager (Multi-process with parent-child relationships)
- ✅ Serial Driver (COM1 at 0x3F8, 38400 baud)
- ✅ CLI Shell (8 commands: ps, plist, mem, memdump, clear, yield, exit, help)

**Next Steps** (Future Work):
- Hardware timer for preemptive scheduling
- IDT/exception handling
- Virtual memory with paging
- File system support
- Keyboard driver
- Network stack

## 📝 License

MIT License - See [LICENSE](LICENSE) for details

## 👨‍💻 Author & Credits

**Built as an educational project** to demonstrate core OS concepts through clean, understandable code.

Created with guidance from:
- OSDev community (osdev.org)
- XINU OS documentation
- Operating Systems textbooks and courses

---

**Questions or contributions?** Feel free to open issues or submit pull requests!

**For submission requirements**, see [config/ORGANIZATION.md](config/ORGANIZATION.md) and [config/RESTRUCTURING_SUMMARY.md](config/RESTRUCTURING_SUMMARY.md).
