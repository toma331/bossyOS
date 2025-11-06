#include "headers/calc.h"
#include "headers/screen.h"
#include "headers/ports.h"
#include "headers/variables.h"
#include "headers/cursor.h"

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
        put_char('-');

    // выводим в обратном порядке
    for (int j = i - 1; j >= 0; j--) {
        put_char(str[j]);
    }
}

void calc() {
    print("Enter expression (example: 2+2): ");

    char expr[64];
    int expr_len = 0;
    unsigned char last_scancode = 0;
    static int shift_pressed = 0;

    while (1) {
        if (inb(0x64) & 1) {
            unsigned char scancode = inb(0x60);

            // Игнорируем повторные коды
            if (scancode == last_scancode)
                continue;
            last_scancode = scancode;

            // Отпуск клавиши
            if (scancode & 0x80) {
                unsigned char released = scancode & 0x7F;
                if (released == 0x2A || released == 0x36)
                    shift_pressed = 0;
                continue;
            }

            // Shift нажат
            if (scancode == 0x2A || scancode == 0x36) {
                shift_pressed = 1;
                continue;
            }

            // Получаем символ из таблицы
            char c = shift_pressed ? shift_keyboard_map[scancode]
                                   : keyboard_map[scancode];

            if (!c) continue;

            // Обработка символов
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
                expr[expr_len++] = c;
            }
        }
    }

    // парсер "число оператор число"
    int a = 0, b = 0;
    char op = 0;
    int i = 0;

    // парсим первое число
    while (expr[i] >= '0' && expr[i] <= '9') {
        a = a * 10 + (expr[i] - '0');
        i++;
    }

    // оператор
    op = expr[i++];

    // второе число
    while (expr[i] >= '0' && expr[i] <= '9') {
        b = b * 10 + (expr[i] - '0');
        i++;
    }

    int result = 0;
    switch (op) {
        case '+': result = a + b; break;
        case '-': result = a - b; break;
        case '*': result = a * b; break;
        case '/':
            if (b == 0) {
                print("Error: divide by zero\n");
                goto done_calc;
            }
            result = a / b;
            break;
        default:
            print("Invalid operator\n");
            goto done_calc;
    }

    print("Result: ");
    print_int(result);
    put_char('\n');

done_calc:
    ;
}
