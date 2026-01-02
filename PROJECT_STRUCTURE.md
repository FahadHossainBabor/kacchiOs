# kacchiOS Project Structure

## 📁 Complete Directory Organization

```
kacchiOS/
│
├── src/                           # 🔧 SOURCE CODE (Core Development)
│   ├── boot/                      # CPU initialization & bootloader
│   │   ├── boot.S                 # x86 Multiboot bootloader (ASM)
│   │   ├── sched.S                # Context switching routine (ASM)
│   │   └── link.ld                # Linker script for kernel layout
│   │
│   ├── kernel/                    # Core OS subsystems
│   │   ├── kernel.c               # Main kernel entry + CLI shell
│   │   ├── scheduler.c/.h         # Cooperative scheduler (must-have)
│   │   ├── memory.c/.h            # Dynamic heap allocator (must-have)
│   │   ├── process.c/.h           # Process manager (must-have)
│   │   ├── string.c/.h            # String utilities
│   │   ├── types.h                # Type definitions
│   │   └── io.h                   # I/O port macros
│   │
│   ├── drivers/                   # Hardware device drivers
│   │   ├── serial.c/.h            # Serial port (COM1) driver
│   │   └── [future drivers]
│   │
│   └── managers/                  # (Reserved for future subsystems)
│
├── bin/                           # 📦 BUILD OUTPUT (Generated)
│   ├── boot.o                     # Compiled bootloader
│   ├── kernel.o                   # Compiled kernel
│   ├── serial.o                   # Compiled driver
│   ├── scheduler.o                # Compiled scheduler
│   ├── memory.o                   # Compiled memory manager
│   ├── process.o                  # Compiled process manager
│   └── [other .o files]
│
├── config/                        # ⚙️ CONFIGURATION & DOCS
│   ├── Dockerfile                 # Docker container definition
│   ├── LICENSE                    # MIT License
│   ├── ORGANIZATION.md            # Project organization notes
│   ├── RESTRUCTURING_SUMMARY.md   # Restructuring changes
│   └── OS_LAB_Project.pdf         # Project specification (if available)
│
├── docs/                          # 📚 DOCUMENTATION
│   ├── BUILD_GUIDE.md             # Comprehensive build guide
│   ├── PROJECT_STRUCTURE.md       # This file
│   └── [future documentation]
│
├── Makefile                       # 🔨 BUILD SYSTEM (Root level)
├── Readme.md                      # Quick start + feature overview
├── kernel.elf                     # Final bootable kernel (generated)
├── .git/                          # Git version control
└── .gitignore                     # Git ignore rules

```

## 📂 Directory Purposes

| Directory | Purpose | Contains |
|-----------|---------|----------|
| `src/boot/` | CPU initialization & bootloader | Multiboot boot code, ASM context switching, linker script |
| `src/kernel/` | Core OS subsystems | Scheduler, memory, process managers, string utils, types |
| `src/drivers/` | Hardware device drivers | Serial driver, future: keyboard, disk, network drivers |
| `src/managers/` | Future subsystems | Reserved for extensions |
| `bin/` | Compiled object files | .o files generated during build (auto-generated) |
| `config/` | Configuration & metadata | Dockerfile, LICENSE, organization docs, PDF specs |
| `docs/` | Project documentation | Build guides, architecture docs, tutorials |

## 🔄 Build Flow

```
Source Code (src/)
       ↓
    Make (Makefile)
       ↓
  Compile & Assemble
       ↓
 Object Files (bin/)
       ↓
     Link (ld)
       ↓
  kernel.elf
       ↓
    QEMU/Run
```

## 📋 File Responsibilities

### Source Code Organization

