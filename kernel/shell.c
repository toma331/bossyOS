#include "headers/calc.h"
#include "headers/cursor.h"
#include "headers/fetch.h"
#include "headers/ports.h"
#include "headers/screen.h"
#include "headers/variables.h"

static int shift_pressed = 0;
unsigned char last_scancode = 0;

// --- shell ---
void shell() {
	char buffer[128];
	int len = 0;

	print("bossyOS> ");

	while (1) {
		if (inb(0x64) & 1) {
			unsigned char scancode = inb(0x60);

			// игнорируем повторное чтение того же кода
			if (scancode == last_scancode)
				continue;
			last_scancode = scancode;

			// клавиша отпущена?
			if (scancode & 0x80) {
				unsigned char released = scancode & 0x7F;

				// если отпущен Shift
				if (released == 0x2A || released == 0x36) {
					shift_pressed = 0;
				}
				continue; // не печатаем break-коды!
			}

			// клавиша нажата
			if (scancode == 0x2A || scancode == 0x36) { // Shift
				shift_pressed = 1;
				continue;
			}

			char c;
			if (shift_pressed)
				c = shift_keyboard_map[scancode];
			else
				c = keyboard_map[scancode];

			if (!c)
				continue;

			// обработка символа
			if (c == '\b') {
				if (len > 0) {
					len--;
					cursor -= 2;
					videomemory[cursor] = ' ';
					update_cursor();
				}
			} else if (c == '\n') {
				buffer[len] = 0;
				put_char('\n');

				// --- команды ---
				if (!strcmp(buffer, "help")) {
					print("Commands: ");
					put_char('\n');
					put_char('\n');
					for (int j = 0; j < 6; j++) {
						print(commands[j]);
						put_char('\n');
					}
					put_char('\n');
				} else if (!strcmp(buffer, "poweroff")) {
					print("Powering off...\n");
					outw(0x604, 0x2000); // QEMU поймёт и выключится
					for (;;)
						; // на случай, если не сработало
				} else if (!strcmp(buffer, "clear")) {
					clearScreen();
				} else if (!strcmp(buffer, "hi")) {
					print("Hello from bossyOS shell!\n");
				} else if (!strcmp(buffer, "fetch")) {
					fetch();
				} else if (!strcmp(buffer, "calc")) {
					calc();
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
