#include "headers/ports.h"
#include "headers/variables.h"

// курсор VGA
void set_cursor(unsigned short index) {
    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char)(index & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char)((index >> 8) & 0xFF));
}

void enable_cursor(unsigned char start, unsigned char end) {
    outb(0x3D4, 0x0A);
    outb(0x3D5, (inb(0x3D5) & 0xC0) | start);

    outb(0x3D4, 0x0B);
    outb(0x3D5, (inb(0x3D5) & 0xE0) | end);
}

void update_cursor() {
    set_cursor(cursor / 2);
}
