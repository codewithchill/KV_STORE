#include <kv/error.h>

str get_error_msg(error_t e) {
    str s = {.data = NULL, .byte_len = 0, .capacity = 0, .utf_len = 0};
    switch (e) {
    case KV_ERR_NONE:
        return s;
    case KV_ERR_MISC:
        return s;
    case KV_ERR_MEM_ALLOC:
        return s;
    case KV_ERR_INVAL_ARGS:
        return s;
    case KV_ERR_NOT_INIT:
        return s;
    default:
        return s;
    }
}