**src/boot/**
- `boot.S` — x86 Multiboot bootloader entry point
- `sched.S` — Low-level context switching (register save/restore)
- `link.ld` — Memory layout and section organization

**src/kernel/**
- `kernel.c` — Main kernel loop, CLI shell, manager initialization
- `scheduler.c/.h` — Task scheduling, context management, yielding
- `memory.c/.h` — Dynamic heap allocation (malloc/free/realloc)
- `process.c/.h` — Process lifecycle, parent-child relationships, signals
- `string.c/.h` — String operations (strlen, strcmp, strcpy)
- `types.h` — Basic type definitions (uint32_t, etc.)
- `io.h` — Hardware port I/O macros

**src/drivers/**
- `serial.c/.h` — COM1 serial driver (communication)

### Configuration

**config/**
- `Dockerfile` — Docker image definition for reproducible builds
- `LICENSE` — MIT License
- `ORGANIZATION.md` — Project principles and organization notes
- `RESTRUCTURING_SUMMARY.md` — Summary of directory restructuring
- `OS_LAB_Project.pdf` — Original project specification

### Documentation

**docs/**
- `BUILD_GUIDE.md` — Complete build instructions and troubleshooting
- `PROJECT_STRUCTURE.md` — This file

### Build Output

**bin/** (auto-generated)
- `.o` files — Compiled object files
- Not tracked in git

### Root Level

- `Makefile` — Main build automation
- `Readme.md` — Quick start guide and feature overview
- `kernel.elf` — Final bootable kernel image
- `.git/` — Git repository

## 🛠️ Adding New Files

### Adding a New Source File

1. Create in appropriate `src/` subdirectory
2. Update `Makefile` OBJS list if needed

Example:
```makefile
OBJS += $(BINDIR)/mynewdriver.o
$(BINDIR)/mynewdriver.o: src/drivers/mynewdriver.c
    $(CC) $(CFLAGS) -c $< -o $@
```

### Adding Documentation

1. Create `.md` file in `docs/`
2. Reference in `Readme.md` or other docs

### Adding Configuration

1. Store in `config/` directory
2. Document in `config/ORGANIZATION.md`

## 📊 Module Dependencies

```
kernel.c (CLI shell)
    ├── scheduler.c (task management)
    ├── memory.c (heap allocation)
    ├── process.c (process management)
    ├── string.c (string utilities)
    └── serial.c (communication)

scheduler.c
    └── sched.S (context switching)

memory.c
    └── (no dependencies)

process.c
    ├── memory.c (stack allocation)
    └── scheduler.c (task scheduling)

serial.c
    └── (hardware only)
```

## 🚀 Build Process

### Complete Build Command

```bash
docker build -t kacchios-build --platform linux/amd64 .
docker run --rm -it --platform linux/amd64 -v $(pwd):/workspace kacchios-build make
```

### What Happens

1. Read `src/` files
2. Compile with `gcc -m32` → `bin/*.o`
3. Assemble with `as --32` → `bin/*.o`
4. Link with `ld` using `src/boot/link.ld` → `kernel.elf`
5. Run in QEMU emulator

## 📦 Key Statistics

- **Bootloader**: ~200 lines (boot.S)
- **Scheduler**: ~176 lines (scheduler.c)
- **Memory Manager**: ~200 lines (memory.c)
- **Process Manager**: ~188 lines (process.c)
- **Serial Driver**: ~100 lines (serial.c)
- **Kernel**: ~122 lines (kernel.c)
- **Total**: ~1200 lines of code

## 🔐 File Permissions

```
src/        → Source code (read-only for students in assignments)
bin/        → Generated files (git-ignored)
config/     → Configuration (reference)
docs/       → Documentation (read-only)
```

## ✅ Verification Checklist

- [ ] All .c files in src/ have corresponding .h headers
- [ ] Makefile includes all source directories: -I$(SRCDIR)/kernel -I$(SRCDIR)/drivers
- [ ] No object files in root directory (should be in bin/)
- [ ] kernel.elf exists after successful build
- [ ] Docker image builds without errors
- [ ] QEMU runs kernel successfully
- [ ] CLI commands work (ps, plist, mem, etc.)

---

**Last Updated**: January 2, 2026  
**Version**: 1.0 - Complete Restructuring
