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

## 📖 Learning Resources

### OS Concepts Demonstrated
- Process management and scheduling
- Memory allocation and management
- Context switching
- Process hierarchies
- Cooperative multitasking
- Baremetal programming

### Reference Materials
- [OSDev.org](https://wiki.osdev.org/) — Comprehensive OS development wiki
- [XINU OS](https://xinu.cs.purdue.edu/) — Similar educational OS
- [The Little OS Book](https://littleosbook.github.io/) — Practical tutorial
- [Operating Systems: Three Easy Pieces](https://pages.cs.wisc.edu/~remzi/OSTEP/) — Textbook

## 💻 Development Tips

### Debugging in QEMU

```bash
# Run with GDB support
docker run --rm -it --platform linux/amd64 -v $(pwd):/workspace kacchios-build make debug

# In another terminal, connect GDB
gdb kernel.elf
(gdb) target remote localhost:1234
(gdb) break kmain
(gdb) continue
```

### Monitor Memory Usage

```bash
kacchiOS> mem
[MEM STATS]
  Total heap:   65536 bytes
  Used:         1024 bytes
  Free:         64512 bytes
  Free blocks:  2
  Alloc blocks: 1
```

### View Process Tree

```bash
kacchiOS> plist
PID     PPID    STATE           CPU
0       4294967295      RUNNING 0
1       0       CREATED         0
```

## 📄 Documentation Files

- **[README.md](README.md)** — This file (quick start & overview)
- **[config/ORGANIZATION.md](config/ORGANIZATION.md)** — Project organization
- **[config/RESTRUCTURING_SUMMARY.md](config/RESTRUCTURING_SUMMARY.md)** — Restructuring notes
- **[docs/BUILD_GUIDE.md](docs/BUILD_GUIDE.md)** — Detailed build guide
- **[config/OS_LAB_Project.pdf](config/OS_LAB_Project.pdf)** — Original project specification

## 🤝 Contributing & Extending

This project is designed for educational purposes. To extend it:

1. **Keep code simple** — Prioritize clarity over optimization
2. **Add comments** — Explain complex concepts and algorithms
3. **Follow structure** — Add files to appropriate src/ directories
4. **Update Makefile** — Add new source files to build rules
5. **Test in QEMU** — Verify changes before committing

### Code Style
- **Naming**: snake_case for variables/functions, UPPERCASE for macros
- **Indentation**: 4 spaces (no tabs)
- **Comments**: Explain the "why", not the "what"
- **Headers**: Include guards, function prototypes, clear signatures

## 📝 License

MIT License - See [config/LICENSE](config/LICENSE) for full text

## 👨‍💻 Created For

Educational purposes to teach operating system fundamentals through hands-on implementation and code reading.

---

## 🎯 Quick Reference

### Key Commands
```bash
# Build and run
docker run --rm -it --platform linux/amd64 -v $(pwd):/workspace kacchios-build make run

# Just compile
docker run --rm --platform linux/amd64 -v $(pwd):/workspace kacchios-build make

# Clean build
docker run --rm --platform linux/amd64 -v $(pwd):/workspace kacchios-build make clean
```

### Inside kacchiOS Shell
```bash
ps                  # List tasks
plist               # List processes  
mem                 # Memory stats
memdump             # Memory debug
clear               # Clear screen
yield               # Yield to scheduler
create              # Create process
exit                # Shutdown
help                # Show commands
```

### Key Files to Understand
1. **[src/kernel/scheduler.c](src/kernel/scheduler.c)** — How tasks are scheduled
2. **[src/kernel/memory.c](src/kernel/memory.c)** — How memory is allocated
3. **[src/kernel/process.c](src/kernel/process.c)** — How processes are managed
4. **[src/kernel/kernel.c](src/kernel/kernel.c)** — Main kernel loop and CLI

---

**Questions?** Check the documentation or review the well-commented source code!

**Ready to learn?** Run it and explore!

```bash
docker run --rm -it --platform linux/amd64 -v $(pwd):/workspace kacchios-build make run
```

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

## 📚 Appendix: Complete Build & Development Reference

### Full Docker Build Commands

**First time setup:**
```bash
# Build Docker image
docker build -t kacchios-build --platform linux/amd64 .
```

**Build and run:**
```bash
# Full build cycle
docker run --rm --platform linux/amd64 -v $(pwd):/workspace kacchios-build make clean
docker run --rm --platform linux/amd64 -v $(pwd):/workspace kacchios-build make
docker run --rm -it --platform linux/amd64 -v $(pwd):/workspace kacchios-build make run
```

**Or use shorthand:**
```bash
docker run --rm -it --platform linux/amd64 -v $(pwd):/workspace kacchios-build bash -c "cd /workspace && make clean && make run"
```

### Detailed GDB Debugging

**Terminal 1: Start QEMU in debug mode**
```bash
docker run --rm -it --platform linux/amd64 -v $(pwd):/workspace kacchios-build make debug
```

**Terminal 2: Connect GDB**
```bash
gdb kernel.elf
(gdb) target remote localhost:1234
(gdb) break kmain
(gdb) continue
(gdb) step
(gdb) next
(gdb) print variable_name
(gdb) backtrace
```

### Project Organization Reference

```
kacchiOS/
├── src/                              # Source code
│   ├── boot/                         # Bootloader & initialization
│   │   ├── boot.S                    # Multiboot bootloader (x86 asm)
│   │   ├── sched.S                   # Context switch (x86 asm)
│   │   └── link.ld                   # Linker script
│   │
│   ├── kernel/                       # Core OS components
│   │   ├── kernel.c                  # Main kernel & CLI shell
│   │   ├── scheduler.c/.h            # Task scheduler (cooperative)
│   │   ├── memory.c/.h               # Heap allocator (malloc/free)
│   │   ├── process.c/.h              # Process manager
│   │   ├── string.c/.h               # String utilities
│   │   ├── types.h                   # Type definitions
│   │   └── io.h                      # I/O port macros
│   │
│   └── drivers/                      # Device drivers
│       └── serial.c/.h               # Serial port (COM1) driver
│
├── bin/                              # Build output (auto-generated)
│   └── *.o                           # Compiled object files
│
├── config/                           # Configuration & metadata
│   ├── LICENSE                       # MIT License
│   ├── Dockerfile                    # Docker build environment
│   ├── Makefile                      # Build rules
│   └── OS_LAB_Project.pdf            # Original specification
│
├── kernel.elf                        # Final kernel executable (generated)
├── Readme.md                         # This comprehensive guide
└── .git/                             # Git repository
```

### Understanding Module Dependencies

```
kernel.c (main kernel)
  ├── includes scheduler.h
  ├── includes memory.h
  ├── includes process.h
  └── includes serial.h
       └── depends on io.h

scheduler.h → types.h
memory.h → types.h
process.h → types.h
```

### Common Development Workflows

**Add a new CLI command:**
1. Edit `src/kernel/kernel.c`
2. Add to command parsing in main loop
3. Update help text
4. Test with `make run`

**Add a new subsystem:**
1. Create `.c` and `.h` files in `src/kernel/` or `src/drivers/`
2. Update `Makefile` with new object file rule
3. Include header in `kernel.c`
4. Implement and test

**Debug a crash:**
```bash
make debug          # Terminal 1: Start QEMU in debug mode
gdb kernel.elf      # Terminal 2: Connect debugger
(gdb) target remote localhost:1234
(gdb) break kmain
(gdb) continue
(gdb) bt            # See call stack when crash occurs
```

### Troubleshooting

**Problem: "as --32: command not found"**
- Solution: Use Docker with `--platform linux/amd64`
- On ARM64 hosts, the 32-bit assembler isn't available

**Problem: "serial.h: No such file or directory"**
- Solution: Check Makefile include paths: `-Isrc/kernel -Isrc/drivers`
- Rebuild with `make clean && make`

**Problem: QEMU hangs after startup**
- This is normal - the OS is waiting for input
- Type commands like `ps`, `mem`, or `exit`

**Problem: "Too many open files"**
- Docker resource limit
- Increase with: `docker run --ulimit nofile=1024:1024 ...`

### File Organization Rationale

**Why separate `src/boot/`?**
- Assembly code is isolated from C code
- Makes bootloader easy to find and modify
- Separate from kernel logic

**Why `src/kernel/` not `src/core/`?**
- Standard naming convention
- Clear that this is the kernel subsystem
- Easy for students to understand

**Why `config/` not root?**
- Keeps root clean (only README, Makefile)
- Groups build/config files together
- config files are secondary to source

**Why `bin/` for object files?**
- Standard convention in many projects
- Easy to see what's generated vs committed
- Single `make clean` removes all

### Performance Notes

- **No optimization by default** — Use `-O2` in CFLAGS for speed
- **No caching** — Memory allocations are simple first-fit
- **No parallelism** — Single-core, cooperative multitasking
- **Educational focus** — Correctness over speed

### Memory Efficiency

- Scheduler: ~1KB per task (PCB + references)
- Memory manager: ~16 bytes per allocation (metadata)
- Process manager: ~200 bytes per process (structure)
- Total overhead: ~5% of 64KB heap

### Next Steps After Running

1. **Explore the code** — Start with `src/kernel/kernel.c`
2. **Modify a task** — Change `task_a` or `task_b` behavior
3. **Add a command** — Implement your own CLI command
4. **Create a process** — Call `proc_create()` from a command
5. **Allocate memory** — Use `malloc()` and `free()`

### Related Learning Resources

**OS Concepts:**
- [Operating Systems: Three Easy Pieces](https://pages.cs.wisc.edu/~remzi/OSTEP/) - Free textbook
- [OSDev Wiki](https://wiki.osdev.org/) - Comprehensive reference

**x86 Assembly:**
- [x86-64 System V ABI](https://refspecs.linuxbase.org/elf/x86_64-abi-0.99.pdf)
- [Intel x86 Reference](https://www.intel.com/content/dam/www/public/us/en/documents/manuals/64-ia-32-architectures-software-developer-manual-combined-volumes-1-2a-2b-2c-2d-3a-3b-3c-3d.pdf)

**Educational OS Projects:**
- [XINU OS](https://xinu.cs.purdue.edu/) - Similar to kacchiOS
- [Linux from Scratch](http://www.linuxfromscratch.org/) - Build your own Linux

### Questions? Issues?

1. **Check the source code** — Well-commented and educational
2. **Review the Makefile** — Understand the build process
3. **Test in QEMU** — Add debug output with `serial_puts()`
4. **Read OSDev docs** — Comprehensive reference for OS concepts

---

**Questions or contributions?** Feel free to open issues or submit pull requests!

**Happy learning! 🚀**

```bash
docker run --rm -it --platform linux/amd64 -v $(pwd):/workspace kacchios-build make run
```

Explore it. Extend it. Learn from it.
