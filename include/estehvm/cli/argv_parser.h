
#ifndef estehvm__cli__argv_parser_h
#define estehvm__cli__argv_parser_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void show_help(char *appname);
bool argv_parser(char **argv, int argc, char **error_message, char **file_name, int *app_argc, char ***app_argv);

#endif
