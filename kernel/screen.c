#include "headers/screen.h"
#include "headers/cursor.h"
#include "headers/variables.h"

void put_char(char c)
{
	if (c == '\n')
	{
		unsigned int line = cursor / (80 * 2);
		cursor = (line + 1) * 80 * 2;
	}
	else
	{
		videomemory[cursor] = c;
		videomemory[cursor + 1] = (0x01 << 4) | 0x07;
		cursor += 2;
	}
	update_cursor();
}

void print(const char *str)
{
	while (*str)
	{
		put_char(*str++);
	}
}

void clearScreen()
{
	for (unsigned int j = 0; j < 80 * 25 * 2; j += 2)
	{
		videomemory[j] = ' ';
		videomemory[j + 1] = (0x01 << 4) | 0x07;
	}
	cursor = 0;
	update_cursor();
}

int strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return *(unsigned char *)s1 - *(unsigned char *)s2;
}

int strncmp(const char *s1, const char *s2, size_t n)
{
	while (n && *s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
		n--;
	}

	if (n == 0)
		return 0;

	return (unsigned char)*s1 - (unsigned char)*s2;
}
