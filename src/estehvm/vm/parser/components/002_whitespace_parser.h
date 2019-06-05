
if (($c[i] == ' ') || ($c[i] == '\t') || ($c[i] == '\r') || ($c[i] == '\n')) {

	uint32_t start_whitespace = i;

	$t[$ti] = (esteh_token *)malloc(sizeof(esteh_token));
	$t[$ti]->type = t_whitespace;
	$t[$ti]->lineno = lineno;

	if ($c[i] == '\n') lineno++;

	i++;
	while (($c[i] == ' ') || ($c[i] == '\t') || ($c[i] == '\r') || ($c[i] == '\n' && ++lineno)) i++;

	$t[$ti]->body_size = i - start_whitespace;
	$t[$ti]->body = (char *)malloc($t[$ti]->body_size + 1);
	memcpy($t[$ti]->body, &($c[start_whitespace]), $t[$ti]->body_size);
	$t[$ti]->body[$t[$ti]->body_size] = '\0';
	$ti++;
	continue;
}
