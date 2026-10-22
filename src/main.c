#include <kv/ansi.h>
#include <kv/kv.h>
#include <stdio.h>
#include <stdlib.h>

int main(const int argc, const char **argv) {
  int status = EXIT_SUCCESS;
  setbuf(stdout, NULL);
  printf(C_FG_BRIGHT_PURPLE "------------------ " BOLD "Welcome" NORMAL
                            " ------------------\n" RESET);
  status = kv(argc, argv);
  printf(C_FG_BRIGHT_PURPLE "----------------- " BOLD "Thank You" NORMAL
                            " ------------------\n" RESET);
  return status;
}
