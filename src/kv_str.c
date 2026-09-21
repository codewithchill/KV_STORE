#include <kv/str.h>
#include <stdlib.h>

#define _DEFAULT_LINE_SIZE 512

void print_line_detail(str *line) {
    printf("[%llu]-[%llu]:[%s]\n", line->capacity, line->len, line->data);
}

/* the caller must free the str.data
 *
 * 1. If str.data != NULL

 *      a. str.capacity != 0
 */
str get_line(FILE *f) {
    size_t line_cap = 0;
    size_t char_count = 0;
    uint8_t *line = malloc(_DEFAULT_LINE_SIZE);
    if (!line)
        goto __end;

    line_cap = _DEFAULT_LINE_SIZE;
    int c = fgetc(f);
    while (c != '\n' && c != EOF) {
        if (c == '\b') {
            if (char_count > 0)
                char_count--;
        } else {
            line[char_count] = (uint8_t)c;
            char_count++;
            if ((char_count + 1) > line_cap) {
                uint8_t *temp = malloc(line_cap + _DEFAULT_LINE_SIZE);
                if (!temp)
                    goto free_err;
                for (size_t i = 0; i < char_count; i++)
                    temp[i] = line[i];

                line_cap += _DEFAULT_LINE_SIZE;
                free(line);
                line = temp;
            }
        }
        c = fgetc(f);
    }
    line[char_count] = '\0';
    if (EOF == c && 0 == char_count)
        goto free_err;
    goto __end;
free_err:
    free(line);
    line = NULL;
    line_cap = 0;
    char_count = 0;
__end:
    str str_line = {.capacity = line_cap, .data = line, .len = char_count};
    return str_line;
}

str *get_empty_str() {
    str *_str = calloc(1, sizeof(str));
    if (!_str)
        return NULL;
    return NULL;
}

str *str_conv(const char *restrict /*src*/) {
    // if (!src)
    return NULL;
}
void free_str(str *str_data) {
    free(str_data->data);
    str_data->len = 0;
    str_data->data = NULL;
    str_data->capacity = 0;
}
size_t str_len(const uint8_t *s) {
    size_t count = 0;
    while (*s) {
        count += ((*s & 0xC0) != 0x80);
        s++;
    }
    return count;
}
