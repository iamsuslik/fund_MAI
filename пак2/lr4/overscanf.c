#include "overscanf.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>





// %Ro
int roman_to_int(const char *str, int *value) {
    if (str == NULL || value == NULL) {
        return OVERSCANF_NO_MATCH;
    }
    if (*str == '\0') {
        return OVERSCANF_INVALID_ROMAN;
    }

    size_t len = strlen(str);
    if (len > 64) {
        return OVERSCANF_TOO_LONG;
    }

    if (strstr(str, "IIV") != NULL || strstr(str, "IIX") != NULL || 
        strstr(str, "IIL") != NULL || strstr(str, "IIC") != NULL ||
        strstr(str, "IID") != NULL || strstr(str, "IIM") != NULL ||
        strstr(str, "XXL") != NULL || strstr(str, "XXC") != NULL ||
        strstr(str, "XXD") != NULL || strstr(str, "XXM") != NULL ||
        strstr(str, "CCD") != NULL || strstr(str, "CCM") != NULL ||
        strstr(str, "VV") != NULL || strstr(str, "LL") != NULL || 
        strstr(str, "DD") != NULL ||
        strstr(str, "IL") != NULL || strstr(str, "IC") != NULL ||
        strstr(str, "ID") != NULL || strstr(str, "IM") != NULL ||
        strstr(str, "VX") != NULL || strstr(str, "VL") != NULL ||
        strstr(str, "VC") != NULL || strstr(str, "VD") != NULL ||
        strstr(str, "VM") != NULL || strstr(str, "XD") != NULL ||
        strstr(str, "XM") != NULL || strstr(str, "LC") != NULL ||
        strstr(str, "LD") != NULL || strstr(str, "LM") != NULL) {
        return OVERSCANF_INVALID_ROMAN;
    }

    const char *ptr = str;
    int result = 0;
    int prev_value = 0;
    int repeat_count = 1;
    char last_char = *str;

    while (*ptr) {
        char c = *ptr;
        int current_value = 0;
        
        switch (c) {
            case 'M': current_value = 1000; break;
            case 'D': current_value = 500; break;
            case 'C': current_value = 100; break;
            case 'L': current_value = 50; break;
            case 'X': current_value = 10; break;
            case 'V': current_value = 5; break;
            case 'I': current_value = 1; break;
            default: return OVERSCANF_INVALID_ROMAN;
        }
        
        if (ptr > str && c == last_char) {
            repeat_count++;
        } else {
            repeat_count = 1;
        }
        
        last_char = c;
        
        if ((c == 'I' || c == 'X' || c == 'C' || c == 'M') && repeat_count > 3) {
            return OVERSCANF_INVALID_ROMAN;
        }
        if ((c == 'V' || c == 'L' || c == 'D') && repeat_count > 1) {
            return OVERSCANF_INVALID_ROMAN;
        }

        if (current_value > prev_value) {
            if (!((prev_value == 1 && (current_value == 5 || current_value == 10)) ||
                  (prev_value == 10 && (current_value == 50 || current_value == 100)) ||
                  (prev_value == 100 && (current_value == 500 || current_value == 1000)) ||
                  prev_value == 0)) {
                return OVERSCANF_INVALID_ROMAN;
            }
            result += current_value - 2 * prev_value;
        } else {
            result += current_value;
        }

        prev_value = current_value;
        ptr++;
    }

    *value = result;
    return OVERSCANF_SUCCESS;
}


// Zr
int zekendorf_to_uint(const char *str, unsigned int *value) {
    if (str == NULL || value == NULL) {
        return OVERSCANF_NO_MATCH;
    }
    if (*str == '\0') {
        return OVERSCANF_INVALID_ZEKENDORF;
    }

    const char *ptr = str;
    unsigned int result = 0;
    unsigned int fib_prev = 1, fib_curr = 2;
    int prev_bit = 0;

    while (*ptr) {
        if (*ptr == '1') {
            if (prev_bit == 1) {
                return OVERSCANF_INVALID_ZEKENDORF;
            }
            result += fib_prev;
            prev_bit = 1;
        } else if (*ptr == '0') {
            prev_bit = 0;
        } else {
            return OVERSCANF_INVALID_ZEKENDORF;
        }

        unsigned int fib_next = fib_prev + fib_curr;
        fib_prev = fib_curr;
        fib_curr = fib_next;

        ptr++;
    }

    if (ptr > str && *(ptr - 1) != '1') {
        return OVERSCANF_INVALID_ZEKENDORF;
    }

    *value = result;
    return OVERSCANF_SUCCESS;
}

// %Cv
// Конвертация из произвольной системы счисления
int custom_base_to_int(const char *str, int *value, int base, int uppercase) {
    if (str == NULL || value == NULL) return OVERSCANF_NO_MATCH;
    if (base < 2 || base > 36) base = 10;
    
    const char *ptr = str;
    int result = 0;
    bool negative = false;
    
    // Обработка знака
    if (*ptr == '-') {
        negative = true;
        ptr++;
    } else if (*ptr == '+') {
        ptr++;
    }
    
    while (*ptr) {
        char c = *ptr;
        int digit;
        
        if (isdigit(c)) {
            digit = c - '0';
        } else if (isalpha(c)) {
            if (uppercase) {
                digit = toupper(c) - 'A' + 10;
            } else {
                digit = tolower(c) - 'a' + 10;
            }
        } else {
            return OVERSCANF_INVALID_CHAR;
        }
        
        if (digit >= base) {
            return OVERSCANF_INVALID_BASE;
        }
        
        result = result * base + digit;
        ptr++;
    }
    
    *value = negative ? -result : result;
    return OVERSCANF_SUCCESS;
}



