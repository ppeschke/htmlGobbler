#include <regex>
using namespace std;

#include "Lexeme.h"

Lexeme::Lexeme(string n, string regx, unsigned int mT, bool complex)
{
	name = n;
	if(complex)
		r = new myComplexRegex(regx);
	else
		r = new myRegex(regx);
	moveTo = mT;
}

Lexeme::Lexeme(const Lexeme & orig)
{
	moveTo = orig.moveTo;
	name = orig.name;
	if(typeid((*orig.r)) == typeid(myRegex))
		r = new myRegex(*orig.r);
	else
		r = new myComplexRegex((*static_cast<myComplexRegex*>(orig.r)));
}

Lexeme::Lexeme(Lexeme&& orig)
{
	this->r = orig.r;
	orig.r = nullptr;
	this->name = orig.name;
	this->moveTo = orig.moveTo;
}

Lexeme & Lexeme::operator=(const Lexeme & right)
{
	this->moveTo = right.moveTo;
	this->name = right.name;
	if(typeid((*right.r)) == typeid(myRegex))
		r = new myRegex(*right.r);
	else
		r = new myComplexRegex((*static_cast<myComplexRegex*>(right.r)));
	return (*this);
}

Lexeme::~Lexeme()
{
	if(r != nullptr)
		delete r;
}
