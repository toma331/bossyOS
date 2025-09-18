char *videomemory = (char*)0xb8000;
unsigned int cursor = 0;

// --- работа с портами ---
unsigned char inb(unsigned short port) {
    unsigned char r;
    __asm__ __volatile__("inb %1, %0" : "=a"(r) : "Nd"(port));
    return r;
}

void outb(unsigned short port, unsigned char val) {
    __asm__ __volatile__("outb %0, %1" : : "a"(val), "Nd"(port));
}

// --- курсор VGA ---
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

// --- клавиатура ---
unsigned char keyboard_map[128] = {
    0, 27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n', 0,
    'a','s','d','f','g','h','j','k','l',';','\'','`', 0,'\\',
    'z','x','c','v','b','n','m',',','.','/', 0, '*', 0,' ',
};

// --- массив команд ---
char commands[5][128] = {
    "help",
    "hi",
    "fetch",
    "clear",
    "poweroff",
};

// --- strcmp ---
int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

// --- bossyOS fetch ---
void fetch() {
    print("   ____                 \n");
    print("  | __ )  ___  ___  ___ \n");
    print("  |  _ \\ / _ \\/ __|/ _ \\\n");
    print("  | |_) | (_) \\__ \\  __/\n");
    print("  |____/ \\___/|___/\\___|\n");
    print("\n");
    print(" bossyOS v0.1 (toy kernel)\n");
    print(" VGA Text Mode 80x25\n");
    print(" Author: Artem Koval :)\n");
    print("\n");
}

// --- outw ---
static inline void outw(unsigned short port, unsigned short val) {
    asm volatile ( "outw %0, %1" : : "a"(val), "Nd"(port) );
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

void bossyOS(void) {
    clearScreen();
    enable_cursor(0, 15);
    print("hello from bossyOS\n\n");
    shell();
}

