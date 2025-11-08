#include "overprintf.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <limits.h>

#define BUFFER_SIZE 256
#define MAX_BASE 36
#define MIN_BASE 2

// функция для безопасного добавления в буфер. Аналог strcat, но с контролем размера
static overprintf_status_t safe_append(char *buffer, size_t *pos, size_t buffer_size, const char *str) {
    if (buffer == NULL || pos == NULL || str == NULL) {
        return OVERPRINTF_INVALID_ARGUMENT;
    }

    size_t len = strlen(str);

    if (*pos + len + 1 > buffer_size) {
        return OVERPRINTF_BUFFER_OVERFLOW;
    }

    memcpy(buffer + *pos, str, len);
    *pos += len;
    buffer[*pos] = '\0';

    return OVERPRINTF_SUCCESS;
}


overprintf_status_t convert_to_roman(int num, char *buffer, size_t buffer_size) {
    if (num <= 0 || num > 3999) {
        return OVERPRINTF_INVALID_ROMAN;
    }

    const char *thousands[] = {"", "M", "MM", "MMM"};
    const char *hundreds[] = {"", "C", "CC", "CCC", "CD", "D", "DC", "DCC", "DCCC", "CM"};
    const char *tens[] = {"", "X", "XX", "XXX", "XL", "L", "LX", "LXX", "LXXX", "XC"};
    const char *ones[] = {"", "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX"};

    size_t pos = 0;

    if (num >= 1000) {
        overprintf_status_t status = safe_append(buffer, &pos, buffer_size, thousands[num / 1000]);
        if (status != OVERPRINTF_SUCCESS) return status;
        num %= 1000;
    }

    if (num >= 100) {
        overprintf_status_t status = safe_append(buffer, &pos, buffer_size, hundreds[num / 100]);
        if (status != OVERPRINTF_SUCCESS) return status;
        num %= 100;
    }

    if (num >= 10) {
        overprintf_status_t status = safe_append(buffer, &pos, buffer_size, tens[num / 10]);
        if (status != OVERPRINTF_SUCCESS) return status;
        num %= 10;
    }

    if (num > 0) {
        overprintf_status_t status = safe_append(buffer, &pos, buffer_size, ones[num]);
        if (status != OVERPRINTF_SUCCESS) return status;
    }

    buffer[pos] = '\0';
    return OVERPRINTF_SUCCESS;
}

// Генерация чисел Фибоначчи
static void generate_fibonacci(unsigned int *fib, size_t *size, unsigned int limit) {
    fib[0] = 1;
    fib[1] = 2;
    *size = 2;

    while (fib[*size - 1] <= limit) {
        fib[*size] = fib[*size - 1] + fib[*size - 2];
        (*size)++;
    }
}


// Конвертация в цекендорфово представление
overprintf_status_t convert_to_zeckendorf(unsigned int num, char *buffer, size_t buffer_size) {
    if (num <= 0) {
        return OVERPRINTF_INVALID_FIBONACCI;
    }

    unsigned int fib[50];
    size_t fib_size;
    generate_fibonacci(fib, &fib_size, num);

    size_t pos = 0;

    char temp_buffer[50] = {0};
    size_t temp_pos = 0;
    unsigned int temp_num = num;

    for (int i = fib_size - 1; i >= 0; i--) {
        if (temp_num >= fib[i]) {
            temp_buffer[temp_pos++] = '1';
            temp_num -= fib[i];
        } else if (temp_pos > 0) {
            temp_buffer[temp_pos++] = '0';
        }
    }
    
    // переворачиваем, чтобы было от мл к ст и  сразу убираем ведущие нули 
    int found_first_one = 0;
    for (int i = temp_pos - 1; i >= 0; i--) {
        if (!found_first_one && temp_buffer[i] == '1') {
            found_first_one = 1;
        }
    
        if (found_first_one) {
            if (pos < buffer_size - 1) {
                buffer[pos++] = temp_buffer[i];
            } else {
                return OVERPRINTF_BUFFER_OVERFLOW;
            }
        }
    }

    if (!found_first_one) {
        return OVERPRINTF_INVALID_FIBONACCI;
    }

    // добавляем завершающую единицу
    if (pos < buffer_size - 1) {
        buffer[pos++] = '1';
        buffer[pos] = '\0';
    } else {
        return OVERPRINTF_BUFFER_OVERFLOW;
    }

    return OVERPRINTF_SUCCESS;
}

