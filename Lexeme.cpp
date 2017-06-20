#include <regex>
using namespace std;

#include "Lexeme.h"

Lexeme::Lexeme(string n, string regx, unsigned int mT)
{
	name = n;
	r = regex(regx);
	moveTo = mT;
}


Lexeme::~Lexeme()
{
}
