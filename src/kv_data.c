#include <kv/data.h>
#include <kv/error.h>
#include <kv/macros.h>
#include <kv/string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct kv_pair {
  string *key, *val;
};
struct data_node {
  data_node *next /*, *prev*/;
  kv_pair *data;
  kv_error_t err;
};

kv_pair *init_key_val() {
  kv_pair *p = malloc(sizeof(*p));
  if (IS_NULL(p))
    return NULL;
  p->key = NULL;
  p->val = NULL;
  return p;
}
void free_kv_pair(kv_pair *p) {
  if (IS_NOT_NULL(p)) {
    if (IS_NOT_NULL(p->key))
      free_str(p->key);
    if (IS_NOT_NULL(p->val))
      free_str(p->val);
    free(p);
  }
}
const string *get_key(const kv_pair *restrict p, kv_error_t *e) {
  if (IS_NULL(p) || IS_NULL(e))
    return NULL;
  return p->key;
}
const string *get_val(const kv_pair *restrict p, kv_error_t *e) {
  if (IS_NULL(p) || IS_NULL(e))
    return NULL;
  return p->val;
}
kv_error_t set_kv_pair_key_val(kv_pair *p, string *restrict key,
                               string *restrict val) {
  if (IS_NULL(p))
    return KV_ERR_INVAL_ARG;
  if (IS_NULL(p->key))
    p->key = key;
  if (IS_NULL(p->val))
    p->val = val;
  return KV_ERR_NONE;
}
void print_kv_pair(kv_pair *k) {
  if (IS_NOT_NULL(k))
    fprintf(stdin, "%s : %s\n", get_c_string(k->key), get_c_string(k->val));
}

data_node *init_data_node() {
  data_node *node = malloc(sizeof(*node));
  if (IS_NULL(node))
    return NULL;
  node->err = KV_ERR_NOT_INIT;
  node->data = NULL; // malloc(sizeof(*(node->data)));
  // node->prev = NULL; // malloc(sizeof(*(node->prev)));
  node->next = NULL; // malloc(sizeof(*(node->next)));
  return node;
}
kv_error_t set_node_value(data_node *n, string *k, string *v) {
  if (IS_NULL(n) || IS_NULL(k) || IS_NULL(v))
    return KV_ERR_INVAL_ARG;
  n->err = KV_ERR_NONE;
  n->data = init_key_val();
  if (IS_NULL(n->data))
    return KV_ERR_MEM_ALLOC;
  n->data->key = k;
  n->data->val = v;
  return KV_ERR_NONE;
}
kv_error_t store_node(data_node **h, data_node *n) {
  if (IS_NULL(h) || IS_NULL(n))
    return KV_ERR_INVAL_ARG;
  if (IS_NULL(*h))
    *h = n;
  else {
    auto curr = *h;
    while (IS_NOT_NULL(curr->next))
      curr = curr->next;
    curr->next = n;
  }
  return KV_ERR_NONE;
}
kv_error_t get_node_err(data_node *n) {
  if (IS_NULL(n))
    return KV_ERR_INVAL_ARG;
  return n->err;
}
data_node *get_next_node(data_node *h) {
  if (IS_NULL(h))
    return NULL;
  return h->next;
}
void free_data_node(data_node *h) {
  if (IS_NOT_NULL(h)) {
    if (IS_NOT_NULL(h->data))
      free_kv_pair(h->data);
    while (IS_NOT_NULL(h->next))
      free_data_node(h->next);
    free(h);
  }
}
void print_node_data(data_node *d) {
  if (IS_NOT_NULL(d))
    print_kv_pair(d->data);
}
