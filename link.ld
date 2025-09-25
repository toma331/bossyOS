OUTPUT_FORMAT(elf32-i386)
ENTRY(start)

SECTIONS {
    . = 1M;

    /* Code (read + execute) */
    .text : {
        *(.text)
    } :text

    /* Data (read + write) */
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
    text PT_LOAD FLAGS(5);    /* R + X */
    rodata PT_LOAD FLAGS(4);  /* R */
    data PT_LOAD FLAGS(6);    /* R + W */
}

