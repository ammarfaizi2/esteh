
#include <hako/argv_parser.hpp>

#define A1_OPTIONS_C 1

argv_parser::argv_parser() {

}

void argv_parser::a1_set(char arg, int opt_code) {

}

void argv_parser::a2_set(char *arg, int opt_code) {

}

void argv_parser::a1_opts(int offset, hako_opt *opt, char *arg, int arglen) {
	switch (*arg) {
		case 'l':
			#define _a0 "linter_only"
			opt->need_param = 0;
			opt->opt_name = (char*)malloc(sizeof(_a0));
			sprintf(opt->opt_name, _a0);
			opt->param = nullptr;
			this->opt_count++;
		break;
	}
}

void argv_parser::a2_opts(int offset, hako_opt *opt, char *arg) {

}

int argv_parser::run(int argc, char **argv, char **filename, hako_opt **opts) {

	this->opt_ptr = opts;
	this->argv_ptr = argv;

	#define $opts opts[k]
	#define $argv argv[i]

	int got_filename = 0;
	size_t l;

	for (int i = 1, k = 0, j = 0; i < argc; ++i) {

		if (this->skip == 1) {
			this->skip = 0;
			continue;
		}

		l = strlen($argv);
		$opts = (hako_opt *)malloc(sizeof(hako_opt));

		if (l == 0) continue;

		if (l == 1 && $argv[0] == '-') {
			printf("Error: Invalid parameter \"-\" (offset %d)\n", i);
			exit(1);
		}

		if (l == 2 && $argv[0] == '-') {
			this->a1_opts(i, $opts, $argv+1, l - 1);
			continue;
		}

		*filename = (char*)malloc(sizeof(char) * l);
		memcpy(*filename, $argv, sizeof(char) * l);
		got_filename = 1;
	}

	if (!got_filename) {
		*filename = nullptr;
	}

	return this->opt_count;
}
