#pragma once

typedef enum {
    KV_ERR_NONE = 0,   /* NO Error occured */
    KV_ERR_MISC = 1,   /* General/Misc occured */
    KV_ERR_MEM_ALLOC,  /* Memory Allocation Failed */
    KV_ERR_INVAL_ARGS, /* Invalid Arguments */
    KV_ERR_NOT_INIT,   /* Variable non initialized */
    KV_ERR_TOK_INVAL,  /* Error in token syntax */
    // KV_ERR_STR_
    // KV_ERR_STR
    // KV_ERR_
} kv_error_t;

#define KV_IS_OK(e) (KV_ERR_NONE==(e))
#define KV_IS_ERROR(e) (KV_ERR_NONE!=(e))

// #define IF_KV_IS_OK(e) if (KV_IS_OK((e)))
// #define IF_KV_IS_ERROR(e) if (KV_IS_OK((e)))

// char* get_error_msg(kv_error_t e);
void kv_print_err(kv_error_t e);
