#include "Token.h"

Token::Token(int ln, string n, string lex)
{
	lineNumber = ln;
	name = n;
	lexeme = lex;
}


Token::~Token()
{
}
