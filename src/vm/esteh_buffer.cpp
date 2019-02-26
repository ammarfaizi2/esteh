
#include <unistd.h>
#include <cstdio>
#include <esteh/vm/esteh_buffer.hpp>

// char *__esteh_buffer = (char *)malloc(sizeof(char));

void esteh_buffer_add(char *str, size_t len, uint8_t _fd = 1) {

	// FILE *fd;
	// switch (_fd) {
	// 	case 0:
	// 		fd = stdin;
	// 	break;
	// 	case 1:
	// 		fd = stdout;
	// 	break;
	// 	case 2:
	// 		fd = stderr;
	// 	break;
	// 	default:
	// 		fprintf(stderr, "Invalid file descriptor!\n");
	// 		exit(1);
	// 	break;
	// }
	// printf("%d\n", str[len - 1]);
	write(_fd, str, len);
	// char buf[len + 1];
	// snprintf(buf, len, "%s", str);
	// write(_fd, buf, len);
}
