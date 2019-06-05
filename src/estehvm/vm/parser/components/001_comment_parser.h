
// Comment parser.
if ($c[i] == '/') {

	// Parse Singeline Comment.
	if ($c[i + 1] == '/') {

		#ifndef COMMENT_IS_NOT_A_TOKEN
			uint32_t start_comment = i;

			$t[$ti] = (esteh_token *)malloc(sizeof(esteh_token));
			$t[$ti]->type = t_whitespace;
			$t[$ti]->lineno = lineno;
		#endif

		i++;
		while ((i < fsize) && ($c[i] != '\n')) i++;

		#ifndef COMMENT_IS_NOT_A_TOKEN
			$t[$ti]->body_size = i - start_comment;
			$t[$ti]->body = (char *)malloc($t[$ti]->body_size + 1);
			memcpy($t[$ti]->body, &($c[start_comment]), $t[$ti]->body_size);
			$t[$ti]->body[$t[$ti]->body_size] = '\0';
			$ti++;
		#endif

		continue;
	}

	// Parse Multiline Comment.
	if ($c[i + 1] == '*') {
		
		#ifndef COMMENT_IS_NOT_A_TOKEN
			uint32_t start_comment = i;

			$t[$ti] = (esteh_token *)malloc(sizeof(esteh_token));
			$t[$ti]->type = t_whitespace;
			$t[$ti]->lineno = lineno;
		#endif

		i++;
		while (i < fsize) {
			if (($c[i] == '*') && ($c[i + 1] == '/')) {
				i += 2;
				break;
			}
			if ($c[i] == '\n') lineno++;
			i++;
		}

		#ifndef COMMENT_IS_NOT_A_TOKEN
			$t[$ti]->body_size = i - start_comment;
			$t[$ti]->body = (char *)malloc($t[$ti]->body_size + 1);
			memcpy($t[$ti]->body, &($c[start_comment]), $t[$ti]->body_size);
			$t[$ti]->body[$t[$ti]->body_size] = '\0';
			$ti++;
		#endif

		continue;
	}
}
