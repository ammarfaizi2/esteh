
#ifndef ESTEH_VM_PARSERS_ESTEH_LEXICAL_HPP
#define ESTEH_VM_PARSERS_ESTEH_LEXICAL_HPP

class esteh_lexical
{
public:
	esteh_lexical(char *filename, esteh_token ***tokens);
	void run();
private:
	char *filename;
	esteh_token ***tokens;
};

#endif
