#include "headers/screen.h"
#include "headers/cursor.h"
#include "headers/shell.h"
#include "headers/fetch.h"
#include "headers/fs.h"

void bossyOS(void) {
    clearScreen();
    enable_cursor(0, 15);
    init_fs();
    print("hello from bossyOS\n\n");
    put_char('\n');
    fetch();
    shell();
}
