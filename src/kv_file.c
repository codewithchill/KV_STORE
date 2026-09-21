#include <kv/init.h>
#include <kv/str.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Incomplete
data_node *parse_file_line(str line) {
    if (line.capacity < 1 || line.len < 1 || !line.data)
        return NULL;

    data_node *__node = malloc(sizeof(data_node));
    if (!__node)
        return NULL;

    char *colon = strchr((char *)line.data, ':');
    if (colon == NULL) {
        free(__node);
        return NULL;
    }
    *colon = '\0';
    // ptrdiff_t end = (void *)colon - (void *)line.data;
    // str *key = str_conv((const char *)line.data);
    // str *val = str_conv((const char *)(colon + 1));
    return __node;
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

    str line = get_line(f);
    while (line.data != NULL) {
        if (0 != line.capacity) {
            free_str(&line);
            break;
        }

        // data_node *node = parse_file_line(line);
        // tail = store_node(node, tail);

        free_str(&line);
        line = get_line(f);
    }
    return head;
}
