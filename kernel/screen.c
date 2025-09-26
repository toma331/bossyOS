#include "headers/screen.h"
#include "headers/cursor.h"
#include "headers/variables.h"

// --- вывод на экран ---
void put_char(char c) {
    if (c == '\n') {
        unsigned int line = cursor / (80 * 2);
        cursor = (line + 1) * 80 * 2;
    } else {
        videomemory[cursor] = c;
        videomemory[cursor + 1] = 0x07;
        cursor += 2;
    }
    update_cursor();
}

void print(const char *str) {
    while (*str) {
        put_char(*str++);
    }
}

void clearScreen() {
    for (unsigned int j = 0; j < 80 * 25 * 2; j += 2) {
        videomemory[j] = ' ';
        videomemory[j + 1] = 0x07;
    }
    cursor = 0;
    update_cursor();
}
