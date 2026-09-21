#include <kv/colors.h>
#include <kv/file.h>
#include <kv/init.h>
#include <kv/str.h>
#include <kv/version.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct kv_args {
    enum { KV_ERR = 0, KV_HELP = 1, KV_FILE_PATH = 2 } flg;
    union {
        const char *filepath;
    } val;
} kv_args;

const char *__cmds[] = {"SET", "GET", "DEL", "SHOW", "EXIT"};
typedef enum {
    KV_CMD_SET = 0x00,
    KV_CMD_GET = 0x01,
    KV_CMD_DEL = 0x02,
    KV_CMD_SHOW = 0x03,
    KV_CMD_EXIT = 0x04,
    // AT THE END
    KV_CMD_ERR
} KV_CMD;

typedef struct {
    KV_CMD cmd;
    kv_pair data;
} kv_cmd_t;

str **kv_parse_get_tokens(str *line) {
    if (!line || !line->data || line->len <= 0 || line->capacity <= 0)
        return NULL;
    return NULL;
}

kv_cmd_t kv_parse_cmd(str *line) {
    kv_cmd_t _cmd = {.cmd = KV_CMD_ERR, .data = {.key = NULL, .val = NULL}};

    if (!line || !line->data || line->len <= 0 || line->capacity <= 0)
        return _cmd;

    str **tokens = kv_parse_get_tokens(line);
    if (!tokens)
        return _cmd;

    /*
     if (!strncmp(__cmds[KV_CMD_EXIT], (const char *)line.data,
                  strlen(__cmds[KV_CMD_EXIT]))) {
         free(line.data);
         status = EXIT_SUCCESS;
         break;
     }
     */

    return _cmd;
}

int kv_start(kv_args Args) {
    if (Args.flg != KV_FILE_PATH || !Args.val.filepath)
        return EXIT_FAILURE;

    int status = EXIT_SUCCESS;
    // data_node *kv_head = load_from_file_if_exists(Args.val.filepath);

    /*
     * start repl loop
     * command  -> lexer -> parse
     * execute command
     * upon exit return status;
     */
    while (true) {
        printf("__$ ");
        str line = get_line(stdin);
        if (line.data) {
            print_line_detail(&line);
            // kv_cmd_t command = kv_parse_cmd(&line);
            if (!strncmp(__cmds[KV_CMD_EXIT], (const char *)line.data,
                         strlen(__cmds[KV_CMD_EXIT]))) {
                free(line.data);
                status = EXIT_SUCCESS;
                break;
            }
            free(line.data);
        }
    }

    return status;
}
void kv_print_help(const char *restrict PROG_NAME, const char *restrict VERSION,
                   const char *restrict msg) {
    const char *str = "\t-f <file_path>: Uses the file to load ans save data.\n"
                      "\t                If no path give saved in the current "
                      "working directory.\n\n"
                      "\t-h\n"
                      "\t--help        : Prints this help menu.\n";
    printf("[%s] Version: %s\n%s\n%s", PROG_NAME, VERSION, msg, str);
}
void kv_print_all_args(const int argc, const char **restrict argv) {
    printf("Argument Count: [%d]\n", argc);
    for (int i = 0; argc >= i && argv[i] != NULL; i++)
        printf("[%02d]: [%s]\n", i, argv[i]);
    printf("\n");
}
kv_args kv_parse_args(const int argc, const char **restrict argv) {
    // kv_print_all_args(argc, argv);
    kv_args arg = {.flg = KV_ERR, .val.filepath = NULL};

    if (argc == 2 && !strcmp("-f", argv[1]))
        arg.flg = KV_ERR;
    if ((argc > 3) || (argc == 2 && !strcmp("-f", argv[1])))
        arg.flg = KV_ERR;

    if (argc == 2 && (!strcmp("--help", argv[1]) || !strcmp("-h", argv[1])))
        arg.flg = KV_HELP;

    if (1 == argc) {
        arg.flg = KV_FILE_PATH;
        arg.val.filepath = _DEFAULT_FILE;
    }
    if (3 == argc && !strcmp("-f", argv[1])) {
        arg.flg = KV_FILE_PATH;
        arg.val.filepath = argv[2];
    }
    return arg;
}
int kv_init(const int argc, const char **restrict argv) {
    int ext_code = 0;
    kv_args Args = kv_parse_args(argc, argv);
    switch (Args.flg) {
    case KV_ERR:
        kv_print_help(_PROGRAM, _VERSION, C_RED "Invalid Arguments!" C_RESET);
        ext_code = EXIT_FAILURE;
        break;
    case KV_HELP:
        kv_print_help(_PROGRAM, _VERSION, "");
        ext_code = EXIT_SUCCESS;
        break;
    case KV_FILE_PATH:
        ext_code = kv_start(Args);
        break;
    default:
        break;
    }
    return ext_code;
}
