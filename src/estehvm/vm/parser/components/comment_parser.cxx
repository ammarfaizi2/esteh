

if (($c[i] == '/')) {
	if ((i < (fsize - 1)) && ($c[i+1] == '/')) {
		i += 2;

		if ((i < (fsize - 1))) {
			while ((i < (fsize - 1)) && $c[i] != '\n') i++;
			if ($c[i] != '\n') {
				lineno++;
			}
		}

		#ifndef COMMENT_IS_NOT_A_TOKEN
			SET_TOKEN(t_whitespace, "\n", sizeof("\n"), lineno - 1);
		#endif

		continue;
	}
}
