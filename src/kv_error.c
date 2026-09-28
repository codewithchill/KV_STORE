#include <kv/ansi.h>
#include <kv/error.h>
#include <kv/string.h>
#include <stdio.h>

static char *get_error_msg(kv_error_t e) {
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

void kv_print_err(kv_error_t e) {
    printf(C_FG_BRIGHT_RED BOLD RAPID_BLINK "ERR: " RESET C_FG_RED "%s\n" RESET,
           get_error_msg(e));
}
