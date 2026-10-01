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

struct _str {
    bytes data;
    size_t byte_len;
    size_t utf_len;
    size_t capacity;
};

/* INFO: Private Functions */
static size_t utf_str_len(bytes restrict s) {
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

static _str *get_str(size_t capacity) {
    if (IS_EQUAL(capacity, 0))
        return NULL;
    _str *s = malloc(sizeof(*s));
    if (IS_NULL(s))
        return NULL;
    s->data = malloc(capacity);
    if (IS_NULL(s->data)) {
        s->data = NULL;
        free(s);
    }
    s->capacity = capacity;
    s->byte_len = 0;
    s->utf_len = 0;
    return s;
}

/* ======================================================
 * INFO: Public Functions
 *
 * ======================================================
 */

/*
 * c: Character Mode
 *
 * n: Number Mode
 *
 * m: Multiple Mode (c+n)
 */
void print_line_detail(string *s, const char mode) {
    if (IS_NULL(s))
        return;
    printf("------------------------------\n");
    printf("| Line Capacity:     [%4lu]  |\n"
           "| Line UTF8 Length:  [%4lu]  |\n"
           "| Line Bytes Length: [%4lu]  |\n"
           "| Line:              ",
           s->s->capacity, s->s->utf_len, s->s->byte_len);
    print_raw_string(s, mode);
    printf("------------------------------\n");
}
void get_line(string *restrict s, FILE *f) {
    if (IS_NULL(s) || IS_NULL(f))
        return;
    s->err = KV_ERR_NONE;
    s->s = get_str(_DEFAULT_LINE_SIZE);
    if (IS_NULL(s->s)) {
        s->err = KV_ERR_MEM_ALLOC;
        s->s = NULL;
        return;
    }
    size_t byte_count = 0;
    Rune c = fgetc(f);
    while (IS_NOT_EQUAL(c, '\n') && IS_NOT_EQUAL(c, EOF)) {
        if (IS_EQUAL(c, '\b')) {
            /* ERROR: Currently cannot handle if an utf8 2 or more byte
             *        is entered and '\b' is pressed.
             */
            if (byte_count > 0)
                byte_count--;
        } else if (IS_NOT_EQUAL(c, '\t') && c > 1 && c < 32) {
            c = fgetc(f);
            continue;
        } else {
            s->s->data[byte_count] = (uint8_t)c;
            byte_count++;
            if ((byte_count + 1) > s->s->capacity) {
                bytes temp = malloc(s->s->capacity + _DEFAULT_LINE_SIZE);
                if (!temp) {
                    s->err = KV_ERR_MEM_ALLOC;
                    free_str(s);
                }
                for (size_t i = 0; i < byte_count; i++)
                    temp[i] = s->s->data[i];

                s->s->capacity += _DEFAULT_LINE_SIZE;
                free(s->s->data);
                s->s->data = temp;
            }
        }
        c = fgetc(f);
    }
    s->s->data[byte_count] = '\0';
    s->s->byte_len = byte_count;
    s->s->utf_len = utf_str_len(s->s->data);
}
void free_str(string *s) {
    if (IS_NOT_NULL(s)) {
        if (IS_NOT_NULL(s->s)) {
            if (IS_NOT_NULL(s->s->data))
                free(s->s->data);
            free(s->s);
            s->s = NULL;
        }
    }
}
size_t str_len(const string *s) { return s->s->utf_len; }
string str_init() {
    string s = {.err = KV_ERR_NOT_INIT, .s = get_str(0)};
    return s;
}
kv_error_t create_string(string *restrict str, const bytes restrict s,
                         const size_t s_len) {
    if (!str || !s || IS_EQUAL(s_len, 0))
        return KV_ERR_INVAL_ARG;
    *str = str_init();
    str->s = get_str(s_len + 1);
    if (!str->s->data)
        return KV_ERR_MEM_ALLOC;
    memcpy(str->s->data, s, s_len);
    str->s->data[s_len] = '\0';
    str->s->byte_len = s_len;
    str->s->capacity = s_len + 1;
    str->s->utf_len = utf_str_len((const bytes)str->s->data);
    return KV_ERR_NONE;
}
bool is_str_ok(const string s) {
    if (IS_NOT_NULL(s.s) && IS_NOT_NULL(s.s->data) && s.s->capacity >= 1 &&
        s.s->byte_len >= 1)
        return true;
    return false;
}
char *get_c_string(const string *s) {
    if (s)
        return (char *)(s->s->data);
    return NULL;
}
