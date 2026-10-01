#pragma once

#include <kv/error.h>
#include <kv/string.h>

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
typedef struct data_node data_node;
typedef struct data_node {
    kv_error_t err;
    kv_pair data;
    struct data_node *next, *prev;
} data_node;

int kv(const int argc, const char **restrict argv);
