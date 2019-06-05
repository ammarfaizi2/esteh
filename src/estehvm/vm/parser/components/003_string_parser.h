
if ($c[i] == '"') {
	size_t allocated_size = 512;
	size_t strsize = 0;
	char *tmp_string = (char *)malloc(allocated_size + 1);
	bool escaped = false;

	i++;
	while ($c[i] == '"' && (!escaped)) {
		tmp_string[strsize] = $c[i];
	}

}
