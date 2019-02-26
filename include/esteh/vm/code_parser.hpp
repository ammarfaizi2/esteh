
#ifndef ESTEH_CODE_PARSER_HPP
#define ESTEH_CODE_PARSER_HPP

#include <string.h>
#include <stdint.h>
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
	uint16_t token_d(char *, bool *, bool *);
	char escape_char(char);
	void init_opcache_dir();
	void build_opcode(uint32_t, uint32_t, uint32_t *, uint32_t *, bool, esteh_token **, esteh_opcode ****);

	int file_fd;
	size_t filesize;
	char *map;
	char *filename;
	char *error_parse;
};

const char *get_token_name(uint16_t);

#endif