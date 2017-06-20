#pragma once

#include <string>

#include "State.h"
#include "TokenStream.h"
using namespace std;

class Lexer
{
public:
	Lexer();
	~Lexer();

	TokenStream Lex(string filename);
private:
	string open(string filename, unsigned int& count);
	State states[16];
};

