
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <esteh/macros.hpp>
#include <esteh/vm/data_types.hpp>
#include <esteh/vm/esteh_token.hpp>
#include <esteh/vm/parsers/parse_error.hpp>
#include <esteh/vm/parsers/esteh_lexical.hpp>

esteh_lexical::esteh_lexical(char *filename, esteh_token ***tokens) {
	this->filename = filename;
	this->tokens = tokens;
	(*this->tokens) = (esteh_token **)malloc(ESTEH_TOKEN_PTR_FIRST_ALLOC);
}

void esteh_lexical::open_file() {
	this->file_des = open(this->filename, O_RDONLY);

	if (this->file_des == -1) {
		printf("Could not open input file: \"%s\"\n", this->filename);
		exit(1);
	}

	struct stat st;
	if (fstat(this->file_des, &st) == -1) {
		printf("Could not open input file: \"%s\"\n", this->filename);
		exit(1);
	}
	this->filesize = st.st_size;
	this->map = (char *)mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, this->file_des, 0);
}

void esteh_lexical::run() {
	this->open_file();

	#define $rb this->map[i]
	#define $tk (*this->tokens)
	#define $tc this->token_count
	#define TOKEN_REALLOC \
		if ((token_size + 2) >= cur_token_alloc) { \
			cur_token_alloc += ESTEH_TOKEN_FIRST_ALLOC; \
			token = (char *)realloc(token, cur_token_alloc); \
		}
	#define TOKEN_PTR_REALLOC \
		if (((this->token_count + 2) * sizeof(esteh_token *)) >= cur_token_ptr_alloc) { \
			cur_token_ptr_alloc += ESTEH_TOKEN_PTR_FIRST_ALLOC; \
			this->tokens = (esteh_token ***)realloc(this->tokens, cur_token_ptr_alloc); \
		}

	uint32_t line = 1;
	uint32_t token_size = 0;
	uint32_t cur_token_alloc = ESTEH_TOKEN_FIRST_ALLOC;
	uint32_t cur_token_ptr_alloc = ESTEH_TOKEN_PTR_FIRST_ALLOC;

	char *token = (char *)malloc(ESTEH_TOKEN_FIRST_ALLOC);

	// Start parser.
	for (uint32_t i = 0; i < this->filesize; i++) {

		// Skip whitespace.
		if ($rb == ' ' || $rb == '\t' || $rb == '\n' || $rb == '\r') {
			if ($rb == '\n') {
				line++;
			}
			continue;
		}

		#define ANY_COND (($rb >= 'a' && $rb <= 'z') || ($rb >= 'A' && $rb <= 'Z') || ($rb >= 128))
		bool any_cx = ANY_COND;

		// Anything..
		if (any_cx) {
			TOKEN_REALLOC
			token[token_size] = $rb;
			token_size++;

			while (ANY_COND || ($rb >= '0' && $rb <= '9')) {
				TOKEN_REALLOC
				token[token_size] = $rb;
				token_size++;
			}

			TOKEN_PTR_REALLOC
			token[token_size] = '\0';
			$tk[$tc] = (esteh_token *)malloc(sizeof(esteh_token));
			$tk[$tc]->token = this->tokenize(token, line);
			$tk[$tc]->type = t_any;
			$tk[$tc]->line = line;
			$tc++;
			token_size = 0;
		}

		// T_SEMICOLON
		if ($rb == ';') {
			TOKEN_PTR_REALLOC
			$tk[$tc] = (esteh_token *)malloc(sizeof(esteh_token));
			$tk[$tc]->token = T_SEMICOLON;
			$tk[$tc]->type = t_symbol;
			$tk[$tc]->line = line;
			$tc++;
			token_size = 0;
			continue;
		}

		// T_STRING parser.
		if ($rb == '"') {
			bool dquoto_escaped = false;
			i++;
			while ($rb != '"' && (!dquoto_escaped)) {
				if ($rb == '\n') line++;
				if (dquoto_escaped) {
					$rb = this->escape_char($rb);
					dquoto_escaped = false;
				} else if ($rb == '\\') {
					dquoto_escaped = true;
					i++;
					continue;
				}
				TOKEN_REALLOC
				token[token_size] = $rb;
				token_size++;
				if (i >= this->filesize) {
					PARSE_ERROR(
						"syntax error, unexpected end of file, unterminated string declaration in \"%s\" on line \"%d\"",
						this->filename,
						line
					);
					exit(254);
				}
				i++;
			}
			TOKEN_PTR_REALLOC
			token[token_size] = '\0';
			$tk[$tc] = (esteh_token *)malloc(sizeof(esteh_token));
			$tk[$tc]->token = T_STRING;
			$tk[$tc]->type = t_constant;
			$tk[$tc]->line = line;
			$tk[$tc]->val.str.len = token_size;
			$tk[$tc]->val.str.val = (char *)malloc(token_size);
			memcpy($tk[$tc]->val.str.val, token, token_size);
			$tc++;
			token_size = 0;
			continue;
		}

		/**
		 * Comments.
		 */
		if (((i+1) < this->filesize) && $rb == '/') {
			/**
			 * Multiline comment.
			 */
			if (this->map[i + 1] == '*') {
				i += 2;
				while (((i + 1) < this->filesize) && (!($rb == '*' && this->map[i + 1] == '/'))) {
					if ($rb == '\n') line++; 
					i++;
				}
				i++;
				continue;
			}

			/**
			 * Singleline comment.
			 */
			if (this->map[i + 1] == '/') {
				i += 2;
				while (((i + 1) < this->filesize) && ($rb != '\n')) i++;
				line++;
				continue;
			}
		}
	}
	// End parser.

	free(token);
	token = nullptr;
	munmap(this->map, this->filesize);
	this->map = nullptr;
	close(this->file_des);
}

char esteh_lexical::escape_char(char c) {
	#define ECX(A,B) case A: return B

	switch (c) {
		ECX('a', '\a');
		ECX('e', '\e');
		ECX('v', '\v');
		ECX('t', '\t');
		ECX('n', '\n');
	}

	return c;
}

uint16_t esteh_lexical::tokenize(char *token, uint32_t line) {
	
	if (SCMP(token, "p") || SCMP(token, "print") || SCMP(token, "echo")) {
		
		return T_PRINT;
	}

	PARSE_ERROR("syntax error, unknown token '%s' in \"%s\" on line %d", token, this->filename, line);
	exit(254);
}

void esteh_lexical::build_node() {

}
