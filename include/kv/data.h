#pragma once

#include <kv/error.h>
#include <kv/string.h>

typedef struct kv_pair kv_pair;
typedef struct data_node data_node;

kv_pair *init_key_val();
kv_error_t set_kv_pair_key_val(kv_pair *p, string *restrict key,
                         string *restrict val);
const string *get_key(const kv_pair *restrict p, kv_error_t *e);
const string *get_val(const kv_pair *restrict p, kv_error_t *e);
void free_kv_pair(kv_pair **p);
