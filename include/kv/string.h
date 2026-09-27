#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <kv/error.h>

typedef struct {
    uint8_t *data;
    size_t byte_len;
    size_t utf_len;
    size_t capacity;
} __str;

typedef struct string {
    kv_error_t err;
    __str *s;
} string;

string *get_str(size_t cap);
string get_line(FILE *f);
string *str_conv(const char *restrict src);

size_t str_len(const uint8_t *s);
void free_str(string *str_data);
void print_line_detail(string *line, const char mode);
