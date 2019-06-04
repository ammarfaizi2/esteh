

if ($c[i] == '"') {
	i++;
	bool is_escaped = false;
	uint32_t string_len = 0;
	char *string = (char *)malloc(4096);

	while ((i < fsize) && ($c[i] != '"' || is_escaped)) {

		if ($c[i] == '\n') {
			lineno++;
		} else if (is_escaped) {
			is_escaped = false;
			#include "escape_char.cxx"
		} else if ($c[i] == '\\') {
			is_escaped = true;
			goto _skip_set;
		}


		string[string_len] = $c[i];
		string_len++;

		_skip_set:
		i++;
	}

	string[string_len] = '\0';

	SET_TOKEN_NP(t_string, string, string_len + 1, lineno);
}