overprintf_status_t convert_base(int num, int base, int uppercase, char *buffer, size_t buffer_size) {
    if (base < MIN_BASE || base > MAX_BASE) {
        base = 10;
    }

    if (buffer_size < 2) {
        return OVERPRINTF_BUFFER_OVERFLOW;
    }

    char temp[BUFFER_SIZE];
    int pos = 0;
    int is_negative = 0;
    unsigned int unum;

    if (num < 0) {
        is_negative = 1;
        unum = (unsigned int)(-num);
    } else {
        unum = (unsigned int)num;
    }

    if (unum == 0) {
        temp[pos++] = '0';
    } else {
        while (unum > 0 && pos < BUFFER_SIZE - 1) {
            int digit = unum % base;
            if (digit < 10) {
                temp[pos++] = '0' + digit;
            } else {
                if (uppercase) {
                    temp[pos++] = 'A' + (digit - 10);
                } else {
                    temp[pos++] = 'a' + (digit - 10);
                }
            }
            unum /= base;
        }
    }


    if (is_negative) {
        temp[pos++] = '-';
    }


    if ((size_t)(pos + 1) > buffer_size) {
        return OVERPRINTF_BUFFER_OVERFLOW;
    }


    for (int i = 0; i < pos; i++) {
        buffer[i] = temp[pos - 1 - i];
    }
    buffer[pos] = '\0';

    return OVERPRINTF_SUCCESS;
}


overprintf_status_t parse_string_to_int(const char *str, int base, int uppercase, int *result) {
    if (str == NULL || result == NULL) {
        return OVERPRINTF_INVALID_ARGUMENT;
    }

    if (base < MIN_BASE || base > MAX_BASE) {
        base = 10;
    }

    int res = 0;
    int is_negative = 0;
    int i = 0;

    if (str[0] == '-') {
        is_negative = 1;
        i++;
    } else if (str[0] == '+') {
        i++;
    }


    if (str[i] == '\0') {
        return OVERPRINTF_INVALID_ARGUMENT;
    }

    for (; str[i] != '\0'; i++) {
        char c = str[i];
        int digit;

        if (c >= '0' && c <= '9') {
            digit = c - '0';
        } else if (c >= 'a' && c <= 'z') {
            if (uppercase) {
                return OVERPRINTF_INVALID_ARGUMENT;
            }
            digit = c - 'a' + 10;
        } else if (c >= 'A' && c <= 'Z') {
            if (!uppercase) {
                return OVERPRINTF_INVALID_ARGUMENT;
            }
            digit = c - 'A' + 10;
        } else {
            return OVERPRINTF_INVALID_ARGUMENT;
        }

        if (digit >= base) {
            return OVERPRINTF_INVALID_ARGUMENT;
        }

        if (res > (INT_MAX - digit) / base) {
            return OVERPRINTF_INVALID_ARGUMENT;
        }

        res = res * base + digit;
    }

    if (is_negative) {
        res = -res;
    }

    *result = res;
    return OVERPRINTF_SUCCESS;
}


overprintf_status_t memory_dump_int(int num, char *buffer, size_t buffer_size) {
    unsigned char *bytes = (unsigned char *)&num; // указывает на первый байт числа
    size_t pos = 0;

    for (size_t i = 0; i < sizeof(int); i++) {
        char byte_str[9]; // буфер для одного байта
        for (int j = 7; j >= 0; j--) {
            byte_str[7 - j] = (bytes[i] & (1 << j)) ? '1' : '0';
        }
    
        byte_str[8] = '\0';

        overprintf_status_t status = safe_append(buffer, &pos, buffer_size, byte_str);
        if (status != OVERPRINTF_SUCCESS) return status;

        if (i < sizeof(int) - 1) {
            status = safe_append(buffer, &pos, buffer_size, " ");
            if (status != OVERPRINTF_SUCCESS) return status;
        }
    }

    buffer[pos] = '\0';
    return OVERPRINTF_SUCCESS;
}


