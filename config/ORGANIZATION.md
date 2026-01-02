# kacchiOS - Project Organization Guide

## Quick Navigation

This document helps you understand the project structure and find what you're looking for.

### I want to...

- **See the big picture** → Read [RESTRUCTURING_SUMMARY.md](RESTRUCTURING_SUMMARY.md)
- **Understand the layout** → Read [docs/PROJECT_STRUCTURE.md](docs/PROJECT_STRUCTURE.md)
- **Build and run the OS** → Read [docs/BUILD_GUIDE.md](docs/BUILD_GUIDE.md)
- **Learn about features** → Read [README.md](README.md)
- **Edit kernel code** → Go to `src/kernel/`
- **Add a driver** → Go to `src/drivers/`
- **Understand assembly** → Go to `src/boot/`

## Project Structure at a Glance

```
kacchiOS/
├── src/                          ← All source code
│   ├── boot/                     ← Bootloader (boot.S, sched.S, link.ld)
│   ├── kernel/                   ← Core OS (scheduler, memory, process, kernel)
│   ├── drivers/                  ← Drivers (serial I/O)
│   └── managers/                 ← Reserved for future subsystems
│
├── bin/                          ← Build output (object files) - auto-generated
├── docs/                         ← Documentation
│   ├── PROJECT_STRUCTURE.md      ← Detailed organization
│   └── BUILD_GUIDE.md            ← Building & development
│
├── README.md                     ← Feature overview & quick start
├── RESTRUCTURING_SUMMARY.md      ← Why we reorganized
├── Makefile                      ← Build rules
├── Dockerfile                    ← Docker build environment
└── LICENSE                       ← MIT License
```

## What's in Each Directory?

### `src/boot/` - Bootloader & Initialization
Files that get CPU started:
- `boot.S` - Multiboot bootloader (x86 assembly)
- `sched.S` - Context switching routine (assembly)
- `link.ld` - Memory layout and linker script

### `src/kernel/` - Core Kernel Components
The heart of the OS:
- `kernel.c` - Main kernel + interactive shell
- `scheduler.c/.h` - Task scheduling (cooperative round-robin)
- `memory.c/.h` - Memory allocator (malloc/free)
- `process.c/.h` - Process management
- `string.c/.h` - String utilities
- `types.h` - Type definitions
- `io.h` - I/O port operations

### `src/drivers/` - Device Drivers
Hardware abstractions:
- `serial.c/.h` - Serial port driver (COM1, 38400 baud)

### `bin/` - Build Artifacts
Auto-generated, cleaned with `make clean`:
- `*.o` - Compiled object files
- (not committed to git)

### `docs/` - Documentation
Learn more about the project:
- `PROJECT_STRUCTURE.md` - Detailed module organization
- `BUILD_GUIDE.md` - How to build and debug
- (Add more docs here)

## Common Tasks

### Building the Project

**Using Docker (macOS/Windows):**
```bash
docker build -t kacchios-build --platform linux/amd64 .
docker run --rm -it --platform linux/amd64 -v $(pwd):/workspace \
  kacchios-build bash -c "cd /workspace && make clean && make run"
```

**Using native tools (Linux):**
```bash
make clean
make
make run
```

### Understanding Module Dependencies

```
kernel.c
  ├── includes scheduler.h
  ├── includes memory.h
  ├── includes process.h
  └── includes serial.h
       └── depends on io.h

scheduler.h → types.h
memory.h → types.h
process.h → types.h
serial.h → io.h, types.h
```

### Adding New Features

1. **New Driver?**
   - Create `src/drivers/mydriver.c` and `src/drivers/mydriver.h`
   - Add to Makefile
   - Include in kernel.c if needed

2. **New Kernel Module?**
   - Create `src/kernel/mymodule.c` and `src/kernel/mymodule.h`
   - Add to Makefile
   - Include and initialize in kernel.c

3. **New Documentation?**
   - Create file in `docs/` directory
   - Reference from README.md

