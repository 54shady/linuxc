#include <stdio.h>
#include <stdlib.h> /* abort */

char error_mode;
FILE *error_log;

/*
 * whatever user put down in place of the
 * ellipsis(...) gets plugged in at the __VA_ARGS__ mark
 */
#define Stopif(assertion, error_action, ...) { \
	if (assertion) { \
		fprintf(error_log ? error_log : stderr, __VA_ARGS__); \
		fprintf(error_log ? error_log : stderr, "\n"); \
		if (error_mode == 's') abort(); \
		else {error_action;} \
	}}
