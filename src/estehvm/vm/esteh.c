
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <estehvm/vm/esteh.h>
#include <estehvm/vm/token.h>

uint8_t esteh_vm_run_file(char *file_name, int app_argc, char **app_argv) {
	uint8_t exit_code = 0;
	int fd;
	char *fmap;
	struct stat st;
	size_t fsize;
	bool lexical_error = false;
	char *lexical_error_message = NULL;
	uint32_t token_count = 0;
	esteh_token **tokens;

	fd = open(file_name, O_RDONLY);

	if (fd == -1) {
		printf("Could not open input file: %s\n", file_name);
		exit(1);
	}

	if (fstat(fd, &st) == -1) {
		printf("Could not stat input file: %s\n", file_name);
		exit(1);
	}

	fsize = st.st_size;
	fmap = (char *)mmap(NULL, fsize + 4, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);

	token_count = esteh_vm_lexical_analyze(fmap, fsize, &tokens, &lexical_error, &lexical_error_message);

	if (lexical_error) {
		exit_code = 254;
	}

	munmap(fmap, fsize + 4);
	close(fd);

	esteh_token_clean_up(&tokens, token_count);

	return exit_code;
}

void esteh_token_clean_up(esteh_token ***tokens, uint32_t token_count) {
	for (uint32_t i = 0; i < token_count; ++i) {
		if ((*tokens)[i] != NULL) {
			if ((*tokens)[i]->body != NULL) {
				free((*tokens)[i]->body);
			}
			free((*tokens)[i]);
		}
	}
	free(*tokens);
}
