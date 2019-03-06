
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <esteh/vm/esteh_vm.h>
#include <esteh/vm/esteh_token.h>

int fd = 0;
char *fmap = NULL;
size_t fmap_size = 0;
esteh_token **tokens;

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
	fmap = (char *)mmap(NULL, fmap_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	return 0;
}
