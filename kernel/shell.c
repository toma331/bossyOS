#include "headers/calc.h"
#include "headers/cursor.h"
#include "headers/fs.h"
#include "headers/ports.h"
#include "headers/screen.h"
#include "headers/variables.h"

#include <stddef.h>

void fetch();

void shell()
{
	char buffer[128];
	int len = 0;
	last_scancode = 0;
	shift_pressed = 0;

	print("--> ");

	while (1)
	{
		if (inb(0x64) & 1)
		{
			unsigned char scancode = inb(0x60);

			if (scancode & 0x80)
			{
				unsigned char released = scancode & 0x7F;
				if (released == 0x2A || released == 0x36)
				{
					shift_pressed = 0;
				}
				last_scancode = 0;
				continue;
			}

			if (scancode == last_scancode)
				continue;
			last_scancode = scancode;

			if (scancode == 0x2A || scancode == 0x36)
			{
				shift_pressed = 1;
				continue;
			}

			char c = shift_pressed ? shift_keyboard_map[scancode]
								   : keyboard_map[scancode];
			if (!c)
				continue;

			if (c == '\b')
			{
				if (len > 0)
				{
					len--;
					if (cursor >= 2)
						cursor -= 2;
					videomemory[cursor] = ' ';
					update_cursor();
				}
			}
			else if (c == '\n' || c == '\r')
			{
				buffer[len] = '\0';
				put_char('\n');

				if (strcmp(buffer, "help") == 0)
				{
					print("Commands:\n");
					for (int j = 0; j < 6; j++)
					{
						print(commands[j]);
						put_char('\n');
					}
					put_char('\n');
					print("[  FS COMMANDS  ]");
					for (int i = 6; i < 9; i++)
					{
						put_char('\n');
						print(commands[i]);
					}
					put_char('\n');
				}
				else if (strcmp(buffer, "poweroff") == 0)
				{
					print("Powering off...\n");
					outw(0x604, 0x2000);
					for (;;)
						;
				}
				else if (strcmp(buffer, "clear") == 0)
				{
					clearScreen();
				}
				else if (strcmp(buffer, "hi") == 0)
				{
					print("Hello from bossyOS shell!\n");
				}
				else if (strcmp(buffer, "fetch") == 0)
				{
					fetch();
				}
				else if (strncmp(buffer, "ls", 2) == 0 &&
						 (buffer[2] == '\0' || buffer[2] == ' '))
				{
					ls_fs();
				}
				else if (strncmp(buffer, "mkdir ", 6) == 0)
				{
					char *arg = buffer + 6;
					while (*arg == ' ')
						arg++;
					if (*arg)
						mkdir_fs(arg);
					else
						print("mkdir: missing operand\n");
				}
				else if (strncmp(buffer, "touch ", 6) == 0)
				{
					char *arg = buffer + 6;
					while (*arg == ' ')
						arg++;
					if (*arg)
						touch_fs(arg);
					else
						print("touch: missing operand\n");
				}
				else if (strcmp(buffer, "calc") == 0)
				{
					calc();
				}
				else if (strcmp(buffer, "") == 0)
				{
				}
				else
				{
					print("Unknown command\n");
				}

				len = 0;
				buffer[0] = '\0';

				put_char('\n');
				print("--> ");
			}
			else
			{
				if (len < (int)sizeof(buffer) - 1)
				{
					put_char(c);
					buffer[len++] = c;
				}
				else
				{
				}
			}
		}
	}
}
