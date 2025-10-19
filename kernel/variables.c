#include "headers/variables.h"

unsigned int cursor = 0;
char *videomemory = (char*)0xb8000;

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
