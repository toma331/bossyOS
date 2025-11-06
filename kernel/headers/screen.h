#include <stddef.h>

#ifndef SCREEN
#define SCREEN

void put_char(char c);
void print(const char *str);
void clearScreen();
int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);

#endif
