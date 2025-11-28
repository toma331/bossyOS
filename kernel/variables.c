#include "headers/variables.h"

unsigned int cursor = 0;
char *videomemory = (char*)0xb8000;

int shift_pressed = 0;
unsigned char last_scancode = 0;

unsigned char keyboard_map[128] = {
    0, 27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n', 0,
    'a','s','d','f','g','h','j','k','l',';','\'','`', 0,'\\',
    'z','x','c','v','b','n','m',',','.','/', 0, '*', 0,' ',
};

char shift_keyboard_map[128] = {
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*', // цифры с Shift
    '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"',
    '~', 0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?',
    0, '*', 0, ' ', 0
};


char commands[9][128] = {
    "help",
    "hi",
    "fetch",
    "clear",
    "poweroff",
    "calc",
    "touch",
    "mkdir",
    "ls",
};
