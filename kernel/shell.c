#include "headers/screen.h"
#include "headers/ports.h"
#include "headers/variables.h"
#include "headers/cursor.h"
#include "headers/fetch.h"

// --- strcmp ---
int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

// --- shell ---
void shell() {
    char buffer[128];
    int len = 0;

    print("bossyOS> ");

    while (1) {
        if (inb(0x64) & 1) {
            unsigned char scancode = inb(0x60);

            if (!(scancode & 0x80)) {
                char c = keyboard_map[scancode];
                if (c) {
                    if (c == '\b') {
                        if (len > 0) {
                            len--;
                            cursor -= 2;
                            videomemory[cursor] = ' ';
                            update_cursor();
                        }
                    } else if (c == '\n') {
                        buffer[len] = 0;
                        print("\n");

                        // --- команды ---
                        if (!strcmp(buffer, "help")) {
                            print("Commands: ");
                            put_char('\n');
                            put_char('\n');
                            for (int j = 0; j < 5; j++) {
                                print(commands[j]); 
                                put_char('\n');
                            }
                            put_char('\n');
                        }else if (!strcmp(buffer, "poweroff")) {
                            print("Powering off...\n");
                            outw(0x604, 0x2000); // QEMU поймёт и выключится
                            for (;;); // на случай, если не сработало
                        }else if (!strcmp(buffer, "clear")) {
                            clearScreen();
                        } else if (!strcmp(buffer, "hi")) {
                            print("Hello from bossyOS shell!\n");
                        } else if (!strcmp(buffer, "fetch")) {
                            fetch();
                        } else {
                            print("Unknown command\n");
                        }

                        len = 0;
                        print("bossyOS> ");
                    } else {
                        put_char(c);
                        buffer[len++] = c;
                    }
                }
            }
        }
    }
}
