
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <esteh/argv_parser/parser.h>

extern int app_argc;
extern char **app_argv;
extern char *filename;

inline static void parse_opt1(char *opt, int *ipt);
inline static void parse_opt2(char *opt, int *ipt);

void argv_parser(int argc, char **argv) {
	for (int i = 1; i < argc; ++i) {
		if (argv[i][0] == '-') {
			if (argc == 2) {
				if (argv[i][1] == '-') {
					printf("Invalid option \"--\"\n");
					exit(1);
				}
				parse_opt1(argv[i]+1, &i);
				continue;
			}

			if (argv[i][1] == '-') {
				parse_opt2(argv[i]+2, &i);
			} else {
				parse_opt1(argv[i]+1, &i);
			}
			continue;
		} else {
			filename = argv[i];
		}
	}
}

inline static void parse_opt1(char *opt, int *ipt) {
	
}

inline static void parse_opt2(char *opt, int *ipt) {
	
}
