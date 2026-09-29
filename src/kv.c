#include <kv/ansi.h>
#include <kv/error.h>
#include <kv/file.h>
#include <kv/kv.h>
#include <kv/macros.h>
#include <kv/string.h>
#include <kv/version.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    string *key, *val;
} kv_pair;
typedef struct kv_args {
    enum { KV_ARGS_ERR = 0, KV_ARGS_HELP, KV_ARGS_FILE_PATH } flg;
    union {
        kv_error_t err;
        const char *filepath;
    } val;
} kv_args;
typedef struct data_node {
    kv_error_t err;
    kv_pair data;
    struct data_node *next, *prev;
} data_node;
typedef enum token_type {
    KV_TOKEN_CMD,
    KV_TOKEN_STR,
    KV_TOKEN_ERR,
} token_type;
typedef struct {
    token_type tok_t;
    union {
        kv_error_t err;
        string token;
    } v;
} token_t;
typedef struct {
    kv_error_t err;
    size_t token_count;
    size_t token_capacity;
    token_t **tokens;
} tokens_t;

static const char *__cmds[] = {"SET", "GET", "DEL", "SHOW", "EXIT"};
constexpr size_t n_cmds = 5;
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

static const char *get_token_type_str(token_type t) {
    switch (t) {
    case KV_TOKEN_CMD:
        return "CMD";
    case KV_TOKEN_ERR:
        return "ERR";
    case KV_TOKEN_STR:
        return "STR";
    default:
        return "";
    }
}
static bool isWhiteSpace(byte c) {
    /* TODO: Improve to support rune also*/
    return (IS_EQUAL(c, '\n') || IS_EQUAL(c, '\v') || IS_EQUAL(c, '\t') ||
            IS_EQUAL('\r', c) || IS_EQUAL(c, '\f') || IS_EQUAL(c, ' '));
}
static bytes skip_white_space(bytes data) {
    if (IS_NULL(data))
        return NULL;
    while (*data != 0 && isWhiteSpace(*data))
        data++;
    return data;
}
static bytes get_end(bytes start) {
    if (IS_NULL(start))
        return NULL;
    while (*start != '\0' && !isWhiteSpace(*start))
        start++;
    return start;
}
/*static bytes get_next_char(bytes s, byte c) {
    // TODO: Improve to support rune also

    while (IS_NOT_EQUAL(*s, '\0') && IS_NOT_EQUAL(*s, c))
        s++;
    return s;
}*/
static token_type get_tok_type(string *s) {
    token_type t = KV_TOKEN_ERR;
    bool is_set = false;
    for (size_t i = 0; i < n_cmds; i++) {
        auto s_len = s->s.byte_len;
        auto cmd_len = strlen(__cmds[i]);
        if (IS_NOT_EQUAL(s_len, cmd_len))
            continue;
        if (!strcmp(get_c_string(s), __cmds[i])) {
            is_set = true;
            t = KV_TOKEN_CMD;
        }
    }
    if (!is_set)
        t = KV_TOKEN_STR;
    return t;
}
static token_t get_next_token(bytes *data) {
    token_t t = {.tok_t = KV_TOKEN_ERR, .v.err = KV_ERR_NOT_INIT};
    enum {
        KV_PARSE_NORMAL,
        KV_PARSE_SINGLE_QUOTE,
        KV_PARSE_DOUBLE_QUOTE
    } state = KV_PARSE_NORMAL;
    *data = skip_white_space(*data);
    bytes start = *data, end = *data;
    while (IS_NOT_EQUAL(**data, '\0')) {
        switch (state) {
        case KV_PARSE_NORMAL:
            if (IS_EQUAL(**data, '"'))
                state = KV_PARSE_DOUBLE_QUOTE;
            else if (IS_EQUAL(**data, '\''))
                state = KV_PARSE_SINGLE_QUOTE;
            else {
                end = get_end(*data);
                ptrdiff_t c_count = end - start;
                string s;
                kv_error_t e = create_string(&s, *data, c_count);
                if (KV_IS_ERROR(e)) {
                    kv_print_err(e);
                    free_str(&s);
                    t.tok_t = KV_TOKEN_ERR;
                    t.v.err = e;
                    return t;
                }
                t.tok_t = get_tok_type(&s);
                if (IS_EQUAL(t.tok_t, KV_TOKEN_ERR)) {
                    free_str(&s);
                    t.v.err = KV_ERR_TOK_INVAL;
                }
                t.v.token = s;
                *data = end;
                return t;
            }
            break;
        case KV_PARSE_DOUBLE_QUOTE:
        case KV_PARSE_SINGLE_QUOTE:
        default:
            return t;
            break;
        }
    }
    return t;
}
static tokens_t kv_get_tokens(string *line) {
    tokens_t tkns = {.err = KV_ERR_NOT_INIT,
                     .token_count = 0,
                     .token_capacity = 0,
                     .tokens = NULL};
    constexpr int32_t DEFAULT_TOK_LIMIT = 3;

    if (IS_NULL(line) || KV_IS_ERROR(line->err)) {
        tkns.err = KV_ERR_INVAL_ARGS;
        return tkns;
    }

    tkns.token_capacity = DEFAULT_TOK_LIMIT + 1;
    tkns.tokens = malloc(sizeof(*tkns.tokens) * tkns.token_capacity);
    if (!tkns.tokens) {
        tkns.err = KV_ERR_MEM_ALLOC;
        tkns.token_capacity = 0;
        return tkns;
    }
    for (size_t i = 0; i < DEFAULT_TOK_LIMIT; i++)
        tkns.tokens[i] = NULL;
    auto data = line->s.data;

    while (true) {
        token_t t = get_next_token(&data);
        if (IS_EQUAL(t.tok_t, KV_TOKEN_ERR)) {
            if (IS_NOT_EQUAL(t.v.err, KV_ERR_NOT_INIT))
                tkns.err = t.v.err;
            break;
        }
        if (IS_EQUAL(tkns.token_count, DEFAULT_TOK_LIMIT)) {
            free_str(&t.v.token);
            tkns.err = KV_ERR_INPUT;
            break;
        }
        tkns.tokens[tkns.token_count] =
            malloc(sizeof(*tkns.tokens[tkns.token_count]));
        if (!tkns.tokens[tkns.token_count]) {
            free_str(&t.v.token);
            tkns.err = KV_ERR_MEM_ALLOC;
            break;
        }
        *tkns.tokens[tkns.token_count] = t;
        tkns.token_count++;
    }
    if (IS_EQUAL(tkns.err, KV_ERR_NOT_INIT))
        tkns.err = KV_ERR_NONE;
    tkns.tokens[tkns.token_count] = NULL;
    return tkns;
}
/*
static kv_cmd_t kv_parse_tokens(const tokens_t *ts) {
    kv_cmd_t cmd = {.cmd = KV_CMD_ERR, .val.err = KV_ERR_NOT_INIT};
    if (IS_NULL(ts) || KV_IS_ERROR(ts->err) ||
        IS_EQUAL(ts->token_capacity, 0) || IS_EQUAL(ts->token_count, 0) ||
        IS_NULL(ts->tokens)) {
        cmd.val.err = KV_ERR_INVAL_ARGS;
        return cmd;
    }
    
    return cmd;
}
static kv_error_t exec_cmd(kv_cmd_t * cmd) {
    kv_error_t e = KV_ERR_NOT_INIT;
    return e;
}
*/
static void print_all_tokens(tokens_t *tok) {
    if (IS_NULL(tok) || KV_IS_ERROR(tok->err))
        return;
    size_t i = 0;
    while (i < tok->token_count && (tok->tokens)[i] != NULL) {
        auto t = tok->tokens[i];
        printf("[" C_FG_BRIGHT_YELLOW "%02lu" RESET "] [" C_FG_BRIGHT_YELLOW
               "%s" RESET "] ",
               i, get_token_type_str(t->tok_t));
        if (IS_EQUAL(tok->tokens[i]->tok_t, KV_TOKEN_ERR))
            printf("[" C_FG_BRIGHT_YELLOW "%s" RESET "]\n",
                   get_error_msg(t->v.err));
        else
            printf("[" C_FG_BRIGHT_YELLOW "%s" RESET "]\n", t->v.token.s.data);
        i++;
    }
}
static void free_tokens(tokens_t *t) {
    if (IS_NOT_NULL(t)) {
        for (size_t i = 0; i < t->token_count; i++) {
            token_t *tok = t->tokens[i];
            if (IS_NOT_NULL(tok)) {
                /*
                 * NOTE:
                 * .
                 * KV_TOKEN_CMD and KV_TOKEN_STR contain an owning
                 * string whose data was allocated by create_string().
                 * .
                 * KV_TOKEN_ERR contains an error value in the union,
                 * so there is no string to free.
                 */
                if (tok->tok_t == KV_TOKEN_CMD || tok->tok_t == KV_TOKEN_STR)
                    free_str(&tok->v.token);
                free(tok);
                t->tokens[i] = NULL;
            }
        }
        free(t->tokens);
        t->tokens = NULL;
        t->err = KV_ERR_NONE;
        t->token_count = 0;
        t->token_capacity = 0;
    }
}
static kv_error_t repl() {
    kv_error_t status = 0;
    // stdin = freopen("./private/input.txt", "r", stdin);
    while (true) {
        printf(C_FG_BRIGHT_GREEN "__$ " RESET);
        string line = str_init();
        get_line(&line, stdin);
        if (KV_IS_OK(line.err) && line.s.data) {
            // print_line_detail(&line, 'm');
            tokens_t t = kv_get_tokens(&line);
            if (KV_IS_ERROR(t.err)) {
                kv_print_err(t.err);
                status = t.err;
            }
            print_all_tokens(&t);
            /*
            kv_cmd_t cmd = kv_parse_tokens(&tokens);
            if (IS_EQUAL(cmd.cmd, KV_CMD_ERR)) {
                kv_print_err(cmd.val.err);
                status = cmd.val.err;
            }
            status = exec_cmd(&cmd);
            free_cmd(&cmd);
             */
            free_tokens(&t);
            /* TODO: Temporary exit function to be removed */
            if (!strncmp(__cmds[KV_CMD_EXIT], get_c_string(&line),
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
