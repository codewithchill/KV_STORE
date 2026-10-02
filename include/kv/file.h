#pragma once

#include <kv/data.h>
#include <kv/kv.h>
#include <kv/string.h>

void save_to_file(data_node* head);
data_node *load_from_file_if_exists(const char *restrict file_path);
