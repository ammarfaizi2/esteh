
if ($c[i] == '"') {
	i++;
	bool is_escaped = false;
	uint32_t string_len = 0;
	uint32_t allocated_string = 512;
	char *string = (char *)malloc(allocated_string);

	while ($c[i] != '"' || is_escaped) {

		if ($c[i] == '\n') {
			lineno++;
		} else if (is_escaped) {
			is_escaped = false;
			#include "escape_char.h"
		} else if ($c[i] == '\\') {
			is_escaped = true;
			goto _skip_set;
		}

		if (i >= fsize) {
			PARSE_ERROR("syntax error, unterminated string in %s on line %d", file_name, lineno);
		}

		string[string_len] = $c[i];
		string_len++;

		_skip_set:
		i++;

		if ((string_len > 512) && ((string_len + 32) >= allocated_string)) {
			allocated_string += 512;
			string = (char *)realloc(string, allocated_string);
		}
	}

	string[string_len] = '\0';
	SET_TOKEN_NP(t_string, string, string_len + 1, lineno);
}
