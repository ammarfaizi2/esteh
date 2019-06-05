
if ($c[i] == '/') {

	// Singeline comment.
	if ($c[i + 1] == '/') {
		#ifndef COMMENT_IS_NOT_A_TOKEN
			uint32_t start_comment = i;
		#endif

		i += 2;
		while ((i < fsize) && ($c[i] != '\n')) i++;

		#ifndef COMMENT_IS_NOT_A_TOKEN
			SET_TOKEN(t_whitespace, &($c[start_comment]), i - start_comment + 1, lineno - 1);
			$t[$ti - 1]->body[i - start_comment] = '\0';
			goto _start_loop;
		#endif
	} else 

	// Multiline comment
	if ($c[i + 1] == '*') {
		#ifndef COMMENT_IS_NOT_A_TOKEN
			uint32_t start_comment = i;
			uint32_t start_lineno = lineno;
		#endif

		i += 2;
		while (i < fsize) {
			if (($c[i] == '*') && ($c[i+1] == '/')) {
				i+=2;
				break;
			} else if ($c[i] == '\n') {
				lineno++;
			}
			i++;
		}

		#ifndef COMMENT_IS_NOT_A_TOKEN
			SET_TOKEN(t_whitespace, &($c[start_comment]), i - start_comment + 1, start_lineno);
			$t[$ti - 1]->body[i - start_comment] = '\0';
			goto _start_loop;
		#endif
	}
}
