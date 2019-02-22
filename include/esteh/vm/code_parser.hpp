
#ifndef ESTEH_CODE_PARSER_HPP
#define ESTEH_CODE_PARSER_HPP

#include <string.h>
#include <stdlib.h>

class code_parser
{
public:
	code_parser();
	~code_parser();
	void build_opcode();
	void finish();
	int is_ok();
	size_t get_error_length();
	char *get_error();
	void set_file(char *, char *);
	void init_opcache_dir();
private:
	void map_file();
	void buf_read();
	int token_d(char *);
	int file_fd;
	size_t filesize;
	char *map;
	char *filename;
	char *buf_code;
	char *error_parse;
	size_t read_bytes;
};

#endif
