
#include <esteh/estehvm/token.h>
#include <esteh/estehvm/estehvm.h>

#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/mman.h>

static const char *cant_open_file = "Couldn't open file: %s\n";

uint8_t esteh_vm_run(esteh_vm_init vm_init)
{
	esteh_token **tokens;
	uint32_t token_count, i;

	token_count = esteh_vm_lexical_analyze(vm_init.map, vm_init.map_size, &tokens);

	for (i = 0; i < token_count; i++) {
		printf("token: %s\n", tokens[i]->body);
	}
}

uint8_t esteh_vm_run_file(esteh_vm_rfile vm_run_file)
{
	int fd;
	uint8_t ret;
	struct stat st;

	fd = open(vm_run_file.file_name, O_RDONLY);
	if (fd == -1) {
		fprintf(stderr, cant_open_file, vm_run_file.file_name);
		ret = 1;
		goto ret_func;
	}

	if (fstat(fd, &st) == -1) {
		fprintf(stderr, cant_open_file, vm_run_file.file_name);
		ret = 1;
		goto close_fd;
	}

	vm_run_file.vm_init->map_size = st.st_size;
	vm_run_file.vm_init->map = (char *)mmap(NULL, st.st_size + 3, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	ret = esteh_vm_run(*(vm_run_file.vm_init));

	munmap(vm_run_file.vm_init->map, st.st_size + 3);

close_fd:
	close(fd);

ret_func:
	return ret;
}
