
if (($c[i] >= '0') && ($c[i] <= '9')) {

	uint32_t start = i;
	bool decimal = false;

	i++;
	while (
		(i < fsize) && 
		((($c[i] >= '0') && ($c[i] <= '9')) || ($c[i] == '.'))
	) {
		if ($c[i] == '.') {
			if (decimal) {
				// Syntax Error
			}
			decimal = true;
		}

		i++;
	}

	SET_TOKEN(t_number, &($c[start]), i - start + 1, lineno);
	$t[$ti - 1]->body[i - start] = '\0';
}
