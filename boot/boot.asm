bits 32

section .text
    align 4 
    dd 0x1BADB002        ; magic number (multiboot)
    dd 0x00              ; flags
    dd -(0x1BADB002+0x00); checksum

global start
extern bossyOS

start:
    cli
    mov esp, stack_space
    call bossyOS
    hlt

section .bss
    resb 8192
stack_space:

