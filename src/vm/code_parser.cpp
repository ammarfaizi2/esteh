
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
	this->map = (char *)mmap(NULL, this->filesize + 1, PROT_READ | PROT_WRITE, MAP_PRIVATE, this->file_fd, 0);
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
	#define __er3 "Syntax Error: Token \"%s\" must has an operand in \"%s\" on line %d"

	#define UNKNOWN_TOKEN(token) \
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

	#define TOKEN_DOES_NOT_HAVE_AN_OPERAND(TOKEN_CODE) \
		const char *token_name = get_token_name(TOKEN_CODE); \
		this->error_parse = (char *)malloc( \
			sizeof(char) * (sizeof(__er3) + strlen(token_name) + (floor(log10(line)) + 1) + strlen(this->filename)) \
		); \
		sprintf(this->error_parse, __er3, token_name, this->filename, line); \
		goto error_clean_up;

	#define CLEAND goto cleand

	// Parser conditions.
	bool in_te = false;
	bool in_dquot = false;
	bool dquot_escaped = false;
	bool must_be_closed = false;
	bool must_have_operand = false;
	bool maybe_have_operand = false;
	bool allow_operation = false;
	bool opcode_must_has_operand = false;
	bool in_an_operand = false;
	bool in_int = false;
	uint32_t line = 1;

	// First init.

	#define TOKEN_ALLOC (sizeof(char) * 100)
	uint32_t cur_token_alloc = TOKEN_ALLOC;
	char *token = (char *)malloc(TOKEN_ALLOC);
	uint32_t token_size = 0;

	#define TOKENS_ALLOC (sizeof(esteh_token *) * 100)
	uint32_t cur_tokens_alloc = TOKENS_ALLOC;
	esteh_token **tokens = (esteh_token **)malloc(TOKENS_ALLOC);
	uint32_t tokens_count = 0;
	uint32_t opcode_offset = 0;

	#define OPCODES_ALLOC sizeof(esteh_opcode *) * 1;
	uint32_t cur_opcodes_alloc = OPCODES_ALLOC;
	uint32_t opcode_count = 0;
	*opcodes = (esteh_opcode **)malloc(cur_opcodes_alloc);

	#define TOKEN_REALLOC \
		if ((token_size + 2) >= cur_token_alloc) { \
			cur_token_alloc = token_size + TOKEN_ALLOC + 2; \
			token = (char *)realloc(token, cur_token_alloc); \
		}

	#define TOKENS_REALLOC \
		if ( ((tokens_count + 2) * sizeof(esteh_token *)) >= cur_tokens_alloc) { \
			cur_tokens_alloc = ((tokens_count + 2) * sizeof(esteh_token *)) + TOKEN_ALLOC; \
			tokens = (esteh_token **)realloc(tokens, cur_tokens_alloc); \
		} \
		tokens[tokens_count] = (esteh_token *)malloc(sizeof(esteh_token));

	for (size_t i = 0; i < this->filesize; ++i) {

start_loop:
		if ($rb == '\n') {
			line++;
		}

		/**
		 * If not in a double quotes.
		 */
		if (!(in_dquot || in_int)) {

			/**
			 * The syntax must be closed with semicolon.
			 */
			if ($rb == ';') {

				if (must_have_operand) {
					// Parse error.
					TOKEN_DOES_NOT_HAVE_AN_OPERAND(tokens[tokens_count - 1]->token)
				}

				if (in_an_operand) {
					if (!this->build_opcode(
						opcode_offset,
						tokens_count,
						&opcode_count,
						&cur_opcodes_alloc,
						opcode_must_has_operand,
						tokens,
						opcodes
					)) {
						goto error_clean_up;
					}
					in_an_operand = false;
				}

				must_be_closed = false;
				continue;
			}

			/**
			 * Multi line comment.
			 */
			if ($rb == '/' && this->map[i + 1] == '*') {
				i += 2;
				while (((i - 1) < this->filesize) && (!($rb == '*' && this->map[i + 1] == '/'))) {
					if ($rb == '\n') {
						line++;
					}
					i++;
				}
				i++;
				if ($rb == '\n') {
					line++;
				}
				continue;
			}

			/**
			 * Single line comment.
			 */
			if ($rb == '/' && this->map[i + 1] == '/') {
				i += 2;
				while ((i < this->filesize) && $rb != '\n') i++;
				line++;
				continue;
			}
		}

		/**
		 * String with double quotes terminator.
		 */
		if ($rb == '"') {

			if (!in_dquot) {

				if (must_be_closed && (!must_have_operand)) {
					// Parse error.
					UNTERMINATED_OP
				}

				in_dquot = true;
				continue;
			}

			if (dquot_escaped && $rb == '"') {

				TOKEN_REALLOC

				dquot_escaped = false;

				token[token_size] = $rb;
				token_size++;
				continue;
			}


			/**
			 * Start's end of string.
			 */
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

			in_dquot = 0;
			token_size = 0;
			must_be_closed = true;
			allow_operation = true;
			must_have_operand = false;
			tokens_count++;
			continue;
			// End's end of string.
		} else if (in_dquot) {

			TOKEN_REALLOC

			if (dquot_escaped) {
				$rb = this->escape_char($rb);
				dquot_escaped = false;
			} else if ($rb == '\\') {
				dquot_escaped = true;
				continue;
			}

			token[token_size] = $rb;
			token_size++;
			continue;
		} else // End of string with double quotes terminator.
		
		/**
		 * TE tokens, such as echo, print, etc.
		 */
		if (
			($rb >= 'A' && $rb <= 'Z') ||
			($rb >= 'a' && $rb <= 'z') ||
			($rb == '_')
		) {
			
			if (!in_te) {
				in_te = true;
			}

			TOKEN_REALLOC

			token[token_size] = $rb;
			token_size++;
			continue;

		} else if (in_te) {
			if ($rb >= '0' && $rb <= '9') {

				TOKEN_REALLOC

				token[token_size] = $rb;
				token_size++;
				continue;
			}

			TOKENS_REALLOC
			token[token_size] = '\0';
			tokens[tokens_count]->lineno = line;
			tokens[tokens_count]->token  = this->token_d(token, &must_be_closed, &must_have_operand);
			tokens[tokens_count]->val.type = T_NOT_A_DATA;
			opcode_offset = tokens_count;
			opcode_must_has_operand = must_have_operand;
			in_an_operand = true;

			if (tokens[tokens_count]->token == T_UNKNOWN) {
				UNKNOWN_TOKEN(token)
			}

			if (must_have_operand) {
				if (this->map[i - 1] == ';') {
					// Parse error.
					TOKEN_DOES_NOT_HAVE_AN_OPERAND(tokens[tokens_count]->token)
				}
			}

			in_te = false;
			token_size = 0;
			tokens_count++;
			continue;
		} else if (
			($rb >= '0' && $rb <= '9')
		) {

			if (!in_int) {
				if (must_be_closed && (!must_have_operand)) {
					// Parse error.
					UNTERMINATED_OP
				}

				in_int = true;
			}

			TOKEN_REALLOC

			token[token_size] = $rb;
			token_size++;

		} else if (in_int) {
			/**
			 * End of int.
			 */
			TOKENS_REALLOC

			token[token_size] = '\0';
			tokens[tokens_count]->lineno = line;
			tokens[tokens_count]->token  = TE_INT;
			tokens[tokens_count]->val.type = ESTEH_TYPE_INT;
			tokens[tokens_count]->val.value.lval = atoll(token);

			in_int = false;
			must_be_closed = true;
			allow_operation = true;
			must_have_operand = false;
			maybe_have_operand = true;
			tokens_count++;
			token_size = 0;
			goto start_loop;

		} else if ($rb == '+') {

			TOKENS_REALLOC

			token[token_size] = '\0';
			tokens[tokens_count]->lineno = line;
			tokens[tokens_count]->token  = TF_ADD;

			must_have_operand = true;
			token_size = 0;
			tokens_count++;

		} else if ($rb == '-') {

			TOKENS_REALLOC

			token[token_size] = '\0';
			tokens[tokens_count]->lineno = line;
			tokens[tokens_count]->token  = TF_MIN;

			must_have_operand = true;
			token_size = 0;
			tokens_count++;

		} else if ($rb == '*') {

			TOKENS_REALLOC

			token[token_size] = '\0';
			tokens[tokens_count]->lineno = line;
			tokens[tokens_count]->token  = TF_MUL;

			must_have_operand = true;
			token_size = 0;
			tokens_count++;
		} else if ($rb == '/') {

			TOKENS_REALLOC

			token[token_size] = '\0';
			tokens[tokens_count]->lineno = line;
			tokens[tokens_count]->token  = TF_DIV;

			must_have_operand = true;
			token_size = 0;
			tokens_count++;
		}
	}

	if (must_be_closed) {
		// Parse error.
		UNTERMINATED_OP
	}
	
	free(token);
	token = nullptr;

	free(tokens);
	tokens = nullptr;

	munmap(this->map, this->filesize);
	this->map = nullptr;
	close(this->file_fd);
	return opcode_count;


	error_clean_up:
		free(token);
		token = nullptr;

		free(tokens);
		tokens = nullptr;

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
