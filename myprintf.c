#include <unistd.h>
#include <stdarg.h>

static int print_char(char c) {
    write(1, &c, 1);
    return 1;
}

static int print_str(char *s) {
    int count = 0;
    if (!s) s = "(null)";
    while (*s) {
        write(1, s, 1);
        s++;
        count++;
    }
    return count;
}

static int print_number(long n, int base, int is_signed) {
    char buf[32];
    char *digits = "0123456789abcdef";
    unsigned long num;
    int i = 0, count = 0;

    if (is_signed && n < 0) {
        count += print_char('-');
        num = -n;
    } else {
        num = (unsigned long)n;
    }

    if (num == 0) {
        return count + print_char('0');
    }

    while (num > 0) {
        buf[i++] = digits[num % base];
        num /= base;
    }
    while (i--) {
        count += print_char(buf[i]);
    }
    return count;
}

static int print_pointer(void *p) {
    unsigned long addr = (unsigned long)p;
    int count = 0;
    count += write(1, "0x", 2);
    count += print_number(addr, 16, 0);
    return count;
}

int my_printf(char *restrict format, ...) {
    va_list args;
    int count = 0;
    char *p = format;

    va_start(args, format);
    while (*p) {
        if (*p == '%') {
            p++;
            if (*p == 'd')
                count += print_number(va_arg(args, int), 10, 1);
            else if (*p == 'u')
                count += print_number(va_arg(args, unsigned int), 10, 0);
            else if (*p == 'o')
                count += print_number(va_arg(args, unsigned int), 8, 0);
            else if (*p == 'x')
                count += print_number(va_arg(args, unsigned int), 16, 0);
            else if (*p == 'c')
                count += print_char((char)va_arg(args, int));
            else if (*p == 's')
                count += print_str(va_arg(args, char *));
            else if (*p == 'p')
                count += print_pointer(va_arg(args, void *));
            else
                count += print_char(*p);
        } else {
            count += print_char(*p);
        }
        p++;
    }
    va_end(args);
    return count;
}

/*
int main() {
    my_printf("Hello %s!\n", "world");
    my_printf("Decimal: %d, Hex: %x, Octal: %o, Unsigned: %u\n", -42, 255, 255, 255);
    my_printf("Char: %c, Pointer: %p\n", 'A', main);
    return 0;
}
*/