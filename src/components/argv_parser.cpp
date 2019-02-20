
#include <hako/argv_parser.hpp>

#define A1_OPTIONS_C 1

argv_parser::argv_parser() {
	this->a1_options_c = A1_OPTIONS_C;
	this->a1_options = (struct a1_opt**)malloc(sizeof(struct a1_opt*) * A1_OPTIONS_C);
	// this->a2_options_c = 0;
	// this->a2_options = (**char)malloc(sizeof(char) * this->a2_options_c);

	this->a1_set('l', OPT_LINTER_ONLY);
}

void argv_parser::a1_set(char arg, int opt_code) {
	this->a1_options[this->a1_opt_offset] = (struct a1_opt*)malloc(sizeof(struct a1_opt*));
	this->a1_options[this->a1_opt_offset]->code = arg;
	this->a1_options[this->a1_opt_offset]->opt_code = opt_code;
	this->a1_opt_offset++;
}

void argv_parser::a2_set(char *arg, int opt_code) {

}

void argv_parser::a1_opts(int offset, char *option, char *arg) {

}

void argv_parser::a2_opts(int offset, char *option, char *arg) {
	sprintf(option, "%s", arg);
}

void argv_parser::run(int argc, char ***argv, char **filename, char ***options) {

	size_t l;

	for (int k, i = 1; i < argc; ++i) {

		if (this->skip == 1) {
			this->skip = 0;
			continue;
		}

		k = i - 1;
		l = strlen(argv[0][i]);
		options[0][k] = (char*)malloc(sizeof(char) * l + 1);
		
		if (argv[0][i][0] == '-') {

			if (l == 1) {
				printf("Invalid option \"-\" (offset %d)!\n", i);
				exit(1);
			}

			if (l == 2) {
				this->a1_opts(i, options[0][k], argv[0][i]);
			}

			if (argv[0][i][1] == '-') {
				if (l == 2) {
					printf("Invalid option \"--\" (offset %d)!\n", i);
					exit(1);
				}

				this->a2_opts(i, options[0][k], argv[0][i]);
			}
		}
	}

}
