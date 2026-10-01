#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <kv/error.h>

typedef int32_t Rune;
typedef uint8_t byte;
typedef byte *bytes;
typedef struct _str _str;
typedef struct string string;

void get_line(string *restrict s, FILE *f);
void free_str(string **s);
void print_line_detail(string *line, const char mode);
bool is_str_ok(const string *s);
char *get_c_string(const string *s);
string *str_init();
size_t str_len(const string *s);
kv_error_t create_string(string **restrict str, const bytes restrict s,
                         const size_t s_capacity);
// bytes str_chr(const string s, const char c);
