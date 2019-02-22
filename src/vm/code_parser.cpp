
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <esteh/error.hpp>
#include <esteh/vm/token.hpp>
#include <esteh/vm/estehvm.hpp>
#include <esteh/vm/code_parser.hpp>

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

void code_parser::set_file(char *filename, char *error_parse) {
	this->filename = filename;
	this->error_parse = error_parse;
}

void code_parser::init_opcache_dir() {
	struct stat st = {0};

	if (stat(ESTEH_DIR_OPCACHE, &st) == -1) {
		#ifdef ESTEH_DEBUG
			printf("mkdir __teacache__\n");
		#endif
	    mkdir(ESTEH_DIR_OPCACHE, 0700);
	}
}

void code_parser::map_file() {
	this->file_fd = open(this->filename, O_RDONLY);

	struct stat st = {0};
	if (fstat(this->file_fd, &st) == -1) {
		esteh_error("Cannot open file \"%s\"", this->filename);
		exit(1);
	}
	this->filesize = st.st_size;
	this->map = (char *)mmap(NULL, this->filesize, PROT_READ | PROT_WRITE, MAP_PRIVATE, this->file_fd, 0);
}

void code_parser::build_opcode() {
	this->init_opcache_dir();
	this->map_file();

	#define $rb this->map[i]

	char *token;
	int in_dquo = 0,
		in_te = 0,
		line = 1,
		dquo_escaped = 0;
	esteh_opcode **opcodes = (esteh_opcode **)malloc(sizeof(esteh_opcode *));
	size_t opcodes_size = 0;

	// Initialize token with 1 char allocation.
	token = (char *)malloc(sizeof(char));

	size_t token_size = 0;

	for (size_t i = 0; i < this->filesize; ++i) {
		if ($rb == 10) {
			line++;
		}
		if ($rb == '"') {
			if (in_dquo) {
				/* End of a string. */
				opcodes = (esteh_opcode **)realloc(opcodes, sizeof(esteh_opcode *) * (opcodes_size + 1));
				opcodes[opcodes_size] = (esteh_opcode *)malloc(sizeof(esteh_opcode));
				opcodes[opcodes_size]->line = line;
				opcodes[opcodes_size]->code = TE_STRING;
				opcodes[opcodes_size]->content = (char *)malloc(sizeof(char) * (token_size + 1));
				memcpy(opcodes[opcodes_size]->content, token, sizeof(char) * (token_size + 1));
				opcodes_size++;
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
				switch ($rb) {
					case 'n':
 						$rb = '\n';
					break;
					case 't':
						$rb = '\t';
					break;
				}
				dquo_escaped = 0;
			}

			token = (char *)realloc(token, token_size + 2);
			token[token_size] = $rb;
			token[token_size + 1] = '\0';
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
			token[token_size + 1] = '\0';
			token_size++;
		} else if (in_te) {
			opcodes = (esteh_opcode **)realloc(opcodes, sizeof(esteh_opcode *) * (opcodes_size + 1));
			opcodes[opcodes_size] = (esteh_opcode *)malloc(sizeof(esteh_opcode));
			opcodes[opcodes_size]->line = line;
			opcodes[opcodes_size]->code = this->token_d(token);
			if ((opcodes[opcodes_size]->code = this->token_d(token)) == T_UNKNOWN) {
				fprintf(stderr, "Syntax Error: Unknown token \"%s\" on line %d\n", token, line);
				exit(1);
			}
			opcodes[opcodes_size]->content = nullptr;
			opcodes_size++;
			in_te = 0;
			token_size = 0;
		}
	}

	printf("Before unmap\n");
	sleep(10);

	close(this->file_fd);
	munmap(this->map, this->filesize);

	printf("After unmap\n");
	sleep(10);

	free(token);

	token = nullptr;
	int skip = 0;

	// Run opcode.

	// for (size_t i = 0; i < opcodes_size; ++i) {

	// 	if (skip) {
	// 		skip = 0;
	// 		continue;
	// 	}

	// 	switch (opcodes[i]->code) {
	// 		case TD_ECHO:
	// 			fprintf(stdout, "%s", opcodes[i+1]->content);
	// 			free(opcodes[i+1]->content);
	// 			opcodes[i+1]->content = nullptr;
	// 			skip = 1;
	// 		break;
	// 	}

	// 	free(opcodes[i]);
	// 	opcodes[i] = nullptr;
	// }

	free(opcodes);
	opcodes = nullptr;

}






	// printf("Before...\n");


	// sleep(10);
	// #define $rb this->buf_code[i]


	// do {
	// 	this->buf_read();

	// 	for (size_t i = 0; i < this->read_bytes; ++i) {
			
			
	// 	}

	// 	this->read_bytes = 0;

	// } while (!feof(this->hdf));
	
	// fclose(this->hdf);
	// this->hdf = nullptr;

	// free(token);
	// free(this->buf_code);

	// token = nullptr;
	// this->buf_code = nullptr;

	// int skip = 0;



	
	// printf("\nDone, before freed\n");
	// sleep(10);

	// free(opcodes);
	// opcodes = nullptr;

	// printf("\nDone, freed\n");
	// sleep(10);