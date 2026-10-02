#include <kv/cmds.h>
#include <kv/data.h>
#include <kv/error.h>
#include <kv/kv.h>
#include <kv/parse.h>
#include <kv/string.h>

#include <stdlib.h>
#include <string.h>

static constexpr int32_t DEFAULT_TOK_LIMIT = 3;
static constexpr char __cmds[][5] = {"SET",  "GET",  "DEL",
                                     "SHOW", "HELP", "EXIT"};
static constexpr size_t n_cmds = sizeof(__cmds) / sizeof(__cmds[0]);
// clang-format off
static const struct {
cmd_t cmd;
const char *cmd_str;
uint8_t noOfArgs;
} cmds[] = {
{.cmd = KV_CMD_SET, .cmd_str = __cmds[KV_CMD_SET], .noOfArgs = 2}, /* <key>, <val> */
{.cmd = KV_CMD_GET, .cmd_str = __cmds[KV_CMD_GET], .noOfArgs = 1}, /* <key> */
{.cmd = KV_CMD_DEL, .cmd_str = __cmds[KV_CMD_DEL], .noOfArgs = 1}, /* <key> */
{.cmd = KV_CMD_SHOW, .cmd_str = __cmds[KV_CMD_SHOW], .noOfArgs = 0},  /*  */
{.cmd = KV_CMD_HELP, .cmd_str = __cmds[KV_CMD_HELP], .noOfArgs = 0},  /*  */
{.cmd = KV_CMD_EXIT, .cmd_str = __cmds[KV_CMD_EXIT], .noOfArgs = 0}}; /*  */
// clang-format on

static const char *get_token_type_str(token_type t) {
  switch (t) {
  case KV_TOK_CMD:
    return "CMD";
  case KV_TOK_ERR:
    return "ERR";
  case KV_TOK_STR:
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
static bytes get_next_char(bytes s, byte c) {
  // TODO: Improve to support rune also
  if (IS_NULL(s))
    return NULL;
  while (IS_NOT_EQUAL(*s, '\0') && IS_NOT_EQUAL(*s, c))
    s++;
  return s;
}
static token_type get_tok_type(string *s) {
  token_type t = KV_TOK_ERR;
  bool is_set = false;
  for (size_t i = 0; i < n_cmds; i++) {
    auto s_len = str_len(s);
    auto cmd_len = strlen(__cmds[i]);
    if (IS_NOT_EQUAL(s_len, cmd_len))
      continue;
    if (!strcmp(get_c_string(s), __cmds[i])) {
      is_set = true;
      t = KV_TOK_CMD;
    }
  }
  if (!is_set)
    t = KV_TOK_STR;
  return t;
}
static token_t get_next_token(bytes *data) {
  token_t t = {.tok_t = KV_TOK_ERR, .v.err = KV_ERR_NOT_INIT};
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
        string *s;
        kv_error_t e = create_string(&s, start, c_count);
        if (KV_IS_ERROR(e)) {
          kv_print_err(e);
          free_str(&s);
          if (IS_EQUAL(t.v.err, KV_ERR_INVAL_ARG))
            t.v.err = KV_ERR_INPUT;
          else
            t.v.err = e;
          t.v.err = e;
          return t;
        }
        t.tok_t = get_tok_type(s);
        if (IS_EQUAL(t.tok_t, KV_TOK_ERR)) {
          free_str(&s);
          t.v.err = KV_ERR_INVAL_TOK;
        }
        t.v.token = s;
        *data = end;
        return t;
      }
      break;
    case KV_PARSE_DOUBLE_QUOTE:
      if (IS_EQUAL(**data, '\''))
        state = KV_PARSE_SINGLE_QUOTE;
      else if (IS_NOT_EQUAL(**data, '"'))
        state = KV_PARSE_NORMAL;
      else {
        end = get_next_char(*data + 1, '"');
        if (IS_EQUAL(*end, '\0')) {
          t.tok_t = KV_TOK_ERR;
          t.v.err = KV_ERR_INVAL_TOK;
          return t;
        }
        start += 1;
        ptrdiff_t c_count = end - start;
        string *s = str_init();
        auto e = create_string(&s, start, c_count);
        if (KV_IS_ERROR(e)) {
          free_str(&s);
          t.tok_t = KV_TOK_ERR;
          if (IS_EQUAL(t.v.err, KV_ERR_INVAL_ARG))
            t.v.err = KV_ERR_INPUT;
          else
            t.v.err = e;
          return t;
        }
        t.tok_t = get_tok_type(s);
        if (IS_EQUAL(t.tok_t, KV_TOK_ERR)) {
          free_str(&s);
          t.v.err = KV_ERR_INVAL_TOK;
        }
        t.v.token = s;
        *data = end + 1;
        return t;
      }
      break;
    case KV_PARSE_SINGLE_QUOTE:
      if (IS_EQUAL(**data, '"'))
        state = KV_PARSE_DOUBLE_QUOTE;
      else if (IS_NOT_EQUAL(**data, '\''))
        state = KV_PARSE_NORMAL;
      else {
        end = get_next_char(*data + 1, '\'');
        if (IS_EQUAL(*end, '\0')) {
          t.tok_t = KV_TOK_ERR;
          t.v.err = KV_ERR_INVAL_TOK;
          return t;
        }
        start += 1;
        ptrdiff_t c_count = end - start;
        string *s = str_init();
        auto e = create_string(&s, start, c_count);
        if (KV_IS_ERROR(e)) {
          free_str(&s);
          t.tok_t = KV_TOK_ERR;
          if (IS_EQUAL(t.v.err, KV_ERR_INVAL_ARG))
            t.v.err = KV_ERR_INPUT;
          else
            t.v.err = e;
          return t;
        }
        t.tok_t = get_tok_type(s);
        if (IS_EQUAL(t.tok_t, KV_TOK_ERR)) {
          free_str(&s);
          t.v.err = KV_ERR_INVAL_TOK;
        }
        t.v.token = s;
        *data = end + 1;
        return t;
      }
      break;
    default:
      return t;
      break;
    }
  }
  return t;
}

