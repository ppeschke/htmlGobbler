#pragma once

#include "State.h"
#include "TokenStream.h"

class CSS_Lexer
{
public:
	CSS_Lexer();
	~CSS_Lexer();
	TokenStream Lex(string text, unsigned int count = 1);

private:
	State css_states[10];
};

