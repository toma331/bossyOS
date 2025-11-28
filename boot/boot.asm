bits 32

section .text
    align 4 
    dd 0x1BADB002        
    dd 0x00             
    dd -(0x1BADB002+0x00)

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

