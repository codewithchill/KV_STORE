#include <kv/data.h>
#include <kv/file.h>
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
  if (colon_idx < 0)
    return NULL;
  auto const val_idx = colon_idx + 1;
  auto const line_c = get_c_string(line);
  string *k = NULL;
  string *v = NULL;
  create_string(&k, (bytes)line_c, colon_idx);
  create_string(&v, (bytes)(line_c + val_idx), line_len - val_idx);
  data_node *node = init_data_node();
  set_node_value(node, k, v);
  return node;
}

/* INFO: Public Functions */
// void save_to_file(data_node *head) {}
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
   *      parse_line
   *      store_node
   */
  while (is_str_ok(line)) {
    auto node = parse_file_line(line);
    if (IS_NULL(node))
      break;
    store_node(&head, node);
    free_str(line);
    line = str_init();
    get_line(line, f);
  }
  free_str(line);
  fclose(f);
  return head;
}
