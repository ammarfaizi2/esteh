
#ifndef HAKO_HAKO_OPT_HPP
#define HAKO_HAKO_OPT_HPP
#define OPT_LINTER_ONLY 1

#include <hako_debug.h>

typedef struct _hako_opt {
	int need_param;
	int opt_code;
	#ifdef HKDBG
		char *opt_name;
	#endif
	char *param;
} hako_opt;

#endif