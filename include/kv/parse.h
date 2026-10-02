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

typedef struct token_t token_t;
typedef struct tokens_t tokens_t;

kv_error_t get_tok_err(const tokens_t *t);
bool is_token_ok(const tokens_t* t);
void free_tokens(tokens_t **t);
void print_all_tokens(tokens_t *tok);
tokens_t *kv_get_tokens(string *line);
kv_cmd_t *kv_parse_tokens(const tokens_t *tks);
