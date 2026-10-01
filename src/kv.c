#include <kv/ansi.h>
#include <kv/cmds.h>
#include <kv/error.h>
#include <kv/file.h>
#include <kv/kv.h>
#include <kv/macros.h>
#include <kv/parse.h>
#include <kv/string.h>
#include <kv/version.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct kv_args {
  enum { KV_ARGS_ERR = 0, KV_ARGS_HELP, KV_ARGS_FILE_PATH } flg;
  union {
    kv_error_t err;
    const char *filepath;
  } val;
} kv_args;

static kv_error_t repl(data_node *kv_head) {
  kv_error_t status = 0;
  bool is_exit = false;
  // stdin = freopen("./private/input.txt", "r", stdin);
  while (true) {
    printf(C_FG_BRIGHT_GREEN "__$ " RESET);
    string *line = str_init();
    get_line(line, stdin);
    if (is_str_ok(line)) {
      tokens_t t = kv_get_tokens(line);
      if (KV_IS_ERROR(t.err)) {
        kv_print_err(t.err);
        status = t.err;
        free_tokens(&t);
        free_str(&line);
        continue;
      }

      kv_cmd_t *cmd = kv_parse_tokens(&t);
      if (!is_cmd_ok(cmd)) {
        status = get_cmd_err(cmd);
        kv_print_err(status);
        free_cmd(&cmd);
        free_tokens(&t);
        free_str(&line);
        continue;
      }
      status = exec_cmd(cmd, &is_exit, kv_head);
      if (KV_IS_ERROR(status))
        kv_print_err(status);
      free_cmd(&cmd);
      free_tokens(&t);
      free_str(&line);
      if (is_exit)
        break;
    }
  }
  return status;
}
static int kv_start(kv_args Args) {
  if (IS_NOT_EQUAL(Args.flg, KV_ARGS_FILE_PATH) || !Args.val.filepath)
    return EXIT_FAILURE;
  int status = EXIT_SUCCESS;
  // data_node *kv_head = load_from_file_if_exists(Args.val.filepath);
  data_node *kv_head = NULL;

  status = repl(kv_head); /* TODO: To parse kv_error_t */

  return status;
}
static void kv_print_help(const char *restrict PROG_NAME,
                          const char *restrict VERSION,
                          const char *restrict msg) {
  const char *str = "\nUsage: [" _PROGRAM "] [-f | -h] [FILE_NAME]\n"
                    "\t-f <file_path>: Uses the file to load ans save data.\n"
                    "\t                If no path give saved in the current "
                    "working directory.\n\n"
                    "\t-h\n"
                    "\t--help        : Prints this help menu.\n";
  printf(C_FG_BRIGHT_CYAN "[%s] Version: %s\n%s\n" RESET C_FG_BRIGHT_YELLOW
                          "%s" RESET,
         PROG_NAME, VERSION, msg, str);
}
/*static void kv_print_all_args(const int argc, const char **restrict argv) {
    printf("Argument Count: [%d]\n", argc);
    for (int i = 0; argc >= i && IS_NOT_EQUAL(argv[i], NULL); i++)
        printf("[%02d]: [%s]\n", i, argv[i]);
    printf("\n");
}*/
static kv_args kv_parse_args(const int argc, const char **restrict argv) {
  // kv_print_all_args(argc, argv);
  kv_args arg = {.flg = KV_ARGS_ERR, .val.filepath = NULL};

  if (IS_EQUAL(argc, 2) && !strcmp("-f", argv[1]))
    arg.flg = KV_ARGS_ERR;
  if ((argc > 3) || (IS_EQUAL(argc, 2) && !strcmp("-f", argv[1])))
    arg.flg = KV_ARGS_ERR;

  if (IS_EQUAL(argc, 2) &&
      (!strcmp("--help", argv[1]) || !strcmp("-h", argv[1])))
    arg.flg = KV_ARGS_HELP;

  if (IS_EQUAL(argc, 1)) {
    arg.flg = KV_ARGS_FILE_PATH;
    arg.val.filepath = _DEFAULT_FILE;
  }
  if (IS_EQUAL(argc, 3) && !strcmp("-f", argv[1])) {
    arg.flg = KV_ARGS_FILE_PATH;
    arg.val.filepath = argv[2];
  }
  return arg;
}

/* INFO: Public Functions */
int kv(const int argc, const char **restrict argv) {
  int ret_code = 0;
  kv_args Args = kv_parse_args(argc, argv);

  switch (Args.flg) {
  case KV_ARGS_ERR:
    kv_print_help(_PROGRAM, _VERSION,
                  C_FG_BRIGHT_RED BOLD "Invalid Arguments!" RESET);
    ret_code = EXIT_FAILURE;
    break;
  case KV_ARGS_HELP:
    kv_print_help(_PROGRAM, _VERSION, "");
    ret_code = EXIT_SUCCESS;
    break;
  case KV_ARGS_FILE_PATH:
    ret_code = kv_start(Args);
    break;
  default:
    printf(C_FG_RED "Unknown Flag Value!" RESET);
    ret_code = EXIT_FAILURE;
    break;
  }
  return ret_code;
}
