#include <stdio.h>

/*
 * For more info
 * https://en.wikipedia.org/wiki/ANSI_escape_code#Colors
 */
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define LOG_RED(X) printf("%s%s %s\n", ANSI_COLOR_RED, X, ANSI_COLOR_RESET)
#define LOG_GREEN(X) printf("%s%s %s\n", ANSI_COLOR_GREEN, X, ANSI_COLOR_RESET)
#define LOG_YELLOW(X) printf("%s%s %s\n", ANSI_COLOR_YELLOW, X, ANSI_COLOR_RESET)
#define LOG_BLUE(X) printf("%s%s %s\n", ANSI_COLOR_BLUE, X, ANSI_COLOR_RESET)
#define LOG_MAGENTA(X) printf("%s%s %s\n", ANSI_COLOR_MAGENTA, X, ANSI_COLOR_RESET)
#define LOG_CYAN(X) printf("%s%s %s\n", ANSI_COLOR_CYAN, X, ANSI_COLOR_RESET)

int main(int argc, char *argv[])
{
	LOG_RED("This text is RED!");
	LOG_GREEN("This text is GREEN!");
	LOG_YELLOW("This text is YELLOW!");
	LOG_BLUE("This text is BLUE!");
	LOG_MAGENTA("This text is MAGEN!");
	LOG_CYAN("This text is CYAN!");

	return 0;
}
