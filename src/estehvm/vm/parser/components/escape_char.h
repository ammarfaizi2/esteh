
#define _SS(A,B) \
	case A: \
		string[string_len] = B; \
		string_len++; \
		goto _skip_set; \
	break;

switch ($c[i]) {
	_SS('a', '\a')
	_SS('b', '\b')
	_SS('e', '\e')
	_SS('f', '\f')
	_SS('n', '\n')
	_SS('r', '\r')
	_SS('t', '\t')
	_SS('v', '\v')
}

#undef _SS
