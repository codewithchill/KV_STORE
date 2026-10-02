// #include <kv/ansi.h>
#include "kv/data.h"
#include "kv/string.h"
#include <kv/cmds.h>
#include <kv/error.h>
#include <kv/macros.h>
#include <stdlib.h>

struct kv_cmd_t {
  cmd_t cmd;
  union {
    kv_error_t err;
    kv_pair *data;
  } val;
};

void cmd_set(data_node * /*kv_head*/) { return; }
void cmd_help() {
  // clang-format off
    printf(
        C_FG_BRIGHT_YELLOW
            "General Usage: <cmd> <key> <val>\n"
            "\nNOTE: For some commands either <key> or <value> or both may be "
            "optional.\n"
            "Available commands:\n\n"
            BOLD "    SET" NORMAL ": Set the <val> for the <value>\n"
            BOLD "    GET" NORMAL ": Get the value of the <key>\n"
            BOLD "    DEL" NORMAL ": Delete the key-value pair associated with <key>\n"
            BOLD "   SHOW" NORMAL ": Show all the keys and there corresponding value\n"
            BOLD "   HELP" NORMAL ": Prints this help menu.\n"
            BOLD "   EXIT" NORMAL ": Exits the Program.\n"
        RESET
    );
  // clang-format on
}

kv_cmd_t *init_cmd() {
  kv_cmd_t *c = malloc(sizeof(*c));
  if (IS_NULL(c))
    return NULL;
  set_cmd_err_value(c, KV_ERR_NOT_INIT);
  return c;
}
kv_error_t init_cmd_args(kv_cmd_t *c) {
  if (IS_NULL(c))
    return KV_ERR_INVAL_ARG;
  c->val.data = init_key_val();
  if (IS_NULL(c->val.data))
    return KV_ERR_MEM_ALLOC;
  return KV_ERR_NONE;
}
void set_cmd_type(kv_cmd_t *restrict c, const cmd_t cmd) {
  if (IS_NOT_NULL(c))
    c->cmd = cmd;
}
void set_cmd_err_value(kv_cmd_t *restrict c, const kv_error_t e) {
  if (IS_NOT_NULL(c)) {
    c->cmd = KV_CMD_ERR;
    c->val.err = e;
  }
}
bool is_cmd_ok(const kv_cmd_t *restrict c) {
  if (IS_NULL(c))
    return KV_ERR_INVAL_ARG;
  return IS_NOT_EQUAL(c->cmd, KV_CMD_ERR);
}
kv_error_t set_cmd_args(kv_cmd_t *c, string *restrict arg_1,
                        string *restrict args_2) {
  if (IS_NULL(c))
    return KV_ERR_INVAL_ARG;
  return set_kv_pair_key_val(c->val.data, arg_1, args_2);
}
kv_error_t get_cmd_err(const kv_cmd_t *restrict c) {
  if (IS_NULL(c))
    return KV_ERR_INVAL_ARG;
  if (!is_cmd_ok(c))
    return c->val.err;
  return KV_ERR_NONE;
}
void free_cmd(kv_cmd_t **c) {
  if (IS_NOT_NULL(c)) {
    if (IS_NOT_NULL(*c))
      if (IS_NOT_EQUAL((*c)->cmd, KV_CMD_ERR))
        free_kv_pair(&((*c)->val.data));
    free(*c);
    *c = NULL;
  }
}
kv_error_t exec_cmd(kv_cmd_t *cmd, bool *is_exit, data_node *kv_head) {
  kv_error_t e = KV_ERR_NOT_INIT;
  if (IS_NULL(cmd) || IS_NULL(is_exit))
    e = KV_ERR_INVAL_ARG;
  else if (IS_EQUAL(cmd->cmd, KV_CMD_ERR))
    e = KV_ERR_INVAL_CMD;
  else
    e = KV_ERR_NONE;
  switch (cmd->cmd) {
  case KV_CMD_SET:
    cmd_set(kv_head);
    printf("Setting Key: [%s] with value [%s]\n",
           get_c_string(get_key(cmd->val.data, &e)),
           get_c_string(get_val(cmd->val.data, &e)));
    break;
  case KV_CMD_GET:
    printf("Key: %s\nVal: Unknown\n", get_c_string(get_key(cmd->val.data, &e)));
    break;
  case KV_CMD_DEL:
    printf("Deleting key-value pair with key: %s\n",
           get_c_string(get_key(cmd->val.data, &e)));
    break;
  case KV_CMD_SHOW:
    printf("Printing all key-value pairs.....\n");
    break;
  case KV_CMD_HELP:
    cmd_help();
    break;
  case KV_CMD_EXIT:
    /* TODO: Handling all the memory, file wrintg etc */
    printf(C_FG_BRIGHT_BLUE BOLD RAPID_BLINK "Exiting.........\n" RESET);
    *is_exit = true;
    break;
  default:
    break;
  }
  return e;
}
