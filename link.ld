OUTPUT_FORMAT(elf32-i386)
ENTRY(start)

SECTIONS {
    . = 1M;

    .text : {
        *(.text)
    } :text

    .rodata : {
        *(.rodata*)
    } :rodata

    .data : {
        *(.data)
    } :data

    .bss : {
        *(.bss)
        *(COMMON)
    } :data
}

PHDRS {
    text PT_LOAD FLAGS(5);  
    rodata PT_LOAD FLAGS(4); 
    data PT_LOAD FLAGS(6); 
}

