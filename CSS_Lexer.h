#pragma once
#include "Lexer.h"

class CSS_Lexer : public Lexer
{
public:
	CSS_Lexer();
	virtual ~CSS_Lexer();
	TokenStream Lex(string text, unsigned int count = 1);

private:
	State states[10];
};

