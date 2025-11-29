#include "headers/cursor.h"
#include "headers/fs.h"
#include "headers/screen.h"

void fetch();
void shell();

void bossyOS(void)
{
	clearScreen();
	enable_cursor(0, 15);
	init_fs();
	print("hello from bossyOS\n\n");
	put_char('\n');
	fetch();
	shell();
}
