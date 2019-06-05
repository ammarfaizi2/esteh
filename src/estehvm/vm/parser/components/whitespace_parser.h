
if (($c[i] == ' ') || ($c[i] == '\n') || ($c[i] == '\t') || ($c[i] == '\r')) {
	uint32_t start = i;
	uint32_t start_lineno = lineno;
	while (($c[i] == ' ') || ($c[i] == '\n' && lineno++) || ($c[i] == '\t') || ($c[i] == '\r')) {
		i++;
	}
	SET_TOKEN(t_whitespace, &($c[start]), i - start + 1, start_lineno);
	$t[$ti - 1]->body[i - start] = '\0';
}
