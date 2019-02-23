
char code_parser::escape_char(char chr) {

	#define rc(r,m) case r: return m

	switch (chr) {
		rc('t', '\t');
		rc('n', '\n');
	}

	return chr;
}
