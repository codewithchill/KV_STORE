#include "kv/error.h"
#include "kv/macros.h"
#include <kv/data.h>
#include <stdlib.h>
#include <string.h>

typedef struct kv_pair {
  string *key, *val;
} kv_pair;
typedef struct data_node {
  kv_error_t err;
  kv_pair *data;
  struct data_node *next, *prev;
} data_node;

kv_pair *init_key_val() {
  kv_pair *p = malloc(sizeof(*p));
  if (IS_NULL(p))
    return NULL;
  p->key = NULL;
  p->val = NULL;
  return p;
}
void free_kv_pair(kv_pair **p) {
  if (IS_NOT_NULL(p) && IS_NOT_NULL(*p)) {
    if (IS_NOT_NULL((*p)->key))
      free_str(&((*p)->key));
    if (IS_NOT_NULL((*p)->val))
      free_str(&((*p)->val));
    free(*p);
    *p = NULL;
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
  if (IS_NULL(node)) return NULL;
  node->err = KV_ERR_NOT_INIT;
  node->data = NULL;
  node->prev = NULL;
  node->next = NULL;
  return node;
}
void free_data_node_head(data_node **head) {
  if (IS_NOT_NULL(head)) {
    if (IS_NOT_NULL(*head)) {
      free_kv_pair(&((*head)->data));
      free(*head);
      *head = NULL;
    }
  }
}