// основная функция обработки формата
static int overscanf_common(int (*input_func)(void *, const char *, ...), void *stream_data, 
                           const char *format, va_list args) {
    if (format == NULL || input_func == NULL) {
        return OVERSCANF_NO_MATCH;
    }

    int total_args = 0;
    const char *p = format;

    while (*p != '\0') {
        // Пропускаем пробелы в формате
        if (isspace(*p)) {
            p++;
            continue;
        }

        if (*p != '%') {
            char buffer[2] = {*p, '\0'};
            int result = input_func(stream_data, "%1s", buffer);
            if (result != 1 || buffer[0] != *p) {
                return total_args > 0 ? total_args : OVERSCANF_NO_MATCH;
            }
            p++;
            continue;
        }

        p++;
        if (*p == '\0') break;

        // Обработка специальных форматов
        if (p[0] == 'R' && p[1] == 'o') {
            int *value_ptr = va_arg(args, int*);
            char buffer[32];
            
            int result = input_func(stream_data, "%31s", buffer);
            if (result != 1) {
                return total_args;
            }
            
            int status = roman_to_int(buffer, value_ptr);
            if (status == OVERSCANF_SUCCESS) {
                total_args++;
            } else {
                return status;
            }
            p += 2;
        }
        else if (p[0] == 'Z' && p[1] == 'r') {
            unsigned int *value_ptr = va_arg(args, unsigned int*);
            char buffer[64];
            
            int result = input_func(stream_data, "%63s", buffer);
            if (result != 1) {
                return total_args;
            }
            
            int status = zekendorf_to_uint(buffer, value_ptr);
            if (status == OVERSCANF_SUCCESS) {
                total_args++;
            } else {
                return status;
            }
            p += 2;
        }
        else if (p[0] == 'C' && (p[1] == 'v' || p[1] == 'V')) {
            int *value_ptr = va_arg(args, int*);
            int base = va_arg(args, int);
            char buffer[64];
            
            int result = input_func(stream_data, "%63s", buffer);
            if (result != 1) {
                return total_args;
            }
            
            int uppercase = (p[1] == 'V');
            int status = custom_base_to_int(buffer, value_ptr, base, uppercase);
            if (status == OVERSCANF_SUCCESS) {
                total_args++;
            } else {
                return status;
            }
            p += 2;
        }
        else {
            // Стандартные спецификаторы
            char fmt_buffer[16] = {0};
            fmt_buffer[0] = '%';
            const char *spec_start = p;
            
            while (*p && !strchr("diouxXeEfFgGaAcspn", *p)) {
                p++;
            }
            
            if (*p) {
                size_t spec_len = p - spec_start + 1;
                if (spec_len < sizeof(fmt_buffer) - 1) {
                    memcpy(fmt_buffer + 1, spec_start, spec_len);
                    fmt_buffer[spec_len + 1] = '\0';
                    
                    void *arg = va_arg(args, void*);
                    int result = input_func(stream_data, fmt_buffer, arg);
                    
                    if (result == 1) {
                        total_args++;
                    } else if (total_args == 0) {
                        return OVERSCANF_NO_MATCH;
                    } else {
                        return total_args;
                    }
                }
                p++;
            } else {
                return OVERSCANF_NO_MATCH;
            }
        }
    }

    return total_args;
}

struct string_input_data {
    const char *str;
    const char **current_pos;
};

struct file_input_data {
    FILE *f;
};


static int string_input(void *stream_data, const char *format, ...) {
    struct string_input_data *data = (struct string_input_data *)stream_data;
    va_list args;
    va_start(args, format);
    
    int result = vsscanf(*data->current_pos, format, args);
    
    if (result > 0) {
        const char *temp = *data->current_pos;
        while (*temp && !isspace(*temp)) temp++;
        while (isspace(*temp)) temp++;
        *data->current_pos = temp;
    }
    
    va_end(args);
    return result;
}

static int file_input(void *stream_data, const char *format, ...) {
    struct file_input_data *data = (struct file_input_data *)stream_data;
    va_list args;
    va_start(args, format);
    int result = vfscanf(data->f, format, args);
    va_end(args);
    return result;
}

int oversscanf(const char *str, const char *format, ...) {
    if (str == NULL || format == NULL) return OVERSCANF_NO_MATCH;
    
    va_list args;
    va_start(args, format);
    
    int count = 0;
    const char *str_ptr = str;
    struct string_input_data data = { str, &str_ptr };
    
    count = overscanf_common(string_input, &data, format, args);
    
    va_end(args);
    return count;
}

int overfscanf(FILE *stream, const char *format, ...) {
    if (stream == NULL || format == NULL) return OVERSCANF_NO_MATCH;
    
    va_list args;
    va_start(args, format);
    
    struct file_input_data data = { stream };
    int count = overscanf_common(file_input, &data, format, args);
    
    va_end(args);
    return count;
}

int overscanf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    char input[1024];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        va_end(args);
        return OVERSCANF_EOF;
    }

    FILE *temp = tmpfile();
    if (temp == NULL) {
        va_end(args);
        return OVERSCANF_NO_MATCH;
    }

    fputs(input, temp);
    rewind(temp);

    struct file_input_data data = { temp };
    int result = overscanf_common(file_input, &data, format, args);
    
    fclose(temp);
    va_end(args);
    return result;
}
