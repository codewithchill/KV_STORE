#pragma once

#include <kv/str.h>

typedef struct {
    str *key, *val;
} kv_pair;

typedef struct data_node data_node;
struct data_node {
    kv_pair data;
    data_node *next, *prev;
};

int kv_init(const int argc, const char **restrict argv);
