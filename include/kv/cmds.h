#pragma once

#include <kv/kv.h>

typedef enum cmd_t {
    KV_CMD_SET = 0x00,
    KV_CMD_GET,
    KV_CMD_DEL,
    KV_CMD_SHOW,
    KV_CMD_HELP,
    KV_CMD_EXIT,
    // AT THE END
    KV_CMD_ERR
} cmd_t;
typedef struct kv_cmd_t kv_cmd_t;

struct kv_cmd_t {
    cmd_t cmd;
    union {
        kv_error_t err;
        kv_pair data;
    } val;
};

void cmd_help();
void cmd_set(data_node* kv_head);
void free_cmd(kv_cmd_t *c);
kv_error_t exec_cmd(kv_cmd_t *cmd, bool *is_exit, data_node* kv_head);
