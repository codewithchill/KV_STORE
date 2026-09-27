#pragma once

#include <kv/kv.h>
#include <kv/string.h>

data_node *parse_file_line(string line);
data_node *load_from_file_if_exists(const char *restrict file_path);
