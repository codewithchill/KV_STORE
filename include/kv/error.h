#pragma once

typedef enum {
    ERR_NULL = 0,       /* NO Error occured */
    ERR_MISC = 1,       /* General/Misc occured */
    ERR_MEM_ALLOC = 2,  /* Memory Allocation Failed */
    ERR_INVAL_ARGS = 3, /* Invalid Arguments */
    // ERR_
} error_t;
