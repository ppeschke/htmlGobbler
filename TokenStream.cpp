#include "TokenStream.h"



TokenStream::TokenStream()
{
}


TokenStream::~TokenStream()
{
}

void TokenStream::AddToken(Token t)
{
	tokens.push_back(t);
}
