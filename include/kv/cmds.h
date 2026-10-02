#pragma once

#include <kv/data.h>
#include <kv/error.h>

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

void cmd_help();
void cmd_set(data_node *kv_head);

kv_cmd_t *init_cmd();
kv_error_t init_cmd_args(kv_cmd_t *c);

void set_cmd_type(kv_cmd_t *restrict c, const cmd_t cmd);
void set_cmd_err_value(kv_cmd_t *restrict c, const kv_error_t e);
kv_error_t set_cmd_args(kv_cmd_t *c, string *restrict arg_1,
                        string *restrict args_2);
bool is_cmd_ok(const kv_cmd_t *restrict c);
kv_error_t get_cmd_err(const kv_cmd_t *restrict c);

void free_cmd(kv_cmd_t **c);

kv_error_t exec_cmd(kv_cmd_t *cmd, bool *is_exit, data_node *kv_head);
