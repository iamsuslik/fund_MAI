#ifndef OVER_SCANF_H
#define OVER_SCANF_H

#include <stdio.h>

int overfscanf(FILE *stream, const char *format, ...);
int oversscanf(const char *str, const char *format, ...);
int overscanf(const char *format, ...);

typedef enum {
    OVERSCANF_SUCCESS,
    OVERSCANF_EOF,
    OVERSCANF_NO_MATCH,
    OVERSCANF_INVALID_ROMAN,
    OVERSCANF_INVALID_ZEKENDORF,
    OVERSCANF_INVALID_BASE,
    OVERSCANF_INVALID_CHAR,
    OVERSCANF_MEMORY_ERROR,
    OVERSCANF_TOO_LONG
} Status_t;


int roman_to_int(const char *str, int *value);
int zekendorf_to_uint(const char *str, unsigned int *value);
int custom_base_to_int(const char *str, int *value, int base, int uppercase);

#endif
