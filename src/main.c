#include <kv/ansi.h>
#include <kv/kv.h>
#include <stdio.h>

// FILE *stdin_;

int main(const int argc, const char **argv) {
    // stdin_ = stdin;
    setbuf(stdout, NULL);
    // freopen("./private/input.txt", "r", stdin);
    printf(C_FG_BRIGHT_PURPLE
           "------------------Welcome------------------\n" RESET);
    int status = kv(argc, argv);
    printf(C_FG_BRIGHT_PURPLE
           "-----------------Thank You------------------\n" RESET);
    return status;
}
