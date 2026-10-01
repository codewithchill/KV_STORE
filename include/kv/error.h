#pragma once

#include <kv/macros.h>
typedef enum {
  KV_ERR_NONE = 0,  /* NO Error occured */
  KV_ERR_MISC = 1,  /* General/Misc occured */
  KV_ERR_MEM_ALLOC, /* Memory Allocation Failed */
  KV_ERR_INVAL_ARG, /* Invalid Arguments */
  KV_ERR_INVAL_TOK, /* Error in token syntax */
  KV_ERR_INVAL_CMD, /* Invalid command */
  KV_ERR_NOT_INIT,  /* Variable non initialized */
  KV_ERR_TOK_ORDER, /* Error in order of tokens */
  KV_ERR_INPUT,     /* Error in user input */
  KV_ERR_CMD_ARGS,  /* Invalid commands arguments */
                    // KV_ERR_
                    // KV_ERR_STR_
} kv_error_t;

#define KV_IS_OK(e) IS_EQUAL(KV_ERR_NONE, (e))
#define KV_IS_ERROR(e) IS_NOT_EQUAL(KV_ERR_NONE, (e))

// #define IF_KV_IS_OK(e) if (KV_IS_OK((e)))
// #define IF_KV_IS_ERROR(e) if (KV_IS_OK((e)))

char *get_error_msg(kv_error_t e);
void kv_print_err(kv_error_t e);
