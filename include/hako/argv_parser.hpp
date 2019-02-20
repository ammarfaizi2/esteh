
#ifndef HAKO_ARGV_PARSER_H
#define HAKO_ARGV_PARSER_H

#include <cstdio>
#include <stdlib.h>
#include <string.h>

#define OPT_LINTER_ONLY 1

class argv_parser
{
public:
	argv_parser();
	void run(int, char ***, char **, char ***);

private:
	void a1_opts(int, char *, char *);
	void a2_opts(int, char *, char *);

	void a1_set(char, int);
	void a2_set(char *, int);

	int skip = 0;
	int a1_options_c;
	int a2_options_c;
	int a1_opt_offset = 0;
	int a2_opt_offset = 1;
	struct a1_opt **a1_options;
	struct a2_opt **a2_options;
};

#endif

