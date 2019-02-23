
#include <math.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <esteh/error.hpp>
#include <esteh/vm/opcode.hpp>
#include <esteh/vm/estehvm.hpp>
#include <esteh/vm/code_parser.hpp>

#include "escape_char.hpp"

#define ESTEH_FILE_BUFFER 1024
#define ESTEH_DIR_OPCACHE "__teacache__"

code_parser::code_parser() {
	this->buf_code = (char*)malloc(sizeof(char) * (ESTEH_FILE_BUFFER + 1));
}

void code_parser::finish() {

}

int code_parser::is_ok() {
	return 1;
}

char *code_parser::get_error() {
	return nullptr;
}

size_t code_parser::get_error_length() {
	return 0;
}

int code_parser::token_d(char *token) {

	if (!strcmp(token, "echo")) {
		return TD_ECHO;
	}
	return T_UNKNOWN;
}

void code_parser::set_file(char *filename) {
	this->filename = filename;
	this->file_fd = open(this->filename, O_RDONLY);
	struct stat st;
	if (fstat(this->file_fd, &st)) {
		esteh_error("Could not open file \"%s\"", this->filename);
		exit(1);
	}
	this->filesize = st.st_size;
	this->map = (char *)mmap(NULL, this->filesize, PROT_READ|PROT_WRITE, MAP_PRIVATE, this->file_fd, 0);
}

void code_parser::init_opcache_dir() {
	struct stat st = {0};
		
	if (stat(ESTEH_DIR_OPCACHE, &st) == -1) {
		#ifdef ESTEH_DEBUG
			printf("mkdir %s\n", ESTEH_DIR_OPCACHE);
		#endif
	    mkdir(ESTEH_DIR_OPCACHE, 0700);
	}

}

void code_parser::build_opcode() {

	#define $rb this->map[i]
	this->init_opcache_dir();

	esteh_opcode **opcodes = (esteh_opcode **)malloc(sizeof(esteh_opcode *));
	uint32_t opcode_count = this->parse_file(&opcodes);
	int skip = 0;	
	
	// Run opcode.

	for (uint32_t i = 0; i < opcode_count; ++i) {

		if (skip) {
			skip = 0;
			continue;
		}
		switch (opcodes[i]->code) {
			case TD_ECHO:
				fprintf(stdout, "%s", opcodes[i+1]->content);
				free(opcodes[i+1]->content);
				opcodes[i+1]->content = nullptr;

				free(opcodes[i+1]);
				opcodes[i+1] = nullptr;
				skip = 1;
			break;
		}
		free(opcodes[i]);
		opcodes[i] = nullptr;
	}

	free(opcodes);
	opcodes = nullptr;
}

uint32_t code_parser::parse_file(esteh_opcode ***opcodes) {

	#define $opc (*opcodes)

	// Parser conditions.
	int in_dquo = 0,
		in_te = 0,
		line = 1,
		dquo_escaped = 0;

	char *token = (char *)malloc(sizeof(char));
	size_t token_size = 0;

	uint32_t opcode_count;

	for (size_t i = 0; i < this->filesize; ++i) {

		if ($rb == 10) {
			line++;
		}

		if ($rb == '"') {
			if (in_dquo) {
				// End of a string.

				// Got an opcode.
				token[token_size] = '\0';
				$opc = (esteh_opcode **)realloc($opc, sizeof(esteh_opcode *) * (opcode_count + 1));
				$opc[opcode_count] = (esteh_opcode *)malloc(sizeof(esteh_opcode));
				$opc[opcode_count]->line = line;
				$opc[opcode_count]->code = TE_STRING;
				$opc[opcode_count]->content = (char *)malloc(sizeof(char) * (token_size + 1));
				memcpy($opc[opcode_count]->content, token, sizeof(char) * (token_size + 1));

				opcode_count++;
				in_dquo = 0;
				token_size = 0;
			} else {
				// Start of a string.
				in_dquo = 1;
			}
			continue;

		} else if (in_dquo) {

			if ((!dquo_escaped) && $rb == '\\') {
				dquo_escaped = 1;
				continue;
			}

			if (dquo_escaped) {
				$rb = this->escape_char($rb);
				dquo_escaped = 0;
			}

			token = (char *)realloc(token, token_size + 2);
			token[token_size] = $rb;
			token_size++;
			continue;
		}

		if (
			($rb >= 65 && $rb <= 90) ||
			($rb >= 97 && $rb <= 122) ||
			($rb == 95)
		) {
			if (!in_te) {
				in_te = 1;
			}
			token = (char *)realloc(token, token_size + 2);
			token[token_size] = $rb;
			token_size++;
		} else if (in_te) {

			// Got an opcode.
			token[token_size] = '\0';
			$opc = (esteh_opcode **)realloc($opc, sizeof(esteh_opcode *) * (opcode_count + 1));
			$opc[opcode_count] = (esteh_opcode *)malloc(sizeof(esteh_opcode));
			$opc[opcode_count]->line = line;
			$opc[opcode_count]->code = this->token_d(token);
			if (($opc[opcode_count]->code = this->token_d(token)) == T_UNKNOWN) {
				#define __erp "Syntax Error: Unknown token \"%s\" on line %d\n"
				this->error_parse = (char *)malloc(
					sizeof(char) * (sizeof(__erp) + strlen(token) + (floor(log10(line)) + 1))
				);
				sprintf(this->error_parse, __erp, token, line);
				return 0;
			}
			$opc[opcode_count]->content = nullptr;

			opcode_count++;
			in_te = 0;
			token_size = 0;
		}
	}

	free(token);
	token = nullptr;

	munmap(this->map, this->filesize);
	this->map = nullptr;
	close(this->file_fd);
	return 1;
}
