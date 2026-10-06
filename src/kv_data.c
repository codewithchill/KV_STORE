#include <kv/data.h>
#include <kv/error.h>
#include <kv/macros.h>
#include <stdlib.h>
#include <string.h>

struct kv_pair {
  string *key, *val;
};
struct data_node {
  kv_error_t err;
  kv_pair *data;
  struct data_node *next, *prev;
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

data_node *init_data_node() {
  data_node *node = malloc(sizeof(*node));
  if (IS_NULL(node))
    return NULL;
  node->err = KV_ERR_NOT_INIT;
  node->data = NULL;
  node->prev = NULL;
  node->next = NULL;
  return node;
}
kv_error_t get_node_err(data_node *n) {
  if (IS_NULL(n))
    return KV_ERR_INVAL_ARG;
  return n->err;
}
void free_data_node(data_node *h) {
  if (IS_NOT_NULL(h)) {
    if (IS_NOT_NULL(h->next))
      free_data_node(h->next);
    if (IS_NOT_NULL(h->data))
      free_kv_pair(h->data);
    free(h);
  }
}
