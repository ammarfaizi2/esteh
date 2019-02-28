
#include <cstdio>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <esteh/macros.hpp>
#include <esteh/vm/token.hpp>
#include <esteh/vm/data_types.hpp>
#include <esteh/vm/esteh_token.hpp>
#include <esteh/vm/parsers/parse_error.hpp>
#include <esteh/vm/parsers/esteh_lexical.hpp>
#include <esteh/vm/executors/esteh_print.hpp>
#include <esteh/vm/executors/operators/simple_arithmetic.hpp>

esteh_lexical::esteh_lexical(char *filename, esteh_token ***tokens) {
	this->filename = filename;
	this->tokens = tokens;
	this->nodes = (tea_node **)malloc(TEA_NODE_FIRST_ALLOC);
	(*this->tokens) = (esteh_token **)malloc(ESTEH_TOKEN_PTR_FIRST_ALLOC);
}

void esteh_lexical::run() {
	this->parse();
	this->analyze_token();
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

void esteh_lexical::parse() {
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

		// Anything..
		if ((($rb >= 'a' && $rb <= 'z') || ($rb >= 'A' && $rb <= 'Z') || ($rb >= 128))) {
			TOKEN_REALLOC
			token[token_size] = $rb;
			token_size++;
			i++;
			while (
				(i < this->filesize) && 
				(
					($rb >= 'a' && $rb <= 'z') || 
					($rb >= 'A' && $rb <= 'Z') || 
					($rb >= '0' && $rb <= '9') ||
					($rb >= 128) 
				)) {
				TOKEN_REALLOC
				token[token_size] = $rb;
				token_size++;
				i++;
			}

			TOKEN_PTR_REALLOC
			token[token_size] = '\0';
			$tk[$tc] = (esteh_token *)malloc(sizeof(esteh_token));
			$tk[$tc]->token = this->tokenize(token, line, &$tk[$tc]);
			$tk[$tc]->line = line;
			$tc++;
			token_size = 0;
			continue;
		}

		if ($rb == '+') {
			TOKEN_PTR_REALLOC
			$tk[$tc] = (esteh_token *)malloc(sizeof(esteh_token));
			$tk[$tc]->token = TA_ADD;
			$tk[$tc]->type = t_operator;
			$tk[$tc]->line = line;
			$tc++;
			token_size = 0;
			continue;
		} else if ($rb == '-') {
			TOKEN_PTR_REALLOC
			$tk[$tc] = (esteh_token *)malloc(sizeof(esteh_token));
			$tk[$tc]->token = TA_MIN;
			$tk[$tc]->type = t_operator;
			$tk[$tc]->line = line;
			$tc++;
			token_size = 0;
			continue;
		} else if ($rb == '*') {
			TOKEN_PTR_REALLOC
			$tk[$tc] = (esteh_token *)malloc(sizeof(esteh_token));
			$tk[$tc]->token = TA_MUL;
			$tk[$tc]->type = t_operator;
			$tk[$tc]->line = line;
			$tc++;
			token_size = 0;
			continue;
		} else if ($rb == '/') {
			TOKEN_PTR_REALLOC
			$tk[$tc] = (esteh_token *)malloc(sizeof(esteh_token));
			$tk[$tc]->token = TA_DIV;
			$tk[$tc]->type = t_operator;
			$tk[$tc]->line = line;
			$tc++;
			token_size = 0;
			continue;
		} else if ($rb == '%') {
			TOKEN_PTR_REALLOC
			$tk[$tc] = (esteh_token *)malloc(sizeof(esteh_token));
			$tk[$tc]->token = TA_MOD;
			$tk[$tc]->type = t_operator;
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
			$tk[$tc]->val_type = ESTEH_STRING;
			memcpy($tk[$tc]->val.str.val, token, token_size);
			$tc++;
			token_size = 0;
			continue;
		}

		if ($rb >= '0' || $rb <= '9') {
			TOKEN_REALLOC
			token[token_size] = $rb;
			token_size++;
			i++;
			while ((i < this->filesize) && ($rb >= '0' && $rb <= '9')) {
				TOKEN_REALLOC
				token[token_size] = $rb;
				token_size++;
				i++;
			}
			i--;
			TOKEN_PTR_REALLOC
			token[token_size] = '\0';
			$tk[$tc] = (esteh_token *)malloc(sizeof(esteh_token));
			$tk[$tc]->token = T_INT;
			$tk[$tc]->line = line;
			$tk[$tc]->type = t_constant;
			$tk[$tc]->val.lval = atoll(token);
			$tk[$tc]->val_type = ESTEH_INT;
			$tc++;
			token_size = 0;
			continue;
		}
	}
	// End parser.

	TOKEN_PTR_REALLOC
	$tk[$tc] = (esteh_token *)malloc(sizeof(esteh_token));
	$tk[$tc]->token = 0;
	$tk[$tc]->type = t_end_of_file;
	$tk[$tc]->line = line;
	$tc++;
	token_size = 0;

	free(token);
	token = nullptr;
	munmap(this->map, this->filesize);
	this->map = nullptr;
	close(this->file_des);
}

