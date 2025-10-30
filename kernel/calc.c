#include "headers/calc.h"
#include "headers/cursor.h"
#include "headers/ports.h"
#include "headers/screen.h"
#include "headers/variables.h"
#include <stdint.h>

#define PI 3.141592653589793
#define MAX_EXPR 64

double my_pow(double base, double n) {
	int p = 1;

	for (int i = 1; i <= n; i++) {
		p = base * p;
	}
	return p;
}

double my_sin(double x) {
	double res = 0;
	double term = x;
	double x2 = x * x;
	int sign = 1;
	for (int i = 1; i < 10; i += 2) {
		res += sign * term;
		term *= x2 / ((i + 1) * (i + 2));
		sign = -sign;
	}
	return res;
}

double my_cos(double x) {
	double res = 1;
	double term = 1;
	double x2 = x * x;
	int sign = -1;
	for (int i = 2; i < 10; i += 2) {
		term *= x2 / (i * (i - 1));
		res += sign * term;
		sign = -sign;
	}
	return res;
}

double my_sqrt(double x) {
	if (x <= 0)
		return 0;
	double guess = x / 2.0;
	for (int i = 0; i < 10; i++)
		guess = 0.5 * (guess + x / guess);
	return guess;
}

void print_double(double num) {
	int int_part = (int)num;
	int frac = (int)((num - int_part) * 10000); // 4 знака
	if (frac < 0)
		frac = -frac;

	print_int(int_part);
	put_char('.');
	if (frac == 0) {
		print("0000");
	} else {
		char buf[6];
		int i = 0;
		while (frac > 0 && i < 4) {
			buf[i++] = (frac % 10) + '0';
			frac /= 10;
		}
		for (int j = i; j < 4; j++)
			put_char('0');
		for (int j = i - 1; j >= 0; j--)
			put_char(buf[j]);
	}
}

void print_int(int num) {
	char str[16];
	int i = 0;
	int is_negative = 0;
	if (num == 0) {
		put_char('0');
		return;
	}
	if (num < 0) {
		is_negative = 1;
		num = -num;
	}
	while (num > 0 && i < sizeof(str) - 1) {
		str[i++] = (num % 10) + '0';
		num /= 10;
	}
	if (is_negative)
		put_char('-'); // выводим в обратном порядке
	for (int j = i - 1; j >= 0; j--) {
		put_char(str[j]);
	}
}
	/* основной калькулятор */
	void calc() {
		print("calc> ");
		char expr[MAX_EXPR];
		int expr_len = 0;
		unsigned char last_scancode = 0;
		int shift_pressed = 0;

		while (1) {
			if (inb(0x64) & 1) {
				unsigned char scancode = inb(0x60);
				if (scancode == last_scancode)
					continue;
				last_scancode = scancode;

				if (scancode & 0x80) {
					unsigned char released = scancode & 0x7F;
					if (released == 0x2A || released == 0x36)
						shift_pressed = 0;
					continue;
				}

				if (scancode == 0x2A || scancode == 0x36) {
					shift_pressed = 1;
					continue;
				}

				char c = shift_pressed ? shift_keyboard_map[scancode]
									   : keyboard_map[scancode];
				if (!c)
					continue;

				if (c == '\n') {
					expr[expr_len] = 0;
					put_char('\n');
					break;
				} else if (c == '\b') {
					if (expr_len > 0) {
						expr_len--;
						cursor -= 2;
						videomemory[cursor] = ' ';
						update_cursor();
					}
				} else {
					put_char(c);
					if (expr_len < MAX_EXPR - 1)
						expr[expr_len++] = c;
				}
			}
		}

		/* Простейший парсер:
		   - поддерживает sin(x), cos(x), sqrt(x)
		   - или просто a+b, a-b, a*b, a/b
		*/
		double result = 0;

		if (expr[0] == 's' && expr[1] == 'i' && expr[2] == 'n') {
			double x = 0;
			int i = 4; // после '('
			while (expr[i] >= '0' && expr[i] <= '9') {
				x = x * 10 + (expr[i] - '0');
				i++;
			}
			result = my_sin(x * PI / 180.0); // градусы
		} else if (expr[0] == 'c' && expr[1] == 'o' && expr[2] == 's') {
			double x = 0;
			int i = 4;
			while (expr[i] >= '0' && expr[i] <= '9') {
				x = x * 10 + (expr[i] - '0');
				i++;
			}
			result = my_cos(x * PI / 180.0);
		} else if (expr[0] == 's' && expr[1] == 'q' && expr[2] == 'r' &&
				   expr[3] == 't') {
			double x = 0;
			int i = 5;
			while (expr[i] >= '0' && expr[i] <= '9') {
				x = x * 10 + (expr[i] - '0');
				i++;
			}
			result = my_sqrt(x);
		} else {
			// старый режим: a+b
			int a = 0, b = 0;
			char op = 0;
			int i = 0;
			while (expr[i] >= '0' && expr[i] <= '9') {
				a = a * 10 + (expr[i] - '0');
				i++;
			}
			op = expr[i++];
			while (expr[i] >= '0' && expr[i] <= '9') {
				b = b * 10 + (expr[i] - '0');
				i++;
			}

			switch (op) {
			case '+':
				result = a + b;
				break;
			case '-':
				result = a - b;
				break;
			case '*':
				result = a * b;
				break;
			case '/':
				if (b == 0) {
					print("Error: divide by zero\n");
					return;
				}
				result = (double)a / b;
				break;
			case '^':
				result = my_pow(a, b);
				break;
			default:
				print("Invalid operator\n");
				return;
			}
		}

		print("= ");
		print_double(result);
		put_char('\n');
	}
