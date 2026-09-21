#pragma once

#include <stdint.h>
#include <stdio.h>

typedef struct {
    uint8_t *data;
    size_t len;
    size_t capacity;
} str;

void print_line_detail(str *line);
str get_line(FILE *f);
str *str_conv(const char *restrict src);
void free_str(str *str_data);
size_t str_len(const uint8_t *s);
