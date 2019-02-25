
#include <math.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <esteh/error.hpp>
#include <esteh/vm/opcode.hpp>
#include <esteh/vm/esteh_vm.hpp>
#include <esteh/vm/code_parser.hpp>

#include "escape_char.hpp"

#define ESTEH_DIR_OPCACHE "__teacache__"

int code_parser::is_ok() {
	return this->error_parse == nullptr;
}

char *code_parser::get_error() {
	return this->error_parse;
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
	this->map = (char *)mmap(NULL, this->filesize, PROT_READ | PROT_WRITE, MAP_PRIVATE, this->file_fd, 0);
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

uint32_t code_parser::parse_file(esteh_opcode ***opcodes) {

	this->init_opcache_dir();

	#define $opc (*opcodes)
	#define $rb this->map[i]

	#define __er0 "Syntax Error: Unknown token \"%s\" in \"%s\" on line %d"
	#define __er1 "Syntax Error: Unterminated operation in \"%s\" on line %d"
	#define __er2 "Syntax Error: Unterminated string in \"%s\" on line %d"

	#define UNKNOWN_TOKEN \
		this->error_parse = (char *)malloc( \
			sizeof(char) * (sizeof(__er0) + strlen(token) + (floor(log10(line)) + 1) + strlen(this->filename)) \
		); \
		sprintf(this->error_parse, __er0, token, this->filename, line); \
		goto error_clean_up;

	#define UNTERMINATED_OP \
		this->error_parse = (char *)malloc( \
			sizeof(char) * (sizeof(__er1) + (floor(log10(line)) + 1) + strlen(this->filename)) \
		); \
		sprintf(this->error_parse, __er1, this->filename, line); \
		goto error_clean_up;

	#define UNTERMINATED_STRING \
		this->error_parse = (char *)malloc( \
			sizeof(char) * (sizeof(__er1) + (floor(log10(line)) + 1) + strlen(this->filename)) \
		); \
		sprintf(this->error_parse, __er2, this->filename, line); \
		goto error_clean_up;

	#define CLEAND goto cleand

	// Parser conditions.
	bool in_te = false;
	bool in_int = false;
	bool in_dquot = false;
	bool in_comment_sl = false;
	bool in_comment_ml = false;
	bool must_close = false;
	bool dquot_escaped = false;
	uint32_t line = 1;
	uint32_t opcode_count = 0;

	// First init.

	#define TOKEN_ALLOC (sizeof(char) * 1000)
	uint32_t cur_token_alloc = TOKEN_ALLOC;
	char *token = (char *)malloc(TOKEN_ALLOC);
	uint32_t token_size = 0;

	#define TOKENS_ALLOC (sizeof(esteh_token *) * 1000)
	uint32_t cur_tokens_alloc = TOKENS_ALLOC;
	esteh_token **tokens = (esteh_token **)malloc(TOKENS_ALLOC);
	uint32_t tokens_count = 0;

	#define TOKEN_REALLOC \
		if ((token_size + 2) >= cur_token_alloc) { \
			cur_token_alloc = token_size + TOKEN_ALLOC + 2; \
			token = (char *)realloc(token, cur_token_alloc); \
		}

	#define TOKENS_REALLOC \
		if ((tokens_count + 2) >= cur_tokens_alloc) { \
			cur_tokens_alloc = tokens_count + TOKENS_ALLOC + 2; \
			tokens = (esteh_token **)realloc(tokens, cur_tokens_alloc); \
		} \
		tokens[tokens_count] = (esteh_token *)malloc(sizeof(esteh_token));

	for (size_t i = 0; i < this->filesize; ++i) {
		
		if ($rb == '\n') {
			line++;
		}

		/**
		 * String with double quotes terminator.
		 */
		if ($rb == '"') {
			
			if (!in_dquot) {
				in_dquot = 1;
				continue;
			}

			if (dquot_escaped && $rb == '"') {

				TOKEN_REALLOC

				token[token_size] = $rb;
				token_size++;
				continue;
			}

			TOKENS_REALLOC

			token[token_size] = '\0';

			tokens[tokens_count]->lineno = line;
			tokens[tokens_count]->token  = TE_STRING;
			tokens[tokens_count]->val.type = ESTEH_TYPE_STRING;
			tokens[tokens_count]->val.value.str.len = token_size;
			tokens[tokens_count]->val.value.str.val = (char*)malloc(token_size);
			memcpy(
				tokens[tokens_count]->val.value.str.val,
				token,
				token_size
			);

			break;
			in_dquot = 0;
			token_size = 0;
			tokens_count++;
			continue;
		} else if (in_dquot) {

			TOKEN_REALLOC;

			if ($rb == '\\') {
				dquot_escaped = true;
				continue;
			}

			if (dquot_escaped) {
				$rb = this->escape_char($rb);
			}

			token[token_size] = $rb;
			token_size++;
			continue;
		}
		// End of string with double quotes terminator.


	}

	if (in_dquot) {
		UNTERMINATED_STRING
		goto error_clean_up;
	}

	if (in_te) {
		UNTERMINATED_OP
		goto error_clean_up;
	}

	if (must_close) {
		UNTERMINATED_OP
		goto error_clean_up;
	}

	free(token);
	token = nullptr;
	munmap(this->map, this->filesize);
	this->map = nullptr;
	close(this->file_fd);
	return opcode_count;


	error_clean_up:
		free(token);
		token = nullptr;

		// We don't need the parsed opcodes since we won't run due to error.
		for (size_t i = 0; i < opcode_count; ++i) {
			free($opc[i]);
			$opc[i] = nullptr;
		}

		free($opc);
		$opc = nullptr;

		munmap(this->map, this->filesize);
		this->map = nullptr;
		close(this->file_fd);
		return 0;
}
