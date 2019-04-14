
#include <esteh/vm/estehvm.h>

extern char *fmap;
extern size_t fmap_size;

void vm_lexical() {
	for (int i = 0; i < fmap_size; ++i) {
		printf("%c", fmap[i]);
	}
}
