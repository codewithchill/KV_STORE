#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <kv/error.h>
#include <kv/kv.h>
#include <kv/macros.h>
#include <kv/string.h>

#define _1_KB 1024
#define _DEFAULT_LINE_SIZE (_1_KB / 4)

static size_t utf_str_len(const uint8_t *s) {
    size_t count = 0;
    while (*s) {
        count += ((*s & 0xC0) != 0x80);
        s++;
    }
    return count;
}
static void print_loop_single(const char *restrict format, const string *line) {
    assert(format != NULL && line != NULL);
    size_t byte_len = line->s->byte_len;
    size_t i = 0;
    while ('\0' != line->s->data[i] && i < byte_len) {
        printf(format, line->s->data[i]);
        i++;
    }
    printf("\n");
}
static void print_loop_double(const char *restrict format, const string *line) {
    assert(format != NULL && line != NULL);
    size_t byte_len = line->s->byte_len;
    size_t i = 0;
    while ('\0' != line->s->data[i] && i < byte_len) {
        printf(format, line->s->data[i], line->s->data[i]);
        i++;
    }
    printf("\n");
}
static void print_raw_string(string *restrict line, const char mode) {
    if (!line)
        return;
    if (line->s->capacity < 1 || !line->s->data)
        return;
    switch (mode) {
    case 'c':
        print_loop_single("[%c] ", line);
        break;
    case 'm':
        print_loop_double("[%c-%u] ", line);
        break;
    case 'n':
        printf("const char *restrict  _Nonnull  _Nonnull format, ...\n");
        print_loop_single("[%u] ", line);
        break;
    }
    return;
}

/*
 * c: Character Mode
 *
 * n: Number Mode
 *
 * m: Multiple Mode (c+n)
 */
void print_line_detail(string *s, const char mode) {
    if (!s || !(s->s))
        return;
    printf("------------------------------\n");
    printf("Line Capacity:     [%lu]\n"
           "Line UTF8 Length:  [%lu]\n"
           "Line Bytes Length: [%lu]\n"
           "Line:              ",
           s->s->capacity, s->s->utf_len, s->s->byte_len);
    print_raw_string(s, mode);
    printf("------------------------------\n");
}

/*
 * cap = Capacity of the string
 */
static _str *get_str(size_t cap) {
    _str *s = malloc(sizeof(_str));
    if (!s)
        return NULL;
    s->data = malloc(cap);
    s->capacity = cap;
    if (cap != 0 && IS_EQUAL(s->data, NULL)) {
        free(s);
        s = NULL;
    }
    return s;
}

void get_line(string *s, FILE *f) {
    if (!s || !f)
        return;
    s->err = KV_ERR_NONE;
    s->s = get_str(_DEFAULT_LINE_SIZE);
    if (!s->s) {
        s->err = KV_ERR_MEM_ALLOC;
        free_str(s);
        return;
    }
    size_t char_count = 0;
    int c = fgetc(f);
    while (IS_NOT_EQUAL(c, '\n') && IS_NOT_EQUAL(c, EOF)) {
        if (IS_EQUAL(c, '\b')) {
            /* ERROR: Currently cannot handle id an utf8 2 or more byte
             *        is entered and '\b' is pressed.
             */
            if (char_count > 0)
                char_count--;
        } else if (IS_NOT_EQUAL(c, '\n') && c > 1 && c < 32) {
            c = fgetc(f);
            continue;
        } else {
            s->s->data[char_count] = (uint8_t)c;
            char_count++;
            if ((char_count + 1) > s->s->capacity) {
                uint8_t *temp = malloc(s->s->capacity + _DEFAULT_LINE_SIZE);
                if (!temp) {
                    s->err = KV_ERR_MEM_ALLOC;
                    free_str(s);
                }
                for (size_t i = 0; i < char_count; i++)
                    temp[i] = s->s->data[i];

                s->s->capacity += _DEFAULT_LINE_SIZE;
                free(s->s->data);
                s->s->data = temp;
            }
        }
        c = fgetc(f);
    }
    s->s->data[char_count] = '\0';
    s->s->byte_len = char_count;
    s->s->utf_len = utf_str_len(s->s->data);
}

static kv_error_t create_string(string *restrict str, const char *restrict s,
                                const size_t s_capacity) {
    if (!str || !s || IS_EQUAL(s_capacity, 0))
        return KV_ERR_INVAL_ARGS;
    str->s = malloc(sizeof(_str));
    if (str->s) {
        str->s->byte_len = strlen((const char *)s);
        str->s->utf_len = utf_str_len((const uint8_t *)s);
        str->s->capacity = s_capacity;
        str->s->data = (uint8_t *)strndup((const char *)s, str->s->byte_len);
    } else {
        return KV_ERR_MEM_ALLOC;
    }

    return KV_ERR_NONE;
}
void free_str(string *s) {
    if (IS_NOT_EQUAL(s, NULL)) {
        if (IS_NOT_EQUAL(s->s, NULL)) {
            if (IS_NOT_EQUAL(s->s->data, NULL))
                free(s->s->data);
            free(s->s);
        }
        s->err = KV_ERR_NONE;
        s->s = NULL;
    }
}
size_t str_len(const string *s) { return s->s->utf_len; }
string str_init() {
    string s = {.err = KV_ERR_NONE, .s = NULL};
    return s;
}