void esteh_lexical::analyze_token() {
	#define $aptr this->anal_ptr
	uint32_t tkn_count = this->token_count - 1;
	for ($aptr = 0; $aptr < tkn_count; ++$aptr) {
		// TOKEN_DUMPER($tk[$aptr]);
		this->build_node($tk[$aptr]);
	}
}

void esteh_lexical::build_node(esteh_token *tkn) {

	#define NODE_REALLOC \
		if (((this->node_count + 2) * sizeof(tea_node *)) >= this->cur_node_alloc) { \
			this->cur_node_alloc += TEA_NODE_FIRST_ALLOC; \
			this->nodes = (tea_node **)realloc(this->nodes, this->cur_node_alloc); \
		}

	#define $node this->nodes
	#define $ndc this->node_count

	if (tkn->type == t_keyword) {
		switch (tkn->token) {


			// Start handle print token.
			case T_PRINT: {
				

				NODE_REALLOC
				$node[$ndc] = (tea_node *)malloc(sizeof(tea_node));
				$node[$ndc]->token = T_PRINT;
				$aptr++;
				tea_node **tmp = &($node[$ndc]);
				while ($tk[$aptr + 1]->token != T_SEMICOLON) {

					switch ($tk[$aptr]->type) {
						// A print token at the end of file.
						case t_end_of_file: {
							PARSE_ERROR(
								"syntax error, unexpected end of file in \"%s\" on line \"%d\"",
								this->filename,
								$tk[$aptr + 1]->line
							);
							exit(254);
						}
						break;

						// t_constant (int, string, null)
						case t_constant: {
							if (
								($tk[$aptr + 1]->token == TA_ADD || $tk[$aptr + 1]->token == TA_MIN) &&
								($tk[$aptr + 3]->token == TA_MUL || $tk[$aptr + 3]->token == TA_DIV)
							) {								
								(*tmp)->op1_type = a_tea_node;
								(*tmp)->op1.node = (tea_node *)malloc(sizeof(tea_node));
								(*tmp)->op1.node->token = TA_ADD;

								(*tmp)->op1.node->op1_type = a_constant;
								(*tmp)->op1.node->op1.constant.val.lval = $tk[$aptr]->val.lval;
								(*tmp)->op1.node->op1.constant.data_type = ESTEH_INT;

								(*tmp)->op1.node->op2_type = a_tea_node;
								(*tmp)->op1.node->op2.node = (tea_node *)malloc(sizeof(tea_node));
								(*tmp)->op1.node->op2.node->token = $tk[$aptr + 3]->token;
								(*tmp)->op1.node->op2.node->op1_type = a_constant;
								(*tmp)->op1.node->op2.node->op1.constant.data_type = ESTEH_INT;
								(*tmp)->op1.node->op2.node->op1.constant.val.lval = $tk[$aptr + 2]->val.lval;
								tmp = &((*tmp)->op1.node->op2.node);
							} else if ($tk[$aptr + 3]->token == T_SEMICOLON) {
								(*tmp)->op1_type = a_tea_node;
								(*tmp)->op1.node = (tea_node *)malloc(sizeof(tea_node));
								(*tmp)->op1.node->token = TA_ADD;

								(*tmp)->op1.node->op1_type = a_constant;
								(*tmp)->op1.node->op1.constant.val.lval = $tk[$aptr]->val.lval;
								(*tmp)->op1.node->op1.constant.data_type = ESTEH_INT;

								(*tmp)->op1.node->op2_type = a_constant;
								(*tmp)->op1.node->op2.constant.val.lval = $tk[$aptr + 2]->val.lval;
								(*tmp)->op1.node->op2.constant.data_type = ESTEH_INT; 
							} else {
								(*tmp)->op2_type = a_constant;
								(*tmp)->op2.constant.data_type = ESTEH_INT;
								(*tmp)->op2.constant.val.lval = $tk[$aptr + 2]->val.lval;
							}
						}
						break;

						// t_operator (+, -, *, /. %)
						case t_operator: {

						}
						break;

						case t_symbol:
						case t_keyword:
						case t_unknown:
						break;
					}
					$aptr++;
				}

				node_dumper($node[$ndc]);

			}
			break;
			// End handle print token.


		}
	}	
}

void esteh_lexical::recursive_opeator_scan(
	tea_node **node,
	esteh_token *op1,
	esteh_token *op2,
	bool in_recursive
) {
}


char esteh_lexical::escape_char(char c) {
	#ifndef EXC
		#define ECX(A,B) case A: return B
	#endif

	switch (c) {
		ECX('a', '\a');
		ECX('e', '\e');
		ECX('v', '\v');
		ECX('t', '\t');
		ECX('n', '\n');
	}

	return c;
}

uint16_t esteh_lexical::tokenize(char *token, uint32_t line, esteh_token **tkn = nullptr) {
	
	if (SCMP(token, "p") || SCMP(token, "print") || SCMP(token, "echo")) {
		(*tkn)->type = t_keyword;
		return T_PRINT;
	}

	PARSE_ERROR("syntax error, unknown token '%s' in \"%s\" on line %d", token, this->filename, line);
	exit(254);
}
