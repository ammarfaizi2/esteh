
#ifndef HAKO_HAKOVM_HPP
#define HAKO_HAKOVM_HPP

#include <cstdio>
#include <unistd.h>
#include <hako/hako_opt.hpp>

#define HAKO_FILE_BUFFER 31457280

class hakovm
{
public:
	hakovm(char *, int, hako_opt **ss);
	void run();
	void init_file_streamer();
	void parse_file(int);
	int linter(char **);
private:
	int opt_count;
	char *filename;
	char *error_parse = nullptr;
	hako_opt **opts;
	FILE *hdf;
};

#endif
