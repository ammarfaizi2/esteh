
#ifndef ESTEH_CODE_PARSER_HPP
#define ESTEH_CODE_PARSER_HPP

#include <string.h>
#include <stdlib.h>
#include <esteh/vm/opcode.hpp>

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
	void set_file(char *);
	void init_opcache_dir();
private:
	void buf_read();
	int token_d(char *);
	uint32_t parse_file(esteh_opcode ***);
	char escape_char(char);
	int file_fd;
	size_t filesize;
	char *map;
	char *filename;
	char *error_parse;
};

#endif