#ifndef OVERPRINTF_H
#define OVERPRINTF_H

#include <stdio.h>
#include <stdarg.h>


typedef enum {
    OVERPRINTF_SUCCESS,
    OVERPRINTF_INVALID_FORMAT,
    OVERPRINTF_INVALID_ARGUMENT,
    OVERPRINTF_BUFFER_OVERFLOW,
    OVERPRINTF_INVALID_BASE,
    OVERPRINTF_INVALID_ROMAN,
    OVERPRINTF_INVALID_FIBONACCI,
    OVERPRINTF_MEMORY_ERROR,
    OVERPRINTF_ROMAN_ERROR,
    OVERPRINTF_ZEKENDORF_ERROR,
    OVERPRINTF_BASE_CONVERSION_ERROR,
    OVERPRINTF_PARSE_ERROR,
    OVERPRINTF_MEMORY_DUMP_ERROR
} overprintf_status_t;


int overfprintf(FILE *stream, const char *format, ...);
int oversprintf(char *str, size_t str_size, const char *format, ...);
int overprintf(const char *format, ...);

overprintf_status_t convert_to_roman(int num, char *buffer, size_t buffer_size);
overprintf_status_t convert_to_zeckendorf(unsigned int num, char *buffer, size_t buffer_size);
overprintf_status_t convert_base(int num, int base, int uppercase, char *buffer, size_t buffer_size);
overprintf_status_t parse_string_to_int(const char *str, int base, int uppercase, int *result);
overprintf_status_t memory_dump_int(int num, char *buffer, size_t buffer_size);
overprintf_status_t memory_dump_unsigned(unsigned int num, char *buffer, size_t buffer_size);
overprintf_status_t memory_dump_double(double num, char *buffer, size_t buffer_size);
overprintf_status_t memory_dump_float(float num, char *buffer, size_t buffer_size);

#endif
