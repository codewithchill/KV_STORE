#pragma once

#include <kv/cmds.h>
#include <kv/error.h>
#include <kv/kv.h>
#include <kv/string.h>

typedef enum token_type {
    KV_TOK_CMD,
    KV_TOK_STR,
    KV_TOK_ERR,
} token_type;
typedef struct {
    token_type tok_t;
    union {
        kv_error_t err;
        string *token;
    } v;
} token_t;
typedef struct {
    kv_error_t err;
    size_t token_count;
    size_t token_capacity;
    token_t **tokens;
} tokens_t;

void free_tokens(tokens_t *t);
void print_all_tokens(tokens_t *tok);
tokens_t kv_get_tokens(string *line);
kv_cmd_t *kv_parse_tokens(const tokens_t *tks);