overprintf_status_t memory_dump_unsigned(unsigned int num, char *buffer, size_t buffer_size) {
    unsigned char *bytes = (unsigned char *)&num;
    size_t pos = 0;

    for (size_t i = 0; i < sizeof(unsigned int); i++) {
        char byte_str[9];
        for (int j = 7; j >= 0; j--) {
            byte_str[7 - j] = (bytes[i] & (1 << j)) ? '1' : '0';
        }
        byte_str[8] = '\0';

        overprintf_status_t status = safe_append(buffer, &pos, buffer_size, byte_str);
        if (status != OVERPRINTF_SUCCESS) return status;

        if (i < sizeof(unsigned int) - 1) {
            status = safe_append(buffer, &pos, buffer_size, " ");
            if (status != OVERPRINTF_SUCCESS) return status;
        }
    }

    buffer[pos] = '\0';
    return OVERPRINTF_SUCCESS;
}



overprintf_status_t memory_dump_double(double num, char *buffer, size_t buffer_size) {
    unsigned char *bytes = (unsigned char *)&num;
    size_t pos = 0;

    for (size_t i = 0; i < sizeof(double); i++) {
        char byte_str[9];
        for (int j = 7; j >= 0; j--) {
            byte_str[7 - j] = (bytes[i] & (1 << j)) ? '1' : '0';
        }
        byte_str[8] = '\0';
    
        overprintf_status_t status = safe_append(buffer, &pos, buffer_size, byte_str);
        if (status != OVERPRINTF_SUCCESS) return status;
    
        if (i < sizeof(double) - 1) {
            status = safe_append(buffer, &pos, buffer_size, " ");
            if (status != OVERPRINTF_SUCCESS) return status;
        }
    }

    buffer[pos] = '\0';
    return OVERPRINTF_SUCCESS;
}


overprintf_status_t memory_dump_float(float num, char *buffer, size_t buffer_size) {
    unsigned char *bytes = (unsigned char *)&num;
    size_t pos = 0;

    for (size_t i = 0; i < sizeof(float); i++) {
        char byte_str[9];
        for (int j = 7; j >= 0; j--) {
            byte_str[7 - j] = (bytes[i] & (1 << j)) ? '1' : '0';
        }
        byte_str[8] = '\0';

        overprintf_status_t status = safe_append(buffer, &pos, buffer_size, byte_str);
        if (status != OVERPRINTF_SUCCESS) return status;

        if (i < sizeof(float) - 1) {
            status = safe_append(buffer, &pos, buffer_size, " ");
            if (status != OVERPRINTF_SUCCESS) return status;
        }
    }

    buffer[pos] = '\0';
    return OVERPRINTF_SUCCESS;
}


