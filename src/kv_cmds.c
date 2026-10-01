#include <stdio.h>

#include <kv/ansi.h>
#include <kv/cmds.h>

void cmd_set(data_node * /*kv_head*/) { return; }
void cmd_help() {
    // clang-format off
    printf(
        C_FG_BRIGHT_YELLOW
            "General Usage: <cmd> <key> <val>\n"
            "\nNOTE: For some commands either <key> or <value> or both may be "
            "optional.\n"
            "Available commands:\n\n"
            BOLD "    SET" NORMAL ": Set the <val> for the <value>\n"
            BOLD "    GET" NORMAL ": Get the value of the <key>\n"
            BOLD "    DEL" NORMAL ": Delete the key-value pair associated with <key>\n"
            BOLD "   SHOW" NORMAL ": Show all the keys and there corresponding value\n"
            BOLD "   HELP" NORMAL ": Prints this help menu.\n"
            BOLD "   EXIT" NORMAL ": Exits the Program.\n"
        RESET
    );
    // clang-format off
}

/*
kv_cmd_t* cmd_init() {
    kv_cmd_t *c = malloc(sizeof(*c));
    return c;
}
*/

void free_cmd(kv_cmd_t *c) {
    if (IS_NOT_NULL(c) && IS_NOT_EQUAL(c->cmd, KV_CMD_ERR)) {
        free_str(c->val.data.key);
        free_str(c->val.data.val);
    }
}
kv_error_t exec_cmd(kv_cmd_t *cmd, bool *is_exit, data_node* kv_head) {
    kv_error_t e = KV_ERR_NOT_INIT;
    if (IS_NULL(cmd) || IS_NULL(is_exit))
        e = KV_ERR_INVAL_ARG;
    if (IS_EQUAL(cmd->cmd, KV_CMD_ERR))
        e = KV_ERR_INVAL_CMD;
    e = KV_ERR_NONE;
    switch (cmd->cmd) {
    case KV_CMD_SET:
        cmd_set(kv_head);
        printf("Setting Key: [%s] with value [%s]\n",
               get_c_string(cmd->val.data.key),
               get_c_string(cmd->val.data.val));
        break;
    case KV_CMD_GET:
        printf("Key: %s\nVal: Unknown\n", get_c_string(cmd->val.data.key));
        break;
    case KV_CMD_DEL:
        printf("Deleting key-value pair with key: %s\n",
               get_c_string(cmd->val.data.key));
        break;
    case KV_CMD_SHOW:
        printf("Printing all key-value pairs.....\n");
        break;
    case KV_CMD_HELP:
        cmd_help();
        break;
    case KV_CMD_EXIT:
        /* TODO: Handling all the memory, file wrintg etc */
        printf(C_FG_BRIGHT_BLUE BOLD RAPID_BLINK "Exiting.........\n" RESET);
        *is_exit = true;
        break;
    default:
        break;
    }
    return e;
}
