#include <stdio.h>
#include <kv/init.h>

int main(const int argc, const char **argv) {
    setbuf(stdout, NULL);
    printf("------------------Welcome------------------\n");

    int status = kv_init(argc, argv);

    printf("------------------ThankYou------------------\n");
    return status;
}
