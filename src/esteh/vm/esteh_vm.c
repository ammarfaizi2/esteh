
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <esteh/vm/esteh_vm.h>
#include <esteh/vm/esteh_token.h>

int fd = 0;
char *fmap = NULL;
size_t fmap_size = 0;
esteh_token **tokens;
uint32_t token_count;
size_t token_cur_size = ESTEH_TOKEN_FIRST_ALLOC;

int esteh_vm(
	char *filename,
	int opt_count,
	argv_opt **opts,
	int argc,
	char **argv
) {

	int exit_code = 0;
	if ((exit_code = esteh_vm_parse_file(filename))) {
		return exit_code;
	}

	if ((exit_code = esteh_lexical_parser())) {
		return exit_code;
	}

	// Unmap the mapped memory and close file descriptor.
	munmap(fmap, fmap_size + 3);
	close(fd);


	// Clean up the tokens heap.
	esteh_token_clean_up();

	return exit_code;
}

int esteh_vm_parse_file(char *filename) {
	fd = open(filename, O_RDONLY);
	if (fd == -1) {
		printf("Could not open input file: \"%s\"\n", filename);
		return 1;
	}
	struct stat st;
	if (fstat(fd, &st) == -1) {
		printf("Could not open input file: \"%s\"\n", filename);
		return 1;	
	}
	fmap_size = st.st_size;
	
	fmap = (char *)mmap(NULL, fmap_size + 3, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);

	// Why the fmap_size + 3?
	// We prevent an invalid address in lexical parser, since it needs to look up the next token.

	return 0;
}
