#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "laba5.h"

#define MAX_LINE 80
#define BUFFER 1024

return_code path_file(const char *input, const char *output) {
    if (strcmp(input, output) == 0) return SAME;
    return SUCCESS;
}

void justify_and_print(char **words, int count, FILE *out) {
    if (count == 0) return;

    int total_len = 0;
    for (int i = 0; i < count; i++)
        total_len += strlen(words[i]);

    int total_spaces = MAX_LINE - total_len;
    int space_between = (count > 1) ? total_spaces / (count - 1) : 0;
    int extra = (count > 1) ? total_spaces % (count - 1) : 0;

    for (int i = 0; i < count; i++) {
        fputs(words[i], out);
        if (i != count - 1) {
            for (int s = 0; s < space_between; s++) fputc(' ', out);
            if (extra > 0) { fputc(' ', out); extra--; }
        }
    }
    fputc('\n', out);
}

void format_line(const char *src, FILE *out) {
    const char *ptr = src;
    while (*ptr && isspace((unsigned char)*ptr)) ptr++; // убираем ведущие пробелы

    while (*ptr) {
        int remaining = strlen(ptr);
        int line_len = remaining >= MAX_LINE ? MAX_LINE : remaining;

        int split = line_len;
        while (split > 0 && !isspace((unsigned char)ptr[split]) && line_len == MAX_LINE) split--;

        if (split == 0 && line_len == MAX_LINE) split = MAX_LINE; // нет пробела — делим ровно 80

        fwrite(ptr, 1, split, out);
        fputc('\n', out);

        ptr += split;


        while (*ptr && isspace((unsigned char)*ptr)) ptr++;
    }
}


return_code mydef(const char *input, const char *output) {
    FILE *in = fopen(input, "r");
    if (!in) return ERROR_INPUT;
    FILE *out = fopen(output, "w");
    if (!out) { fclose(in); return ERROR_OUTPUT; }

    char buffer[BUFFER];
    while (fgets(buffer, BUFFER, in)) {
        buffer[strcspn(buffer, "\n")] = '\0';
        format_line(buffer, out);
    }

    fclose(in);
    fclose(out);
    return SUCCESS;
}

int main() {
    char input[512], output[512];

    printf("Enter input file: ");
    scanf("%511s", input);
    printf("Enter output file: ");
    scanf("%511s", output);

    return_code res = path_file(input, output);
    if (res == SAME) { printf("Input and output must be different!\n"); return 1; }

    res = mydef(input, output);
    if (res != SUCCESS) { printf("Error processing file.\n"); return 1; }

    printf("File processed successfully.\n");
    return 0;
}
