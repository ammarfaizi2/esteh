
#include <hako/error.hpp>

void _hako_error(int exit_code, const char *msg, ...) {
	fprintf(stderr, "Error: ");
	va_list arg;
	va_start(arg, msg);
	vfprintf(stderr, msg, arg);
	va_end(arg);
	fprintf(stderr, "\n");
	exit(exit_code);
}

void hako_error(const char *msg, ...) {
	fprintf(stderr, "Error: ");
	va_list arg;
	va_start(arg, msg);
	vfprintf(stderr, msg, arg);
	va_end(arg);
	fprintf(stderr, "\n");
	exit(1);
}
