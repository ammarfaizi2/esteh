
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

	token_count = esteh_vm_lexical_analyze(fmap, fsize, &tokens);

	munmap(fmap, fsize + 4);
	close(fd);


	return exit_code;
}
