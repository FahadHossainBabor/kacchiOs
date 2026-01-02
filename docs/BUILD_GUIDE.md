# Build and Development Guide

## Quick Start

### Prerequisites

- Docker (for cross-compilation on macOS/Windows)
- OR Linux with x86 GCC toolchain installed

### Building kacchiOS

#### Option 1: Docker (Recommended for macOS/Windows)

```bash
# Build the Docker image (one time only)
docker build -t kacchios-build --platform linux/amd64 .

# Build and run the OS
docker run --rm -it --platform linux/amd64 -v $(pwd):/workspace \
  kacchios-build bash -c "cd /workspace && make clean && make run"
```

When the prompt appears:
```
kacchiOS> 
```

Try these commands:
- `help` - List all commands
- `ps` - Show scheduler tasks
- `plist` - Show processes
- `mem` - Show memory stats
- `clear` - Clear screen
- `exit` - Exit the OS

#### Option 2: Linux (Native Build)

On Linux with x86 development tools:

```bash
# Install required packages (Ubuntu/Debian)
sudo apt-get install build-essential qemu-system-x86 gcc-multilib binutils-i686-linux-gnu

# Build
cd kacchiOS
make clean
make run
```

## Makefile Targets

### Build Targets

| Target | Description |
|--------|-------------|
| `make` or `make all` | Build kernel.elf |
| `make clean` | Remove build artifacts |

### Execution Targets

| Target | Description |
|--------|-------------|
| `make run` | Execute in QEMU (serial output only) |
| `make run-vga` | Execute in QEMU with VGA window |
| `make debug` | Execute in QEMU ready for GDB debugging |

### Example: Full Build Cycle

```bash
make clean      # Remove old artifacts
make            # Compile all source files
make run        # Execute in emulator
```

## Debugging

### Using GDB

Terminal 1 (start QEMU in debug mode):
```bash
make debug
```

Terminal 2 (connect with GDB):
```bash
gdb kernel.elf
(gdb) target remote localhost:1234
(gdb) break kmain
(gdb) continue
```

### Useful GDB Commands

```gdb
break main              # Set breakpoint at main
continue               # Resume execution
step                   # Step into function
next                   # Step over function
print variable_name    # Print variable value
backtrace              # Show call stack
disassemble            # Show assembly
quit                   # Exit GDB
```

## Project File Organization

### Source Files

```
src/
├── boot/                  # Bootloader and linker script
│   ├── boot.S             # x86 bootloader
│   ├── sched.S            # Assembly context switching
│   └── link.ld            # Linker script
│
├── kernel/                # Core kernel modules
│   ├── kernel.c           # Main kernel entry
│   ├── scheduler.c/.h     # Task scheduler
│   ├── memory.c/.h        # Memory allocator
│   ├── process.c/.h       # Process manager
│   ├── string.c/.h        # String utilities
│   ├── types.h            # Type definitions
│   └── io.h               # I/O port macros
│
└── drivers/               # Device drivers
    └── serial.c/.h        # Serial I/O driver
```

### Generated Files

```
bin/                      # Object files (*.o) - removed by 'make clean'
kernel.elf                # Final executable kernel
```

### Documentation

```
docs/
├── PROJECT_STRUCTURE.md   # Directory organization
└── BUILD_GUIDE.md         # This file
```

## Compiler Configuration

The build uses:
- **Compiler**: GCC (32-bit mode)
- **Assembler**: GNU as (GAS)
- **Linker**: GNU ld
- **Flags**: See Makefile for details

## Troubleshooting

### Error: "clang: error: unknown argument: '--32'"

**Cause**: Using Xcode's clang instead of GNU toolchain

**Solution**: Use Docker build:
```bash
docker build -t kacchios-build --platform linux/amd64 .
docker run --rm -it --platform linux/amd64 -v $(pwd):/workspace \
  kacchios-build bash -c "cd /workspace && make clean && make run"
```

### Error: "qemu-system-i386: command not found"

**Cause**: QEMU not installed

**Solution**: Install QEMU:
- macOS: `brew install qemu`
- Linux: `sudo apt-get install qemu-system-x86`
- Windows: Download from qemu.org

### Build Error: "fatal error: xxx.h: No such file or directory"

**Cause**: Include path not set correctly

**Solution**: Check Makefile CFLAGS has `-I$(SRCDIR)/kernel`

## Extending the Build

### Adding a New C Source File

1. Create the file in appropriate `src/` subdirectory
2. Add compilation rule to Makefile:
   ```makefile
   $(BINDIR)/myfile.o: $(DIR)/myfile.c
       $(CC) $(CFLAGS) -c $< -o $@
   ```
3. Add to OBJS list:
   ```makefile
   OBJS = ... $(BINDIR)/myfile.o ...
   ```

### Adding a New Assembly File

1. Create the file in `src/boot/`
2. Add compilation rule:
   ```makefile
   $(BINDIR)/myfile.o: $(BOOTDIR)/myfile.S
       $(AS) $(ASFLAGS) $< -o $@
   ```
3. Add to OBJS list

### Changing Compiler Flags

Edit the CFLAGS or ASFLAGS in the Makefile:
```makefile
CFLAGS = -m32 -ffreestanding -O2 ...  # Add/modify flags here
```

## Docker Build Details

The Dockerfile:
- Uses `ubuntu:22.04` as base image
- Installs build-essential (GCC, binutils, make)
- Installs qemu-system-x86 for emulation
- Works on both x86 and ARM64 hosts (with `--platform linux/amd64`)

Build the Docker image once:
```bash
docker build -t kacchios-build --platform linux/amd64 .
```

Then use it repeatedly:
```bash
docker run --rm -it --platform linux/amd64 -v $(pwd):/workspace \
  kacchios-build bash -c "cd /workspace && make clean && make run"
```

## Performance Tips

### Faster Rebuilds

Clean and rebuild only changed files:
```bash
make run  # Recompiles only changed source files
```

### Faster QEMU Execution

The default build runs in `-display none` mode (headless):
```bash
make run      # Faster (no GUI)
make run-vga  # Slower (with VGA window)
```

## Continuous Integration

To automate builds, you can create a CI script:

```bash
#!/bin/bash
set -e  # Exit on error
make clean
make
echo "Build successful!"
```

Or use Docker in a CI/CD pipeline:
```yaml
script:
  - docker build -t kacchios-build --platform linux/amd64 .
  - docker run --rm -v $(pwd):/workspace kacchios-build \
      bash -c "cd /workspace && make clean && make"
```

## Next Steps

- Read [PROJECT_STRUCTURE.md](PROJECT_STRUCTURE.md) for directory organization
- Review source code comments in `src/` files
- Study the Makefile for build configuration
- Experiment with extending drivers and subsystems
