
#ifndef HAKO_HAKOVM_HPP
#define HAKO_HAKOVM_HPP

#include <unistd.h>
#include <hako/hako_opt.hpp>

class hakovm
{
public:
	hakovm(char *, int opt_count, hako_opt **);
	void run();
	void init_file_streamer();
	void parse_file();
private:
	int opt_count;
	char *filename;
	hako_opt **opts;
	FILE *hdf;
};

#endif
