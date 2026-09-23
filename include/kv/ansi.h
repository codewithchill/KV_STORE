#pragma once

/*
 * INFO:
 * Link : https://en.wikipedia.org/wiki/ANSI_escape_code
 *
 * FIX: Still in development
 */

#define RESET "\033[0m"
#define BOLD "\033[1m"
#define FAINT "\033[2m"
#define ITALIC "\033[3m"
#define UNDERLINE "\033[4m"
#define SLOW_BLINK "\033[5m"
#define RAPID_BLINK "\033[6m"
#define INVERT_COLOR "\033[7m"
#define HIDE "\033[8m"
#define STRIKE "\033[9m"

// INFO Not supported
// #define FONT_DEF "\033[10m"
// #define FONT_1 "\033[11m"
// #define FONT_2 "\033[12m"
// #define FONT_3 "\033[13m"
// #define FONT_4 "\033[14m"
// #define FONT_5 "\033[15m"
// #define FONT_6 "\033[16m"
// #define FONT_7 "\033[17m"
// #define FONT_8 "\033[18m"
// #define FONT_9 "\033[19m"
// #define FONT_GOTHIC "\033[20m"

#define DOUBLY_UNDERLINE "\033[21m"
#define NORMAL "\033[22m"
#define NON_ITALIC "\033[23m"
#define NON_UNDERLINE "\033[24m"
#define NON_BLINK "\033[25m"
#define PROPORTIONAL_SPACE "\033[26m"
#define NON_REVERSE "\033[27m"
#define REVEAL "\033[28m"
#define NON_STRIKE "\033[29m"

#define C_FG_BLACK "\033[30m"
#define C_FG_RED "\033[31m"
#define C_FG_GREEN "\033[32m"
#define C_FG_YELLOW "\033[33m"
#define C_FG_BLUE "\033[34m"
#define C_FG_PURPLE "\033[35m"
#define C_FG_CYAN "\033[36m"
#define C_FG_WHITE "\033[37m"

// #define __A "\033[38m"
// #define __B "\033[39m"

#define C_BG_BLACK "\033[40m"
#define C_BG_RED "\033[41m"
#define C_BG_GREEN "\033[42m"
#define C_BG_YELLOW "\033[43m"
#define C_BG_BLUE "\033[44m"
#define C_BG_PURPLE "\033[45m"
#define C_BG_CYAN "\033[46m"
#define C_BG_WHITE "\033[47m"

// #define __AA "\033[48m"
// #define __BB "\033[49m"

#define NON_PROPORTIONAL_SPACE "\033[50m"
#define FRAMED "\033[51m"
#define ENCIRCLED "\033[52m"
#define OVERLINE "\033[53m"
#define NON_FRAME_ENCIRCLE "\033[54m"
#define NON_OVERLINE "\033[55m"
// #define  "\033[56m"
// #define  "\033[57m"
// #define UNDERLINE_COLOR() "\033[58m"
#define DEF_UNDERLINE_COLOR "\033[59m"

/*
#define  "\033[60m"
#define  "\033[61m"
#define  "\033[62m"
#define  "\033[63m"
#define  "\033[64m"
#define  "\033[65m"
*/
#define SUPER_SCRIPT "\033[73m"
#define SUB_SCRIPT "\033[74m"
#define NON_SUB_SUPER_SCRIPT "\033[75m"

#define C_FG_BRIGHT_BLACK "\033[90m"
#define C_FG_BRIGHT_RED "\033[91m"
#define C_FG_BRIGHT_GREEN "\033[92m"
#define C_FG_BRIGHT_YELLOW "\033[93m"
#define C_FG_BRIGHT_BLUE "\033[94m"
#define C_FG_BRIGHT_PURPLE "\033[95m"
#define C_FG_BRIGHT_CYAN "\033[96m"
#define C_FG_BRIGHT_WHITE "\033[97m"

#define C_BG_BRIGHT_BLACK "\033[100m"
#define C_BG_BRIGHT_RED "\033[101m"
#define C_BG_BRIGHT_GREEN "\033[102m"
#define C_BG_BRIGHT_YELLOW "\033[103m"
#define C_BG_BRIGHT_BLUE "\033[104m"
#define C_BG_BRIGHT_PURPLE "\033[105m"
#define C_BG_BRIGHT_CYAN "\033[106m"
#define C_BG_BRIGHT_WHITE "\033[107m"
