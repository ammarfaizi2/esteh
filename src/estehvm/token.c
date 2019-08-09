
#include <esteh/estehvm/token.h>
#include <esteh/estehvm/estehvm.h>

#define TOKENS_ALLOC_GROW_SIZE 1024

uint32_t esteh_vm_lexical_analyze(char *map, size_t map_size, esteh_token ***tokens)
{
	#define $tokens (*tokens)

	size_t i;
	uint32_t
		token_count = 0,
		token_alloc_size = (sizeof(esteh_token *)) * TOKENS_ALLOC_GROW_SIZE;

	$tokens = (esteh_token **)malloc(token_alloc_size);

	for (i = 0; i < map_size; i++) {
		

		// Parse string.
		if (map[i] == '"') {

			#define TOKEN_BODY_ALLOC_GROW_SIZE 128

			i++;
			size_t allocated_body = TOKEN_BODY_ALLOC_GROW_SIZE;

			$tokens[token_count] = (esteh_token *)malloc(sizeof(esteh_token) + allocated_body);
			$tokens[token_count]->body_size = 0;

			while (map[i] != '"') {
				if (map[i] == '\\') {
					switch (map[i+1]) {
						case 'n':
							$tokens[token_count]->body[$tokens[token_count]->body_size] = '\n';
							$tokens[token_count]->body_size++;
				    		i += 2;
						break;
						case 't':
							$tokens[token_count]->body[$tokens[token_count]->body_size] = '\t';
							$tokens[token_count]->body_size++;
				    		i += 2;
						break;
					}
				} else {
					$tokens[token_count]->body[$tokens[token_count]->body_size] = map[i];
				    $tokens[token_count]->body_size++;
				    i++;
				}

			    if (($tokens[token_count]->body_size + 5) > allocated_body) {
			    	allocated_body += TOKEN_BODY_ALLOC_GROW_SIZE;
			    	$tokens[token_count] = (esteh_token *)realloc($tokens[token_count], sizeof(esteh_token) + allocated_body);
			    }
			}

			$tokens[token_count]->type = t_string;
			token_count++;
		}
	}

	#undef $tokens

	return token_count;
}