void print_all_tokens(tokens_t *tok) {
  if (IS_NULL(tok) || KV_IS_ERROR(tok->err))
    return;
  size_t i = 0;
  while (i < tok->token_count && (tok->tokens)[i] != NULL) {
    auto t = tok->tokens[i];
    printf("[" C_FG_BRIGHT_YELLOW "%02lu" RESET "] [" C_FG_BRIGHT_YELLOW
           "%s" RESET "] ",
           i, get_token_type_str(t->tok_t));
    if (IS_EQUAL(tok->tokens[i]->tok_t, KV_TOK_ERR))
      printf("[" C_FG_BRIGHT_YELLOW "%s" RESET "]\n", get_error_msg(t->v.err));
    else
      printf("[" C_FG_BRIGHT_YELLOW "%s" RESET "]\n", get_c_string(t->v.token));
    i++;
  }
  printf(C_FG_BRIGHT_GREEN "Successfully Parsed input to Tokens!\n" RESET);
}
/* INFO: Public Functions */
void free_tokens(tokens_t *t) {
  if (IS_NOT_NULL(t)) {
    for (size_t i = 0; i < t->token_count; i++) {
      token_t *tok = t->tokens[i];
      if (IS_NOT_NULL(tok)) {
        /*
         * NOTE:
         * .
         * KV_TOK_CMD and KV_TOK_STR contain an owning
         * string whose data was allocated by create_string().
         * .
         * KV_TOK_ERR contains an error value in the union,
         * so there is no string to free.
         */
        if (tok->tok_t == KV_TOK_CMD || tok->tok_t == KV_TOK_STR)
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
tokens_t kv_get_tokens(string *line) {
  tokens_t tkns = {.err = KV_ERR_NOT_INIT,
                   .token_count = 0,
                   .token_capacity = 0,
                   .tokens = NULL};

  if (IS_NULL(line) || !is_str_ok(line)) {
    tkns.err = KV_ERR_INVAL_ARG;
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
  auto data = (bytes)get_c_string(line);

  while (true) {
    token_t t = get_next_token(&data);
    if (IS_EQUAL(t.tok_t, KV_TOK_ERR)) {
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
kv_cmd_t *kv_parse_tokens(const tokens_t *tks) {
  kv_cmd_t *cmd = init_cmd();
  bool is_set = false;
  if (IS_NULL(tks) || KV_IS_ERROR(tks->err) ||
      IS_EQUAL(tks->token_capacity, 0) || IS_EQUAL(tks->token_count, 0) ||
      IS_NULL(tks->tokens)) {
    is_set = true;
    set_cmd_err_value(cmd, KV_ERR_INVAL_ARG);
    return cmd;
  }
  if (IS_NOT_EQUAL(tks->tokens[0]->tok_t, KV_TOK_CMD)) {
    is_set = true;
    set_cmd_err_value(cmd, KV_ERR_TOK_ORDER);
  } else {
    token_t *tok = tks->tokens[0];
    kv_cmd_t *c = init_cmd();
    for (size_t i = 0; i < n_cmds; i++) {
      if (IS_EQUAL(strlen(cmds[i].cmd_str), str_len(tok->v.token)) &&
          !strcmp(get_c_string(tok->v.token), cmds[i].cmd_str)) {
        is_set = true;
        if (IS_NOT_EQUAL(tks->token_count, cmds[i].noOfArgs + 1))
          set_cmd_err_value(c, KV_ERR_CMD_ARGS);
        else {
          set_cmd_type(c, cmds[i].cmd);

          if (IS_NULL(c.val.data)) {
            set_cmd_err_value(c, KV_ERR_MEM_ALLOC);
            return c;
          }
          switch (cmds[i].noOfArgs) {
          case 0:
            set_kv_pair_key_val(c.val.data, NULL, NULL);
            break;
          case 1: {
            string *k = NULL;
            if (IS_NOT_EQUAL(tks->tokens[1]->tok_t, KV_TOK_ERR)) {
              char *raw = get_c_string(tks->tokens[1]->v.token);
              create_string(&k, (bytes)raw, strlen(raw));
            }
            set_kv_pair_key_val(c.val.data, k, NULL);
            break;
          }
          case 2: {
            string *k = NULL;
            string *v = NULL;
            if (IS_NOT_EQUAL(tks->tokens[1]->tok_t, KV_TOK_ERR)) {
              char *raw_k = get_c_string(tks->tokens[1]->v.token);
              create_string(&k, (bytes)raw_k, strlen(raw_k));
            }
            if (IS_NOT_EQUAL(tks->tokens[2]->tok_t, KV_TOK_ERR)) {
              char *raw_v = get_c_string(tks->tokens[2]->v.token);
              create_string(&v, (bytes)raw_v, strlen(raw_v));
            }
            set_kv_pair_key_val(c.val.data, k, v);
            break;
          }
          }
        }
        cmd = c;
        break;
      }
    }
  }
  if (!is_set) {
    cmd.cmd = KV_CMD_ERR;
    cmd.val.err = KV_ERR_INVAL_CMD;
  }
  return cmd;
}
