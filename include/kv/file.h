#pragma once

#include <kv/init.h>
#include <kv/str.h>

data_node *parse_file_line(str line);
data_node *load_from_file_if_exists(const char *restrict file_path);
