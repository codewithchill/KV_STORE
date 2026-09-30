#pragma once

#include <kv/error.h>
#include <kv/string.h>
#include <kv/kv.h>

typedef enum token_type {
    KV_TOK_CMD,
    KV_TOK_STR,
    KV_TOK_ERR,
} token_type;
typedef struct {
    token_type tok_t;
    union {
        kv_error_t err;
        string token;
    } v;
} token_t;
typedef struct {
    kv_error_t err;
    size_t token_count;
    size_t token_capacity;
    token_t **tokens;
} tokens_t;
typedef enum {
    KV_CMD_SET = 0x00,
    KV_CMD_GET,
    KV_CMD_DEL,
    KV_CMD_SHOW,
    KV_CMD_HELP,
    KV_CMD_EXIT,
    // AT THE END
    KV_CMD_ERR
} cmd_t;
typedef struct kv_cmd_t {
    cmd_t cmd;
    union {
        kv_error_t err;
        kv_pair data;
    } val;
} kv_cmd_t;

void free_tokens(tokens_t *t);
void free_cmd(kv_cmd_t *c);
tokens_t kv_get_tokens(string *line);
kv_cmd_t kv_parse_tokens(const tokens_t *tks);
kv_error_t exec_cmd(kv_cmd_t *cmd, bool *is_exit, data_node* kv_head);