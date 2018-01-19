#include "State.h"



State::State(unsigned int s)
{
	name = s;
}

State::State() {}

State::~State()
{
}

void State::AddLexeme(string n, string regx, unsigned int ls, bool complex)
{
	lexemes.push_back(Lexeme(n, regx, ls, complex));
}

vector<Lexeme>* State::getLexemes()
{
	return &lexemes;
}
