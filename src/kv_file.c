#include <kv/kv.h>
#include <kv/macros.h>
#include <kv/string.h>
#include <stdio.h>
#include <string.h>

// Incomplete
data_node *parse_file_line(data_node *node, string line) {
    if (!node)
        return NULL;
    if (line.s.capacity < 1 || line.s.byte_len < 1 || !line.s.data)
        return NULL;

    char *colon = strchr((char *)line.s.data, ':');
    if (!colon)
        return NULL;
    *colon = '\0';
    // ptrdiff_t end = (void *)colon - (void *)line.data;
    // string *key = str_conv((const char *)line.data);
    // string *val = str_conv((const char *)(colon + 1));
    return node;
}
data_node *load_from_file_if_exists(const char *restrict file_path) {
    if (!file_path)
        return NULL;

    FILE *f = fopen(file_path, "r");
    if (!f)
        return NULL;
    /*
     * LOOP:
     *      get_line
     *      break if line NULL
     *      parse_line
     *      store_node
     *
     */

    data_node *head = NULL;
    // data_node *tail = NULL;

    string line;
    get_line(&line, f);
    while (IS_NOT_EQUAL(line.s.data, NULL)) {
        if (IS_NOT_EQUAL(0, line.s.capacity)) {
            free_str(&line);
            break;
        }

        // data_node *node = parse_file_line(line);
        // tail = store_node(node, tail);

        free_str(&line);
        get_line(&line, f);
    }
    return head;
}
