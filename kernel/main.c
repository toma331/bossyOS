#include "headers/screen.h"
#include "headers/cursor.h"
#include "headers/shell.h"

void bossyOS(void) {
    clearScreen();
    enable_cursor(0, 15);
    print("hello from bossyOS\n\n");
    shell();
}