// Основная функция форматирования
static int overprintf_common(int (*output_func)(void *, const char *, ...), void *stream, const char *format, va_list args) {
    if (format == NULL || output_func == NULL) {
        return -1;
    }

    int total_chars = 0;
    char buffer[BUFFER_SIZE];

    for (const char *p = format; *p != '\0'; p++) {
        if (*p != '%') {
            int written = output_func(stream, "%c", *p);
            if (written > 0) total_chars += written;
            continue;
        }

        p++;
        if (*p == '\0') break;


        if (p[0] == 'R' && p[1] == 'o') {
            int num = va_arg(args, int);
            overprintf_status_t status = convert_to_roman(num, buffer, BUFFER_SIZE);
            if (status == OVERPRINTF_SUCCESS) {
                int written = output_func(stream, "%s", buffer);
                if (written > 0) total_chars += written;
            } else {
                const char* error_msg = "[ROMAN_ERROR]";
                int written = output_func(stream, "%s", error_msg);
                if (written > 0) total_chars += written;
            }
            p++;
        }
        else if (p[0] == 'Z' && p[1] == 'r') {
            unsigned int num = va_arg(args, unsigned int);
            overprintf_status_t status = convert_to_zeckendorf(num, buffer, BUFFER_SIZE);
            if (status == OVERPRINTF_SUCCESS) {
                int written = output_func(stream, "%s", buffer);
                if (written > 0) total_chars += written;
            } else {
                const char* error_msg = "[ZEKENDORF_ERROR]";
                int written = output_func(stream, "%s", error_msg);
                if (written > 0) total_chars += written;
            }
            p++;
        }
        else if (p[0] == 'C' && p[1] == 'v') {
            int num = va_arg(args, int);
            int base = va_arg(args, int);
            overprintf_status_t status = convert_base(num, base, 0, buffer, BUFFER_SIZE);
            if (status == OVERPRINTF_SUCCESS) {
                int written = output_func(stream, "%s", buffer);
                if (written > 0) total_chars += written;
            } else {
                const char* error_msg = "[BASE_CONVERSION_ERROR]";
                int written = output_func(stream, "%s", error_msg);
                if (written > 0) total_chars += written;
            }
            p++;
        }
        else if (p[0] == 'C' && p[1] == 'V') {
            int num = va_arg(args, int);
            int base = va_arg(args, int);
            overprintf_status_t status = convert_base(num, base, 1, buffer, BUFFER_SIZE);
            if (status == OVERPRINTF_SUCCESS) {
                int written = output_func(stream, "%s", buffer);
                if (written > 0) total_chars += written;
            } else {
                const char* error_msg = "[BASE_CONVERSION_ERROR]";
                int written = output_func(stream, "%s", error_msg);
                if (written > 0) total_chars += written;
            }
            p++;
        }
        else if (p[0] == 't' && p[1] == 'o') {
            const char *str = va_arg(args, const char *);
            int base = va_arg(args, int);
            int result;
            overprintf_status_t status = parse_string_to_int(str, base, 0, &result);
            if (status == OVERPRINTF_SUCCESS) {
                int written = output_func(stream, "%d", result);
                if (written > 0) total_chars += written;
            } else {
                const char* error_msg = "[PARSE_ERROR]";
                int written = output_func(stream, "%s", error_msg);
                if (written > 0) total_chars += written;
            }
            p++;
        }
        else if (p[0] == 'T' && p[1] == 'O') {
            const char *str = va_arg(args, const char *);
            int base = va_arg(args, int);
            int result;
            overprintf_status_t status = parse_string_to_int(str, base, 1, &result);
            if (status == OVERPRINTF_SUCCESS) {
                int written = output_func(stream, "%d", result);
                if (written > 0) total_chars += written;
            } else {
                const char* error_msg = "[PARSE_ERROR]";
                int written = output_func(stream, "%s", error_msg);
                if (written > 0) total_chars += written;
            }
            p++;
        }
        else if (p[0] == 'm' && p[1] == 'i') {
            int num = va_arg(args, int);
            overprintf_status_t status = memory_dump_int(num, buffer, BUFFER_SIZE);
            if (status == OVERPRINTF_SUCCESS) {
                int written = output_func(stream, "%s", buffer);
                if (written > 0) total_chars += written;
            } else {
                const char* error_msg = "[MEMORY_DUMP_ERROR]";
                int written = output_func(stream, "%s", error_msg);
                if (written > 0) total_chars += written;
            }
            p++;
        }
        else if (p[0] == 'm' && p[1] == 'u') {
            unsigned int num = va_arg(args, unsigned int);
            overprintf_status_t status = memory_dump_unsigned(num, buffer, BUFFER_SIZE);
            if (status == OVERPRINTF_SUCCESS) {
                int written = output_func(stream, "%s", buffer);
                if (written > 0) total_chars += written;
            } else {
                const char* error_msg = "[MEMORY_DUMP_ERROR]";
                int written = output_func(stream, "%s", error_msg);
                if (written > 0) total_chars += written;
            }
            p++;
        }
        else if (p[0] == 'm' && p[1] == 'd') {
            double num = va_arg(args, double);
            overprintf_status_t status = memory_dump_double(num, buffer, BUFFER_SIZE);
            if (status == OVERPRINTF_SUCCESS) {
                int written = output_func(stream, "%s", buffer);
                if (written > 0) total_chars += written;
            } else {
                const char* error_msg = "[MEMORY_DUMP_ERROR]";
                int written = output_func(stream, "%s", error_msg);
                if (written > 0) total_chars += written;
            }
            p++; 
        }
        else if (p[0] == 'm' && p[1] == 'f') {
            float num = (float)va_arg(args, double);
            overprintf_status_t status = memory_dump_float(num, buffer, BUFFER_SIZE);
            if (status == OVERPRINTF_SUCCESS) {
                int written = output_func(stream, "%s", buffer);
                if (written > 0) total_chars += written;
            } else {
                const char* error_msg = "[MEMORY_DUMP_ERROR]";
                int written = output_func(stream, "%s", error_msg);
                if (written > 0) total_chars += written;
            }
            p++;
        }
        else {
            char fmt_buffer[16] = {0};
            fmt_buffer[0] = '%';
            const char *spec_start = p;
            while (*p && !strchr("diuoxXfFeEgGaAcspn%", *p)) {
                p++;
            }
        
            if (*p) {
                size_t spec_len = p - spec_start + 1;
                if (spec_len < sizeof(fmt_buffer) - 1) {
                    memcpy(fmt_buffer + 1, spec_start, spec_len);
                    fmt_buffer[spec_len + 1] = '\0';

                    int written = 0;
                    switch (*p) {
                        case 'd': case 'i': {
                            int num = va_arg(args, int);
                            written = output_func(stream, fmt_buffer, num);
                            break;
                        }
                        case 'u': case 'x': case 'X': case 'o': {
                            unsigned int num = va_arg(args, unsigned int);
                            written = output_func(stream, fmt_buffer, num);
                            break;
                        }
                        case 'f': case 'F': case 'e': case 'E': case 'g': case 'G': {
                            double num = va_arg(args, double);
                            written = output_func(stream, fmt_buffer, num);
                            break;
                        }
                        case 'c': {
                            int ch = va_arg(args, int);
                            written = output_func(stream, fmt_buffer, ch);
                            break;
                        }
                        case 's': {
                            char *str = va_arg(args, char*);
                            written = output_func(stream, fmt_buffer, str);
                            break;
                        }
                        case 'p': {
                            void *ptr = va_arg(args, void*);
                            written = output_func(stream, fmt_buffer, ptr);
                            break;
                        }
                        case '%': {
                            written = output_func(stream, "%%");
                            break;
                        }
                        default: {
                            written = output_func(stream, "%%%c", *p);
                            break;
                        }
                    }
                    if (written > 0) total_chars += written;
                }
            } else {
                int written = output_func(stream, "%%%s", spec_start);
                if (written > 0) total_chars += written;
                break;
            }
        }
    }

    return total_chars;
}








