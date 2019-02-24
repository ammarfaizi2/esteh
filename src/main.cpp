
/**
 * @author Ammar Faizi <ammarfaizi2@gmail.com> https://www.facebook.com/ammarfaizi2
 * @licese MIT
 * @version 0.0.1
 *
 * Esteh Virtual Machine.
 */

#include <esteh_debug.h>
#include <esteh/error.hpp>
#include <esteh/usage.hpp>
#include <esteh/esteh_opt.hpp>
#include <esteh/vm/estehvm.hpp>
#include <esteh/argv_parser.hpp>

int main(int argc, char *argv[])
{
	if (argc == 1) {
		usage *st = new usage(argv[0]);
		st->general();
		delete st;
		st = nullptr;
		exit(0);
	}

	esteh_opt **opts = (esteh_opt**)malloc(sizeof(esteh_opt *));
	char *filename;
	int opt_count;
	
	// Scope isolation.
	{
		argv_parser *st = new argv_parser;
		opt_count = st->run(argc, argv, &filename, opts);
		delete st;
		st = nullptr;
	}

	// Debug only.
	#ifdef ESTEH_DEBUG
		fprintf(stderr, "Debug Mode: On\n");
		for (int i = 0; i < opt_count; ++i) {
			fprintf(stderr, "opt[%d]:\n", i);
			fprintf(stderr, "  opt_code\t= %d\n", opts[i]->opt_code);
			fprintf(stderr, "  opt_name\t= %s\n", opts[i]->opt_name);
			fprintf(stderr, "  need_param\t= %d\n", opts[i]->need_param);
			fprintf(stderr, "  param\t\t= %s\n", opts[i]->param);	
		}
		fprintf(stderr, "\n");

		// print filename
		fprintf(stderr, "Filename: %s\n", filename);
		fprintf(stderr, "Running esteh...\n\n");
		fprintf(stdout, "Output:\n");
	#endif
	
	// Scope isolation.
	{
		estehvm *st = new estehvm(filename, opt_count, opts);
		st->run();
		delete st;
		st = nullptr;
	}

	free(filename); filename = nullptr;
	free(*opts); *opts = nullptr;
	free(opts); opts = nullptr;

	// Run esteh shutdown.

	// ...
	// ...
	// ...

	// end esteh shutdown.

	return 0;
}