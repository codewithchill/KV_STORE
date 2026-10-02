#include <kv/data.h>
#include <kv/macros.h>
#include <kv/string.h>

// #include <stdio.h>
// #include <string.h>

static data_node *parse_file_line(string *line) {
  if (!is_str_ok(line))
    return NULL;
  /* * Line format: `<key>:<val>` */
  auto const line_len = str_len(line);
  auto const colon_idx = get_char_idx(line, ':');
  auto const val_idx = colon_idx + 1;
  string *k, *v;
  create_string(&k, (bytes)get_c_string(k), colon_idx);
  create_string(&v, (bytes)(get_c_string(k) + val_idx), line_len - val_idx);
  data_node* node = init_data_node();
  set_node_value(&node, )
  return node;
}

/* INFO: Public Functions */
// void save_to_file(data_node* head);
data_node *load_from_file_if_exists(const char *restrict file_path) {
  if (!file_path)
    return NULL;
  FILE *f = fopen(file_path, "r");
  if (!f)
    return NULL;

  data_node *head = NULL;
  string *line = str_init();
  get_line(line, f);
  /*
   * LOOP:
   *      get_line
   *      break if line NULL
   *      parse_line
   *      store_node
   */
  while (!is_str_ok(line)) {
    
  }
  free_str(&line);
  return head;
  fclose(f);
  return NULL;
}
