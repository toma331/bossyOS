#include "headers/variables.h"
#include "headers/ports.h"
#include "headers/screen.h"

#include <stddef.h>

size_t strcspn(const char *s, const char *reject) {
    const char *p, *r;

    for (p = s; *p; p++) {
        for (r = reject; *r; r++) {
            if (*p == *r)
                return p - s;
        }
    }

    return p - s;
}

char cgetch() {
    unsigned char scancode = 0;
    char c = 0;

    // Ждём, пока клавиша нажата (бит 0 в порту 0x64)
    do {
        scancode = inb(0x60);
    } while (scancode & 0x80); // игнорируем отпускание клавиши (release event)

    c = keyboard_map[scancode];
    return c;
}

void kgets(char *buf, int max_len) {
    int i = 0;
    while (i < max_len - 1) {
        char c = cgetch(); // Ваша функция чтения символа с клавиатуры
        if (c == '\n' || c == '\r') {
            buf[i] = '\0';
            put_char('\n');
            return;
        } else if (c == '\b' && i > 0) {
            i--;
            put_char('\b');
        } else if (c >= 32 && c <= 126) { // печатаемые символы
            buf[i++] = c;
            put_char(c);
        }
    }
    buf[i] = '\0';
}
