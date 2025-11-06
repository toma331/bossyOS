#include <stddef.h>

#ifndef LIB_H
#define LIB_H

size_t strcspn(const char *s, const char *reject);
char cgetch();
void kgets(char *buf, int max_len);

#endif