## File Locations Quick Reference

| File | Location | Purpose |
|------|----------|---------|
| Bootloader entry | `src/boot/boot.S` | CPU initialization |
| Kernel main | `src/kernel/kernel.c` | OS startup & CLI |
| Scheduler | `src/kernel/scheduler.c` | Task management |
| Memory allocator | `src/kernel/memory.c` | Heap management |
| Process manager | `src/kernel/process.c` | Process lifecycle |
| Serial driver | `src/drivers/serial.c` | COM1 I/O |
| Linker script | `src/boot/link.ld` | Memory layout |
| Build system | `Makefile` | Compilation rules |

## Understanding the Build Process

```
.S Files (Assembly)     → Assemble → .o files
.c Files (C Code)       → Compile  → .o files
                                      ↓
                                   Link
                                      ↓
                                 kernel.elf
                                      ↓
                                    QEMU
                                      ↓
                                  Running OS
```

## Module Responsibilities

| Module | Handles | Key API |
|--------|---------|---------|
| **boot.S** | CPU startup | _start, multiboot header |
| **sched.S** | Register switching | context_switch() |
| **kernel.c** | OS initialization | kmain(), command loop |
| **scheduler.c** | Task scheduling | create_task(), yield() |
| **memory.c** | Heap allocation | malloc(), free() |
| **process.c** | Process lifecycle | proc_create(), proc_exit() |
| **serial.c** | Serial I/O | serial_getc(), serial_puts() |

## CLI Commands Available

Once the OS is running (`kacchiOS> ` prompt):

- `ps` - Show scheduler tasks
- `plist` - Show processes
- `mem` - Show memory statistics
- `memdump` - Dump memory allocations
- `clear` - Clear screen
- `yield` - Manually yield to scheduler
- `exit` - Exit the OS
- `help` - List commands

## Troubleshooting

**Problem: "make: command not found"**
- Solution: Use Docker build (it includes make and gcc)

**Problem: "kernel won't build"**
- Check Makefile CFLAGS has correct include paths
- Verify all .S and .c files are in right directories

**Problem: "QEMU window appears but nothing happens"**
- OS is running, try typing commands
- Use `help` to see available commands

## Recommended Reading Order

1. **[README.md](README.md)** - Get overview of features
2. **[RESTRUCTURING_SUMMARY.md](RESTRUCTURING_SUMMARY.md)** - Understand the organization
3. **[docs/PROJECT_STRUCTURE.md](docs/PROJECT_STRUCTURE.md)** - Deep dive into modules
4. **[docs/BUILD_GUIDE.md](docs/BUILD_GUIDE.md)** - Learn to build and debug
5. **Source code** - Read actual implementation in `src/`

## Key Concepts

### Cooperative Scheduling
Tasks explicitly yield to each other, no preemption.

### Free-List Memory Management
Free blocks are linked together, coalesced when adjacent.

### Process Hierarchy
Parent processes track children, can wait for them.

### Signal Handling
Framework for inter-process communication (extensible).

## Git Tips

Track changes:
```bash
git log --oneline              # See recent commits
git diff                       # See what changed
git status                     # Current state
```

Commit the restructuring:
```bash
git add -A
git commit -m "Reorganize project structure"
```

## References

- **Bootloader**: Multiboot 1.4 specification
- **x86 Assembly**: Intel x86 ISA reference
- **Memory Management**: Kernel-level malloc/free patterns
- **Scheduling**: Round-robin scheduler theory
- **Process Management**: Unix process model

## Support

For questions about:
- **Project structure** → See docs/PROJECT_STRUCTURE.md
- **Building** → See docs/BUILD_GUIDE.md
- **Features** → See README.md
- **Code** → See comments in src/ files

## Next Steps

1. Build the project using Docker or native tools
2. Run `make run` and type `help`
3. Explore the code in `src/kernel/`
4. Read the documentation
5. Extend with new features!

---

**Happy coding!** 🚀
