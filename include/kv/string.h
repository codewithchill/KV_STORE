#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <kv/error.h>

typedef uint32_t Rune;
typedef uint8_t byte;
typedef byte* bytes;

typedef struct {
    bytes data;
    size_t byte_len;
    size_t utf_len;
    size_t capacity;
} _str;
typedef struct {
    kv_error_t err;
    _str s;
} string;

void get_line(string* s, FILE *f);
void free_str(string *str_data);
void print_line_detail(string *line, const char mode);
char* get_c_string(string* s);
string str_init();
size_t str_len(const string* s);
kv_error_t create_string(string *restrict str, const bytes restrict s, const size_t s_capacity);
