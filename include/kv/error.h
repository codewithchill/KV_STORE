#pragma once

#include <kv/str.h>

typedef enum {
    KV_ERR_NONE = 0,       /* NO Error occured */
    KV_ERR_MISC = 1,       /* General/Misc occured */
    KV_ERR_MEM_ALLOC = 2,  /* Memory Allocation Failed */
    KV_ERR_INVAL_ARGS = 3, /* Invalid Arguments */
    KV_ERR_NOT_INIT = 4,   /* Variable non initialized */
    // ERR_
} error_t;

str get_error_msg(error_t e);
