#include <kv/ansi.h>
#include <kv/error.h>
#include <kv/string.h>
#include <stdio.h>

char *get_error_msg(kv_error_t e) {
  switch (e) {
  case KV_ERR_NONE:
    return "No Error Occured";
  case KV_ERR_MISC:
    return "Miscellaneous Error";
  case KV_ERR_MEM_ALLOC:
    return "Memory Allocation Failed";
  case KV_ERR_INVAL_ARG:
    return "Invalid Function Arguments";
  case KV_ERR_INVAL_TOK:
    return "Invalid tokens syntax";
  case KV_ERR_INVAL_CMD:
    return "Invalid Command";
  case KV_ERR_NOT_INIT:
    return "Variables Not Initialised";
  case KV_ERR_TOK_ORDER:
    return "Invalid cli arguments!\n" C_FG_BRIGHT_YELLOW
           "Usage: <cmd> <key> <val>\nUse HELP for more info!";
  case KV_ERR_INPUT:
    return "Invalid Input";
  case KV_ERR_CMD_ARGS:
    return "Invalid Command arguments\nHint: use `HELP`";

  default:
    return "Default Error" C_FG_CYAN "Function Should not reach here!" RESET;
  }
}

void kv_print_err(kv_error_t e) {
  printf(C_FG_BRIGHT_RED BOLD RAPID_BLINK "ERR: " RESET C_FG_RED "%s\n" RESET,
         get_error_msg(e));
}
