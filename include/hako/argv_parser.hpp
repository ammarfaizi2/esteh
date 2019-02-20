
#ifndef __HAKO_ARGV_PARSER_H
#define __HAKO_ARGV_PARSER_H

void argv_parser(
	int argc,
	char ***argv,
	char **filename,
	char ***options
) {

	for (size_t l, int i = 1; i < argc; ++i) {

		l = strlen(*argv[i]);
		*options[i] = (char*)malloc(sizeof(char) * l);

		if (*argv[i][0] == '-') {

			if (l == 1) {
				pritnf("Invalid option \"-\" (offset %d)!\n", i);
				exit(1);
			}

			if (*argv[i][1] == '-') {
				if () {
					
				}
			}
		}
	}

}

#endif
