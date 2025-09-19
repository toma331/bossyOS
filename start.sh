#! /bin/sh

mkdir -p ./build
nasm -f elf32 ./boot/boot.asm -o ./build/boot.o
gcc -m32 -c ./kernel/header.c -o ./build/kernel.o

ld -m elf_i386 -T link.ld -o ./build/kernel.elf ./build/boot.o ./build/kernel.o

cp kernel.elf iso/boot/kernel
grub-mkrescue -o bossy.iso iso
qemu-system-i386 -cdrom bossy.iso
