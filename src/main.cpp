
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
		free(st);
		st = nullptr;
		exit(0);
	}

	esteh_opt **opts = (esteh_opt**)malloc(sizeof(esteh_opt *));
	char *filename;
	int opt_count;
	
	{
		argv_parser *st = new argv_parser;
		opt_count = st->run(argc, argv, &filename, opts);
		delete st;
		st = nullptr;
	}

	// Debug only.
	#ifdef ESTEH_DEBUG
		printf("Debug Mode: On\n\n");
		for (int i = 0; i < opt_count; ++i) {
			printf("opt[%d]:\n", i);
			printf("  opt_code\t= %d\n", opts[i]->opt_code);
			printf("  opt_name\t= %s\n", opts[i]->opt_name);
			printf("  need_param\t= %d\n", opts[i]->need_param);
			printf("  param\t\t= %s\n", opts[i]->param);	
		}
		printf("\n");

		// print filename
		printf("Filename: %s\n", filename);
		printf("Running esteh...\n\n");
	#endif
	
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

	// end esteh shutdown.

	return 0;
}