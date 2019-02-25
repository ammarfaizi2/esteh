
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
	bool in_dquo = false;
	bool in_comment_sl = false;
	bool in_comment_ml = false;
	bool must_close = false;
	bool dquo_escaped = false;
	uint32_t line = 1;

	char *token = (char *)malloc(sizeof(char));
	uint32_t token_size = 0;

	uint32_t opcode_count;

	for (size_t i = 0; i < this->filesize; ++i) {

		/**
		 * Syntax must be cloesd with ';'.
		 */
		if (must_close) {
			if ($rb != '\t' && $rb != ' ' && $rb != '\n' && $rb != ';') {
				UNTERMINATED_OP	
			} else if ($rb == ';') {
				must_close = false;
				continue;
			}
			CLEAND;
		}

		/**
		 * Single line comment.
		 */
		if (in_comment_sl) {
			while ($rb != '\n' && (i < this->filesize)) i++;
			in_comment_sl = false;
		} else {
			if ((!in_dquo) && $rb == '/' && this->map[i + 1] == '/') {
				in_comment_sl = true;
				i++;
				continue;
			}
		}		

		/**
		 * Multi Line Comment.
		 */
		if (in_comment_ml) {
			while ($rb != '*' && this->map[i + 1] != '/' && (i < this->filesize)) i++;
			in_comment_ml = false;
		} else {
			if ((!in_dquo) && $rb == '/' && this->map[i + 1] == '*') {
				in_comment_ml = true;
				i++;
				continue;
			}
		}

		if ($rb == '"') {
			
			if (in_dquo) {

				if (dquo_escaped) {
					$rb = this->escape_char($rb);
					dquo_escaped = 0;
					token = (char *)realloc(token, token_size + 2);
					token[token_size] = $rb;
					token_size++;
					CLEAND;
				}

				// End of a string.
				$opc[opcode_count - 1]->op1.type = ESTEH_TYPE_STRING;
				$opc[opcode_count - 1]->op1.value.str.len = sizeof(char) * (token_size + 1);
				$opc[opcode_count - 1]->op1.value.str.val = (char *)malloc($opc[opcode_count - 1]->op1.value.str.len);
				memcpy($opc[opcode_count - 1]->op1.value.str.val, token, $opc[opcode_count - 1]->op1.value.str.len);
				in_dquo = 0;
				token_size = 0;
				must_close = true;
			} else {
				// Start of a string.
				in_dquo = 1;
			}
			CLEAND;

		} else if (in_dquo) {

			if (dquo_escaped) {
				$rb = this->escape_char($rb);
				dquo_escaped = false;
			} else if ($rb == '\\') {
				dquo_escaped = 1;
				CLEAND;
			}

			token = (char *)realloc(token, token_size + 2);
			token[token_size] = $rb;
			token_size++;
			CLEAND;
		}

		if (
			(!in_dquo) &&
			(($rb >= 65 && $rb <= 90) ||
			($rb >= 97 && $rb <= 122) ||
			($rb == 95))
		) {
			if (!in_te) {
				in_te = 1;
			}
			token = (char *)realloc(token, token_size + 2);
			token[token_size] = $rb;
			token_size++;
			CLEAND;
		} else if (in_te) {

			if ($rb >= 48 && $rb <= 57) {
				token = (char *)realloc(token, token_size + 2);
				token[token_size] = $rb;
				token_size++;
				continue;
			}

			// Got an opcode.
			token[token_size] = '\0';
			$opc = (esteh_opcode **)realloc($opc, sizeof(esteh_opcode *) * (opcode_count + 1));
			$opc[opcode_count] = (esteh_opcode *)malloc(sizeof(esteh_opcode));
			$opc[opcode_count]->lineno = line;
			$opc[opcode_count]->code = this->token_d(token);
			if (($opc[opcode_count]->code = this->token_d(token)) == T_UNKNOWN) {
				UNKNOWN_TOKEN
			}
			$opc[opcode_count]->content = nullptr;
			opcode_count++;
			in_te = false;
			token_size = 0;
		}

		if (
			(!in_dquo) && (!in_te) &&
			($rb >= 48 && $rb <= 57)
		) {
			if (!in_int) {
				in_int = true;
			}
			token = (char *)realloc(token, token_size + 2);
			token[token_size] = $rb;
			token_size++;
			CLEAND;
		} else if (in_int) {
			// Got an opcode.
			token[token_size] = '\0';
			$opc = (esteh_opcode **)realloc($opc, sizeof(esteh_opcode *) * (opcode_count + 1));
			$opc[opcode_count] = (esteh_opcode *)malloc(sizeof(esteh_opcode));
			$opc[opcode_count]->lineno = line;
			$opc[opcode_count]->code = TE_INT;
			$opc[opcode_count]->content = malloc(sizeof(int));
			*((int *)$opc[opcode_count]->content) = atoi(token);
			opcode_count++;
			in_int = false;
			token_size = 0;
		}

		cleand:		
		if ($rb == 10) {
			line++;
		}
	}

	line--;

	if (in_dquo) {
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

		// We don't need parsed opcodes since we won't run due to error.
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
