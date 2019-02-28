
#include <cstdio>
#include <esteh/vm/data_types.hpp>
#include <esteh/vm/esteh_token.hpp>
#include <esteh/vm/executors/esteh_print.hpp>

void esteh_print(void *op1) {
	printf("%s", (char *)op1);
}
