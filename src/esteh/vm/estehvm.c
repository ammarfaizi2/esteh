
#include <sys/stat.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <esteh/vm/estehvm.h>

extern char *filename;

int filefd;
size_t fmap_size = 0;
char *fmap;

int estehvm() {
	vm_openfile();
	vm_lexical();
	vm_token_clean_up();
	return 0;
}

void vm_openfile() {
	filefd = open(filename, O_RDONLY);

	if (filefd == -1) {
		printf("Could not open input file: \"%s\"\n", filename);
		exit(1);
	}

	struct stat st;
	if (fstat(filefd, &st) == -1) {
		printf("Could not open input file: \"%s\"\n", filename);
		exit(1);
	}

	fmap_size = st.st_size;
	
	fmap = (char *)mmap(NULL, fmap_size + 3, PROT_READ | PROT_WRITE, MAP_PRIVATE, filefd, 0);

	// Why the fmap_size + 3?
	// We prevent an invalid address in lexical parser, since it needs to look up the next token.
}
