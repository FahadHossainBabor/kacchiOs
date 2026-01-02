# Makefile for kacchiOS
CC = gcc
LD = ld
AS = as

SRCDIR = src
BOOTDIR = $(SRCDIR)/boot
KERNELDIR = $(SRCDIR)/kernel
DRIVERDIR = $(SRCDIR)/drivers
BINDIR = bin

CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra -nostdinc \
         -fno-builtin -fno-stack-protector -I$(SRCDIR)/kernel -I$(SRCDIR)/drivers -I$(SRCDIR)/managers -I$(SRCDIR)
ASFLAGS = --32
LDFLAGS = -m elf_i386

OBJS = $(BINDIR)/boot.o $(BINDIR)/kernel.o $(BINDIR)/serial.o \
       $(BINDIR)/string.o $(BINDIR)/sched.o $(BINDIR)/scheduler.o \
       $(BINDIR)/memory.o $(BINDIR)/process.o

all: kernel.elf

kernel.elf: $(OBJS)
	$(LD) $(LDFLAGS) -T $(BOOTDIR)/link.ld -o $@ $^

$(BINDIR)/boot.o: $(BOOTDIR)/boot.S
	@mkdir -p $(BINDIR)
	$(AS) $(ASFLAGS) $< -o $@

$(BINDIR)/kernel.o: $(KERNELDIR)/kernel.c
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BINDIR)/serial.o: $(DRIVERDIR)/serial.c
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BINDIR)/string.o: $(KERNELDIR)/string.c
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BINDIR)/sched.o: $(BOOTDIR)/sched.S
	@mkdir -p $(BINDIR)
	$(AS) $(ASFLAGS) $< -o $@

$(BINDIR)/scheduler.o: $(KERNELDIR)/scheduler.c
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BINDIR)/memory.o: $(KERNELDIR)/memory.c
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BINDIR)/process.o: $(KERNELDIR)/process.c
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -c $< -o $@

run: kernel.elf
	qemu-system-i386 -kernel kernel.elf -m 64M -serial stdio -display none

run-vga: kernel.elf
	qemu-system-i386 -kernel kernel.elf -m 64M -serial mon:stdio

debug: kernel.elf
	qemu-system-i386 -kernel kernel.elf -m 64M -serial stdio -display none -s -S &
	@echo "Waiting for GDB connection on port 1234..."
	@echo "In another terminal run: gdb -ex 'target remote localhost:1234' -ex 'symbol-file kernel.elf'"

clean:
	rm -f $(BINDIR)/*.o kernel.elf

.PHONY: all run run-vga debug clean