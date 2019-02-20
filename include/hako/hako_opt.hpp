
#ifndef HAKO_HAKO_OPT_H
#define HAKO_HAKO_OPT_H

#define OPT_LINTER_ONLY 1

typedef struct _hako_opt {
	int need_param;
	char *opt_name;
	char *param;
} hako_opt;

#endif