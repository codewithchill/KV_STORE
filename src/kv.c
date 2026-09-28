#include <kv/ansi.h>
#include <kv/error.h>
#include <kv/file.h>
#include <kv/kv.h>
#include <kv/macros.h>
#include <kv/string.h>
#include <kv/version.h>

#include <stdbool.h>
#include <stddef.h>
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
typedef struct {
    string *key, *val;
} kv_pair;
typedef struct data_node data_node;
struct data_node {
    kv_error_t err;
    kv_pair data;
    data_node *next, *prev;
};

const char *__cmds[] = {"SET", "GET", "DEL", "SHOW", "EXIT"};
typedef struct {
    enum {
        KV_CMD_SET = 0x00,
        KV_CMD_GET = 0x01,
        KV_CMD_DEL = 0x02,
        KV_CMD_SHOW = 0x03,
        KV_CMD_EXIT = 0x04,
        // AT THE END
        KV_CMD_ERR
    } cmd;
    union {
        kv_error_t err;
        kv_pair *data;
    } val;
} kv_cmd_t;
typedef struct {
    kv_error_t err;
    string *token;
    enum {
        KV_TOKEN_ERROR,
        KV_TOKEN_CMD,
        KV_TOKEN_STR,
    } token_type;
} token_t;
typedef struct {
    kv_error_t err;
    size_t token_count;
    token_t **tokens;
} tokens_t;

/*
static tokens_t kv_get_tokens(string * line) {
    tokens_t tkns = {.err = KV_ERR_NOT_INIT, .token_count = 0, .tokens = NULL};
    return tkns;
}
static kv_cmd_t kv_parse_tokens(const tokens_t * tokens) {
    kv_cmd_t cmd = {.cmd = KV_CMD_ERR, .val.err = KV_ERR_NOT_INIT};
    return cmd;
}
static kv_error_t exec_cmd(kv_cmd_t * cmd) {
    kv_error_t e = KV_ERR_NOT_INIT;
    return e;
}
*/
/*
kv_cmd_t kv_parse_cmd(string *line) {
    kv_cmd_t _cmd = {
        .cmd = KV_CMD_ERR, .val.err = KV_ERR_NOT_INIT
        //.val = {.data = {.key = NULL, .val = NULL}
    };

if (!line || !line->data || line->byte_len <= 0 || line->capacity <= 0)
    return _cmd;

tokens_t tokens = kv_get_tokens(line);
if (!KV_IS_OK(tokens.err))
    return _cmd;

// return kv_parse_tokens(tokens);
return _cmd;
}
*/

static kv_error_t repl() {
    kv_error_t status = 0;
    while (true) {
        printf("__$ ");
        string line = str_init();
        get_line(&line, stdin);
        if (KV_IS_OK(line.err) && line.s->data) {
            // print_line_detail(&line, 'm');
            /*
            tokens_t tokens = kv_get_tokens(&line);
            if (KV_IS_ERROR(tokens.err)) {
                kv_print_err(tokens.err);
                status = tokens.err;
            }
            kv_cmd_t cmd = kv_parse_tokens(&tokens);
            if (IS_EQUAL(cmd.cmd, KV_CMD_ERR)) {
                kv_print_err(cmd.val.err);
                status = cmd.val.err;
            }
            status = exec_cmd(&cmd);
             */
            /* TODO: Temporary exit function to be removed */
            if (!strncmp(__cmds[KV_CMD_EXIT], (const char *)(line.s->data),
                         strlen(__cmds[KV_CMD_EXIT]))) {
                free_str(&line);
                status = EXIT_SUCCESS;
                break;
            }
            free_str(&line);
        }
    }
    return status;
}

static int kv_start(kv_args Args) {
    if (IS_NOT_EQUAL(Args.flg, KV_ARGS_FILE_PATH) || !Args.val.filepath)
        return EXIT_FAILURE;

    int status = EXIT_SUCCESS;
    // data_node *kv_head = load_from_file_if_exists(Args.val.filepath);

    status = repl(); /* TODO: To parse kv_error_t */

    return status;
}
static void kv_print_help(const char *restrict PROG_NAME,
                          const char *restrict VERSION,
                          const char *restrict msg) {
    const char *str = "\t-f <file_path>: Uses the file to load ans save data.\n"
                      "\t                If no path give saved in the current "
                      "working directory.\n\n"
                      "\t-h\n"
                      "\t--help        : Prints this help menu.\n";
    printf("[%s] Version: %s\n%s\n%s", PROG_NAME, VERSION, msg, str);
}

// static void kv_print_all_args(const int argc, const char **restrict argv) {
//     printf("Argument Count: [%d]\n", argc);
//     for (int i = 0; argc >= i && IS_NOT_EQUAL(argv[i], NULL); i++)
//         printf("[%02d]: [%s]\n", i, argv[i]);
//     printf("\n");
// }
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
