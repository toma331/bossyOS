#include "screen.h"
#include "cursor.h"
#include "shell.h"

void bossyOS(void) {
    clearScreen();
    enable_cursor(0, 15);
    print("hello from bossyOS\n\n");
    shell();
}