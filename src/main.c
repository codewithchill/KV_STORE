#include "kv/ansi.h"
#include <kv/kv.h>
#include <stdio.h>

int main(const int argc, const char **argv) {
    setbuf(stdout, NULL);
    printf("------------------Welcome------------------\n" RESET);
    int status = kv(argc, argv);
    printf("------------------ThankYou------------------\n");
    return status;
}
