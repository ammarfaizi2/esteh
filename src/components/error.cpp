
#include <esteh/error.hpp>

void _esteh_error(int exit_code, const char *msg, ...) {
	fprintf(stdout, "Error: ");
	va_list arg;
	va_start(arg, msg);
	vfprintf(stdout, msg, arg);
	va_end(arg);
	fprintf(stdout, "\n");
}

void esteh_error(const char *msg, ...) {
	fprintf(stdout, "Error: ");
	va_list arg;
	va_start(arg, msg);
	vfprintf(stdout, msg, arg);
	va_end(arg);
	fprintf(stdout, "\n");
}
