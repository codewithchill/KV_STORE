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
} _str;

typedef struct string {
    kv_error_t err;
    _str *s;
} string;

// _str *get_str(size_t cap);
void get_line(string* s, FILE *f);

size_t str_len(const string* s);
void free_str(string *str_data);
void print_line_detail(string *line, const char mode);
string str_init();