
#ifndef ESTEH_ESTEH_OPT_HPP
#define ESTEH_ESTEH_OPT_HPP

#define OPT_LINTER_ONLY 1
#define OPT_NO_DEBUG 2

#include <esteh_debug.h>

typedef struct _esteh_opt {
	int need_param;
	int opt_code;
	#ifdef ESTEH_DEBUG
		char *opt_name;
	#endif
	char *param;
} esteh_opt;

#endif