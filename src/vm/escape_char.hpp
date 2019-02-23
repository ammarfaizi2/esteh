
char code_parser::escape_char(char chr) {

	#define rc(r,m) case r: return m

	switch (chr) {
		rc('0', '\0');
		rc('1', '\1');
		rc('2', '\2');
		rc('3', '\3');
		rc('4', '\4');
		rc('5', '\5');
		rc('6', '\6');
		rc('7', '\7');
		rc('t', '\t');
		rc('n', '\n');
		rc('r', '\r');
		rc('e', '\e');
		rc('f', '\f');
	}

	return chr;
}
