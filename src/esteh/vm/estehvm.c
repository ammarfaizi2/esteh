
#include <sys/stat.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <esteh/vm/estehvm.h>
#include <esteh/vm/parser/token.h>
#include <esteh/vm/buffer/buffer.h>
#include <esteh/vm/debugger/token_dumper.h>

extern char *filename;

int esteh_vm_init() {
	init_stdout_buffer();
	init_stderr_buffer();

	return 0;
}

int esteh_vm_shutdown() {
	flush_stdout_buffer();
	flush_stderr_buffer();

	return 0;
}

int estehvm() {
	esteh_vm_init();
	esteh_vm_execute();
	esteh_vm_shutdown();
	return 0;
}

int esteh_vm_execute() {
	int filefd;
	size_t fmap_size = 0;
	char *fmap;

	uint32_t token_amount;
	tea_token **tokens;

	// Init tokens memory allocation.
	tokens = (tea_token **)malloc(TOKENS_CYCLE_ALLOC);

	// Open mapped file.
	fmap = (char *)esteh_vm_openfile(filename, &fmap_size, &filefd);

	// Parse token from file.
	token_amount = esteh_vm_lexical_analyze(fmap, fmap_size, tokens);

	esteh_token_dumper(tokens, token_amount);

	return 0;
}


void *esteh_vm_openfile(char *filename, size_t *filesize, int *filefd) {

	*filefd = open(filename, O_RDONLY);
	if ((*filefd) == -1) {
		printf("Could not open input file: \"%s\"\n", filename);
		exit(1);
	}

	struct stat st;
	if (fstat(*filefd, &st) == -1) {
		printf("Could not open input file: \"%s\"\n", filename);
		exit(1);
	}

	*filesize = st.st_size;

	// Why the fmap_size + 3?
	// We prevent an invalid address in lexical parser, since it needs to look up the next token.
	return mmap(NULL, st.st_size + 3, PROT_READ | PROT_WRITE, MAP_PRIVATE, *filefd, 0);
}
