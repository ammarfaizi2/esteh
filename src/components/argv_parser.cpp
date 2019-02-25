
#include <esteh_debug.h>
#include <esteh/error.hpp>
#include <esteh/esteh_opt.hpp>
#include <esteh/argv_parser.hpp>

#include "version.h"

argv_parser::argv_parser() {

}

void argv_parser::a1_opts(int offset, esteh_opt *opt, char *arg, int arglen) {

	opt->param = nullptr;
	this->opt_count++;

	switch (*arg) {
		case 'l':
			#ifdef ESTEH_DEBUG
				opt->opt_name = (char*)malloc(sizeof(char) * (sizeof("linter_only") + 1));
				sprintf(opt->opt_name, "linter_only");
			#endif
			opt->opt_code = OPT_LINTER_ONLY;
			opt->need_param = 0;
		break;

		case 'v':
			#ifdef ESTEH_DEBUG
				opt->opt_name = (char*)malloc(sizeof(char) * (sizeof("version_number") + 1));
				sprintf(opt->opt_name, "version_number");
			#endif
			opt->need_param = 0;
			printf("Esteh %s\n", ESTEH_VERSION);
			exit(0);
		break;

		default:
			esteh_error("Unknown option \"-%c\" (offset %d)", *arg, offset);
		break;
	}

	if (opt->need_param == 1) {
		if (arglen > 1) {
			opt->param = (char*)malloc(sizeof(char) * arglen);
			memcpy(opt->param, arg+1, sizeof(char) * arglen);
		} else if (offset >= (this->argc - 1)) {
			esteh_error("Option \"-%c\" needs a parameter! (offset %d)", *arg, offset);
		}
	} else {
		if (arglen > 1) {
			opt->param = (char*)malloc(sizeof(char) * arglen);
			memcpy(opt->param, arg+1, sizeof(char) * arglen);
			esteh_error(
				"\tOption \"-%c\" doesn't need any parameter. But, a parameter given. (offset %d)"\
				"\n\tDetected parameter: \"-%c\" \"%s\"",
				*arg,
				offset,
				*arg,
				opt->param
			);
		}
	}
}

void argv_parser::a2_opts(int offset, esteh_opt *opt, char *arg, int arglen) {
	opt->param = nullptr;
	this->opt_count++;

	if (!strcmp(arg, "no-debug")) {
		#ifdef ESTEH_DEBUG
			opt->opt_name = (char*)malloc(sizeof(char) * (sizeof("no_debug") + 1));
			sprintf(opt->opt_name, "no_debug");
		#endif
		opt->opt_code = OPT_NO_DEBUG;
		opt->need_param = 0;
	} else if (!strcmp(arg, "version")) {
		#ifdef ESTEH_DEBUG
			opt->opt_name = (char*)malloc(sizeof(char) * (sizeof("version") + 1));
			sprintf(opt->opt_name, "no_debug");
		#endif
		opt->need_param = 0;
		printf("Esteh %s\n", ESTEH_VERSION);
		exit(0);
	} else {
		esteh_error("Unknown option \"--%s\" (offset %d)", arg, offset);
	}
}

int argv_parser::run(int argc, char **argv, char **filename, esteh_opt **opts) {

	this->argc = argc;
	this->argv_ptr = argv;
	this->opt_ptr = opts;

	#define $opts opts[this->opt_count]
	#define $argv argv[i]

	int got_filename = 0;
	size_t l;

	for (int i = 1; i < argc; ++i) {

		if (this->skip == 1) {
			this->skip = 0;
			continue;
		}

		l = strlen($argv);
		$opts = (esteh_opt *)malloc(sizeof(esteh_opt));

		if (l == 0) continue;

		if (l == 1 && $argv[0] == '-') {
			printf("Error: Invalid parameter \"-\" (offset %d)\n", i);
			exit(1);
		}

		if (l >= 2 && $argv[0] == '-') {
			if ($argv[1] == '-') {
				if (l == 2) {
					printf("Error: Invalid parameter \"--\" (offset %d)\n", i);
					exit(1);
				}
				this->a2_opts(i, $opts, $argv+2, l - 2);
				continue;
			}

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
