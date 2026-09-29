#pragma once

#undef IS_EQUAL
#define IS_EQUAL(a,b) ((a)==(b))

#undef IS_NOT_EQUAL
#define IS_NOT_EQUAL(a,b) ((a)!=(b))

#undef IS_NULL
#define IS_NULL(a) IS_EQUAL(a,NULL)

#undef IS_NOT_NULL
#define IS_NOT_NULL(a) IS_NOT_EQUAL(a,NULL)

#undef PRINT_DEBUG_LINE
#define PRINT_DEBUG_LINE printf(C_FG_BRIGHT_BLUE RAPID_BLINK "=====================================\n" RESET);
