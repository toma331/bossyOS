ASM     = nasm
CC      = gcc
LD      = ld
GRUB    = grub-mkrescue
CFLAGS  = -m32 -c -Wall -Wextra -fno-pie -fno-stack-protector -fno-builtin -fno-asynchronous-unwind-tables
ASFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T link.ld

BUILD   = build
ISO     = bossy.iso
KERNEL  = $(BUILD)/kernel.elf
BOOT    = $(BUILD)/boot.o

C_SOURCES = $(wildcard kernel/*.c)
C_OBJS    = $(patsubst kernel/%.c, $(BUILD)/%.o,$(C_SOURCES))

OBJS = $(BOOT) $(C_OBJS)

all: $(ISO)

$(BUILD):
	mkdir -p $(BUILD)

$(BOOT): boot/boot.asm | $(BUILD)
	$(ASM) $(ASFLAGS) $< -o $@

$(BUILD)/%.o: kernel/%.c | $(BUILD)
	$(CC) $(CFLAGS) $< -o $@

$(KERNEL): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

$(ISO): $(KERNEL)
	cp $(KERNEL) iso/boot/kernel
	$(GRUB) -o $@ iso

clean:
	rm -rf $(BUILD) $(ISO)
	rm iso/boot/kernel

.PHONY: all clean
