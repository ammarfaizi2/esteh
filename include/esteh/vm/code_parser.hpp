
#ifndef ESTEH_CODE_PARSER_HPP
#define ESTEH_CODE_PARSER_HPP

#include <string.h>
#include <stdlib.h>
#include <esteh/vm/opcode.hpp>

class code_parser
{
public:
	int is_ok();
	char *get_error();
	void set_file(char *);
	uint32_t parse_file(esteh_opcode ***);
private:
	uint16_t token_d(char *);
	char escape_char(char);
	void init_opcache_dir();

	int file_fd;
	size_t filesize;
	char *map;
	char *filename;
	char *error_parse;
};

#endif