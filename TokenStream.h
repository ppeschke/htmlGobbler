#pragma once
#include <vector>
#include "Token.h"

class TokenStream
{
public:
	TokenStream();
	~TokenStream();
	void AddToken(Token t);

	vector<Token> tokens;
};

