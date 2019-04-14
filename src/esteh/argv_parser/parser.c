
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <esteh/argv_parser/parser.h>

extern int app_argc;
extern char **app_argv;

inline static void parse_opt1(char *opt);
inline static void parse_opt2(char *opt);

void argv_parser(int argc, char **argv) {
	for (int i = 0; i < argc; ++i) {
		if (argv[i][0] == '-') {
			if (argc == 2) {
				if (argv[i][1] == '-') {
					printf("Invalid option \"--\"\n");
					exit(1);
				}
				parse_opt1(argv[i]+1);
				continue;
			}
		}
	}
}

inline static void parse_opt1(char *opt) {
	
}

inline static void parse_opt2(char *opt) {

}
