#include <kv/error.h>
#include <kv/string.h>

char *get_error_msg(kv_error_t e) {
    switch (e) {
    case KV_ERR_NONE:
        return "No Error Occured";
    case KV_ERR_MISC:
        return "Dummy Error";
    case KV_ERR_MEM_ALLOC:
        return "Dummy Error";
    case KV_ERR_INVAL_ARGS:
        return "Dummy Error";
    case KV_ERR_NOT_INIT:
        return "Dummy Error";
    default:
        return "";
    }
}
