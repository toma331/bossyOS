#! /bin/sh

mkdir -p ./build
nasm -f elf32 ./boot/boot.asm -o ./build/boot.o

gcc -m32 -c ./kernel/main.c -o ./build/kernel.o
gcc -m32 -c ./kernel/cursor.c -o ./build/cursor.o
gcc -m32 -c ./kernel/ports.c -o ./build/ports.o
gcc -m32 -c ./kernel/screen.c -o ./build/screen.o
gcc -m32 -c ./kernel/shell.c -o ./build/shell.o
gcc -m32 -c ./kernel/variables.c -o ./build/variables.o
gcc -m32 -c ./kernel/fetch.c -o ./build/fetch.o
gcc -m32 -fno-stack-protector -c ./kernel/calc.c -o ./build/calc.o
gcc -m32 -c ./kernel/fs.c -o ./build/fs.o

ld -m elf_i386 -T link.ld -o ./build/kernel.elf ./build/*.o
cp ./build/kernel.elf iso/boot/kernel
grub-mkrescue -o bossy.iso iso
qemu-system-i386 -cdrom bossy.iso
