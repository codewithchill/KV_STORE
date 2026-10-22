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
void print_kv_pair(kv_pair *k);
void free_kv_pair(kv_pair *p);

data_node *init_data_node();
kv_error_t set_node_value(data_node *n, string *k, string *v);
kv_error_t store_node(data_node **h, data_node *n);
data_node *get_next_node(data_node *h);
void print_node_data(data_node *d);
void free_data_node(data_node *h);
