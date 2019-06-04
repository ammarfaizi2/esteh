
if (($c[i] == ' ') || ($c[i] == '\n') || ($c[i] == '\t') || ($c[i] == '\r')) {
	uint32_t start = i;
	while (($c[i] == ' ') || ($c[i] == '\n' && (++lineno)) || ($c[i] == '\t') || ($c[i] == '\r')) i++;
	SET_TOKEN(t_whitespace, &($c[start]), i - start + 1, lineno - 1);
	$t[$ti - 1]->body[i - start] = '\0';
}
