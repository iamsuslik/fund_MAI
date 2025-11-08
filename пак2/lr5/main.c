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
    char word[BUFFER];
    int word_len;
    char *ptr = (char*)src;

    char *line_words[BUFFER];
    int line_count = 0;
    int line_len = 0;

    while (*ptr) {
        while (*ptr && isspace((unsigned char)*ptr)) ptr++;
        if (!*ptr) break;

        word_len = 0;
        while (*ptr && !isspace((unsigned char)*ptr)) {
            word[word_len++] = *ptr++;
        }
        word[word_len] = '\0';
        int wlen = strlen(word);

        if (line_len + (line_count > 0 ? 1 : 0) + wlen > MAX_LINE) {
            if (line_count > 0) {
                justify_and_print(line_words, line_count, out);
                for (int i = 0; i < line_count; i++) free(line_words[i]);
                line_count = 0;
                line_len = 0;
            }
            while (wlen > MAX_LINE) {
                fwrite(word, 1, MAX_LINE, out);
                fputc('\n', out);
                memmove(word, word + MAX_LINE, wlen - MAX_LINE);
                wlen -= MAX_LINE;
                word[wlen] = '\0';
            }
        }

        line_words[line_count] = malloc(strlen(word) + 1);
        strcpy(line_words[line_count], word);
        line_count++;
        line_len += wlen + (line_count > 1 ? 1 : 0);
    }

    if (line_count > 0) {
        justify_and_print(line_words, line_count, out);
        for (int i = 0; i < line_count; i++) free(line_words[i]);
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