struct string_output_data {
    char *buffer;
    size_t size;   // размер всего буфера
    size_t *pos;   // текущая позиция записи
};

static int string_output(void *stream_struct, const char *format, ...) {
    struct string_output_data *data = (struct string_output_data *)stream_struct;
    va_list args;
    va_start(args, format);

    size_t remaining = data->size - *(data->pos);
    if (remaining == 0) {
        va_end(args);
        return 0;
    }

    int written = vsnprintf(data->buffer + *(data->pos), remaining, format, args);
    va_end(args);

    if (written < 0) written = 0;
    if ((size_t)written > remaining - 1) written = remaining - 1;

    *(data->pos) += written;
    return written;
}

int oversprintf(char *str, size_t str_size, const char *format, ...) {
    if (str == NULL || str_size == 0) return -1;

    size_t pos = 0;
    struct string_output_data data = { str, str_size, &pos };

    va_list args;
    va_start(args, format);
    int result = overprintf_common(string_output, &data, format, args);
    va_end(args);

    str[pos < str_size ? pos : str_size - 1] = '\0';

    return result;
}


struct file_output_data {
    FILE *f;
};


static int file_output_wrapper(void *stream_struct, const char *format, ...)
{
    struct file_output_data *data = (struct file_output_data *)stream_struct;
    va_list args;
    va_start(args, format);
    int written = vfprintf(data->f, format, args);
    va_end(args);
    return written;
}

int overfprintf(FILE *f, const char *format, ...)
{
    if (!f || !format) return -1;

    struct file_output_data data = { f };
    va_list args;
    va_start(args, format);
    int result = overprintf_common(file_output_wrapper, &data, format, args);
    va_end(args);

    return result;
}



int overprintf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    
    struct file_output_data data = { stdout };
    int result = overprintf_common(file_output_wrapper, &data, format, args);
    
    va_end(args);
    return result;
}
