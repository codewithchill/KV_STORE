#include <stdio.h>

#include <kv/ansi.h>
#include <kv/cmds.h>

void cmd_set(data_node * /*kv_head*/) { return; }

void cmd_help() {
    printf(C_FG_BRIGHT_YELLOW
           "General Usage: <cmd> <key> <val>\n"
           "\nNOTE: For some commands either <key> or <value> or both may be "
           "optional.\n"
           "Available commands:\n\n" BOLD "    SET" NORMAL
           ": Set the <val> for the <value>\n" BOLD "    GET" NORMAL
           ": Get the value of the <key>\n" BOLD "    DEL" NORMAL
           ": Delete the key-value pair associated with <key>\n" BOLD
           "   SHOW" NORMAL
           ": Show all the keys and there corresponding value\n" BOLD
           "   HELP" NORMAL ": Prints this help menu.\n" BOLD "   EXIT" NORMAL
           ": Exits the Program.\n" RESET);
}
